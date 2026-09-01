#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <capabilities2_events/event_parameters.hpp>
#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{
/**
 * @brief Prompt over AnyGrasp candidates and emit one semantically selected grasp index.
 */
class PromptGraspSelectorRunner : public PromptServiceRunner
{
public:
  /**
   * @brief Default constructor required for pluginlib.
   */
  PromptGraspSelectorRunner()
  : PromptServiceRunner()
  {
  }

  /**
   * @brief Build the semantic grasp-selection prompt from upstream AnyGrasp arrays.
   *
   * @param parameters Event parameters containing grasp candidates and task hints.
   * @param prompt Output prompt text sent to prompt_tools.
   */
  virtual void generate_prompt(capabilities2_events::EventParameters & parameters, std::string & prompt) override
  {
    task_hint_ = std::any_cast<std::string>(parameters.get_value("task_hint", std::string("")));
    frame_ids_ = std::any_cast<std::vector<std::string>>(parameters.get_value("frame_ids", std::vector<std::string>{}));
    positions_x_ = std::any_cast<std::vector<double>>(parameters.get_value("positions_x", std::vector<double>{}));
    positions_y_ = std::any_cast<std::vector<double>>(parameters.get_value("positions_y", std::vector<double>{}));
    positions_z_ = std::any_cast<std::vector<double>>(parameters.get_value("positions_z", std::vector<double>{}));
    orientations_x_ = std::any_cast<std::vector<double>>(parameters.get_value("orientations_x", std::vector<double>{}));
    orientations_y_ = std::any_cast<std::vector<double>>(parameters.get_value("orientations_y", std::vector<double>{}));
    orientations_z_ = std::any_cast<std::vector<double>>(parameters.get_value("orientations_z", std::vector<double>{}));
    orientations_w_ = std::any_cast<std::vector<double>>(parameters.get_value("orientations_w", std::vector<double>{}));

    prompt =
      "You are selecting one grasp pose candidate for execution based on semantic task intent and grasp suitability. Return JSON only with keys "
      "selected_grasp_index and selection_reason. If nothing is suitable, return "
      "{\"selected_grasp_index\":-1,\"selection_reason\":\"...\"}. ";
    if (!task_hint_.empty())
    {
      prompt += "Task hint: '" + task_hint_ + "'. ";
    }
    prompt += "Available grasp candidates: ";

    for (std::size_t index = 0; index < frame_ids_.size(); ++index)
    {
      prompt += "[index=" + std::to_string(index) +
        ", frame_id='" + frame_ids_[index] +
        "', x=" + std::to_string(positions_x_.at(index)) +
        ", y=" + std::to_string(positions_y_.at(index)) +
        ", z=" + std::to_string(positions_z_.at(index)) +
        ", qx=" + std::to_string(orientations_x_.at(index)) +
        ", qy=" + std::to_string(orientations_y_.at(index)) +
        ", qz=" + std::to_string(orientations_z_.at(index)) +
        ", qw=" + std::to_string(orientations_w_.at(index)) + "] ";
    }
  }

  /**
   * @brief Parse the LLM response and cache the selected grasp index.
   *
   * @param response Prompt service response.
   */
  virtual void process_response(typename prompt_msgs::srv::Prompt::Response::SharedPtr response) override
  {
    PromptServiceRunner::process_response(response);

    selected_grasp_index_ = -1;
    selection_reason_.clear();
    status_message_.clear();

    if (prompt_buffered_)
    {
      status_message_ = "PromptGraspSelectorRunner received a buffered prompt response; a final selection is required.";
      return;
    }

    try
    {
      const nlohmann::json parsed = nlohmann::json::parse(strip_code_fences(prompt_response_));
      selected_grasp_index_ = parsed.value("selected_grasp_index", -1);
      selection_reason_ = parsed.value("selection_reason", std::string(""));
    }
    catch (const std::exception & error)
    {
      status_message_ = std::string("Failed to parse PromptGraspSelectorRunner JSON response: ") + error.what();
      return;
    }

    if (!validate_selection())
    {
      return;
    }
  }

  /**
   * @brief Emit the selected grasp index and pass-through candidate arrays.
   *
   * @return Event parameters used by downstream pose selection.
   */
  virtual capabilities2_events::EventParameters param_on_success() override
  {
    capabilities2_events::EventParameters parameters = PromptServiceRunner::param_on_success();
    parameters.set_value("selected_grasp_index", selected_grasp_index_, capabilities2_events::OptionType::INT);
    parameters.set_value("selection_reason", selection_reason_, capabilities2_events::OptionType::STRING);
    parameters.set_value("frame_ids", frame_ids_, capabilities2_events::OptionType::VECTOR_STRING);
    parameters.set_value("positions_x", positions_x_, capabilities2_events::OptionType::VECTOR_DOUBLE);
    parameters.set_value("positions_y", positions_y_, capabilities2_events::OptionType::VECTOR_DOUBLE);
    parameters.set_value("positions_z", positions_z_, capabilities2_events::OptionType::VECTOR_DOUBLE);
    parameters.set_value("orientations_x", orientations_x_, capabilities2_events::OptionType::VECTOR_DOUBLE);
    parameters.set_value("orientations_y", orientations_y_, capabilities2_events::OptionType::VECTOR_DOUBLE);
    parameters.set_value("orientations_z", orientations_z_, capabilities2_events::OptionType::VECTOR_DOUBLE);
    parameters.set_value("orientations_w", orientations_w_, capabilities2_events::OptionType::VECTOR_DOUBLE);
    return parameters;
  }

  /**
   * @brief Emit the prompt-selection failure message.
   *
   * @return Event parameters describing the selection error.
   */
  virtual capabilities2_events::EventParameters param_on_failure() override
  {
    capabilities2_events::EventParameters parameters = PromptServiceRunner::param_on_failure();
    parameters.set_value("message", status_message_, capabilities2_events::OptionType::STRING);
    return parameters;
  }

private:
  /**
   * @brief Strip Markdown code fences around a JSON response, if present.
   *
   * @param text Raw prompt response text.
   * @return Extracted JSON object text when braces are present.
   */
  static std::string strip_code_fences(const std::string & text)
  {
    const std::size_t first_brace = text.find('{');
    const std::size_t last_brace = text.rfind('}');
    if (first_brace != std::string::npos && last_brace != std::string::npos && last_brace >= first_brace)
    {
      return text.substr(first_brace, last_brace - first_brace + 1U);
    }
    return text;
  }

  /**
   * @brief Validate that the AnyGrasp pose arrays are aligned and the selected index is valid.
   *
   * @return True when the selected grasp can be consumed downstream.
   */
  bool validate_selection()
  {
    const std::size_t pose_count = frame_ids_.size();
    if (pose_count == 0U)
    {
      status_message_ = "No grasp candidates available to select.";
      return false;
    }

    const bool arrays_match =
      positions_x_.size() == pose_count &&
      positions_y_.size() == pose_count &&
      positions_z_.size() == pose_count &&
      orientations_x_.size() == pose_count &&
      orientations_y_.size() == pose_count &&
      orientations_z_.size() == pose_count &&
      orientations_w_.size() == pose_count;
    if (!arrays_match)
    {
      status_message_ = "AnyGrasp grasp arrays are inconsistent in length.";
      return false;
    }

    if (selected_grasp_index_ < 0 || selected_grasp_index_ >= static_cast<int>(pose_count))
    {
      status_message_ = "Selected grasp index is out of range.";
      return false;
    }

    return true;
  }

  std::string task_hint_;
  std::vector<std::string> frame_ids_;
  std::vector<double> positions_x_;
  std::vector<double> positions_y_;
  std::vector<double> positions_z_;
  std::vector<double> orientations_x_;
  std::vector<double> orientations_y_;
  std::vector<double> orientations_z_;
  std::vector<double> orientations_w_;
  int selected_grasp_index_{-1};
  std::string selection_reason_;
  std::string status_message_;
};
}  // namespace capabilities2_runner