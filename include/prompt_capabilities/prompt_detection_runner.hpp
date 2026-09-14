#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <capabilities2_events/event_parameters.hpp>
#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{
/**
 * @brief Prompt over YOLO detections and emit one semantically selected detection.
 */
class PromptDetectionRunner : public PromptServiceRunner
{
public:
  /**
   * @brief Default constructor required for pluginlib.
   */
  PromptDetectionRunner()
  : PromptServiceRunner()
  {
  }

  /**
   * @brief Build the semantic detection-selection prompt from upstream YOLO outputs.
   *
   * @param parameters Event parameters containing detections and target hints.
   * @param prompt Output prompt text sent to prompt_tools.
   */
  virtual void generate_prompt(capabilities2_events::EventParameters & parameters, std::string & prompt) override
  {
    target_object_ = std::any_cast<std::string>(parameters.get_value("target_object", std::string("")));
    frame_id_ = std::any_cast<std::string>(parameters.get_value("frame_id", std::string("")));
    image_width_ = std::any_cast<int>(parameters.get_value("image_width", 0));
    image_height_ = std::any_cast<int>(parameters.get_value("image_height", 0));

    detection_ids_ = std::any_cast<std::vector<int>>(parameters.get_value("detection_ids", std::vector<int>{}));
    class_ids_ = std::any_cast<std::vector<int>>(parameters.get_value("class_id", std::vector<int>{}));
    class_names_ = std::any_cast<std::vector<std::string>>(parameters.get_value("class_name", std::vector<std::string>{}));
    confidences_ = std::any_cast<std::vector<double>>(parameters.get_value("confidence", std::vector<double>{}));
    bbx_center_x_ = std::any_cast<std::vector<int>>(parameters.get_value("bbx_center_x", std::vector<int>{}));
    bbx_center_y_ = std::any_cast<std::vector<int>>(parameters.get_value("bbx_center_y", std::vector<int>{}));
    bbx_size_w_ = std::any_cast<std::vector<int>>(parameters.get_value("bbx_size_w", std::vector<int>{}));
    bbx_size_h_ = std::any_cast<std::vector<int>>(parameters.get_value("bbx_size_h", std::vector<int>{}));

    prompt =
      "You are selecting one 2D detection for grasp planning based on the semantic target object description. Return JSON only with keys "
      "selected_detection_index and selection_reason. If nothing is suitable, return "
      "{\"selected_detection_index\":-1,\"selection_reason\":\"...\"}. ";

    if (!target_object_.empty())
    {
      prompt += "Target object: '" + target_object_ + "'. ";
    }
    if (!frame_id_.empty())
    {
      prompt += "Detection frame: '" + frame_id_ + "'. ";
    }

    prompt += "Available detections: ";
    for (std::size_t index = 0; index < detection_ids_.size(); ++index)
    {
      prompt += "[index=" + std::to_string(index) +
        ", detection_id=" + std::to_string(value_at(detection_ids_, index, -1)) +
        ", class_id=" + std::to_string(value_at(class_ids_, index, -1)) +
        ", class_name='" + value_at(class_names_, index, std::string("")) +
        "', confidence=" + std::to_string(value_at(confidences_, index, 0.0)) +
        ", bbx_center_x=" + std::to_string(value_at(bbx_center_x_, index, 0)) +
        ", bbx_center_y=" + std::to_string(value_at(bbx_center_y_, index, 0)) +
        ", bbx_size_w=" + std::to_string(value_at(bbx_size_w_, index, 0)) +
        ", bbx_size_h=" + std::to_string(value_at(bbx_size_h_, index, 0)) + "] ";
    }
  }

  /**
   * @brief Parse the LLM response and cache the selected detection fields.
   *
   * @param response Prompt service response.
   */
  virtual void process_response(typename prompt_msgs::srv::Prompt::Response::SharedPtr response) override
  {
    PromptServiceRunner::process_response(response);

    selection_reason_.clear();
    selected_detection_index_ = -1;
    selected_detection_id_ = -1;
    selected_class_id_ = -1;
    selected_class_name_.clear();
    selected_bbx_center_x_ = 0;
    selected_bbx_center_y_ = 0;
    selected_bbx_size_w_ = 0;
    selected_bbx_size_h_ = 0;
    status_message_.clear();

    if (prompt_buffered_)
    {
      status_message_ = "PromptDetectionRunner received a buffered prompt response; a final selection is required.";
      log_failure();
      return;
    }

    try
    {
      const nlohmann::json parsed = nlohmann::json::parse(strip_code_fences(prompt_response_));
      selected_detection_index_ = parsed.value("selected_detection_index", -1);
      selection_reason_ = parsed.value("selection_reason", std::string(""));
    }
    catch (const std::exception & error)
    {
      status_message_ = std::string("Failed to parse PromptDetectionRunner JSON response: ") + error.what();
      log_failure();
      return;
    }

    if (!validate_selection())
    {
      log_failure();
      return;
    }

    selected_detection_id_ = detection_ids_.at(selected_detection_index_);
    selected_class_id_ = class_ids_.at(selected_detection_index_);
    selected_class_name_ = class_names_.at(selected_detection_index_);
    selected_bbx_center_x_ = bbx_center_x_.at(selected_detection_index_);
    selected_bbx_center_y_ = bbx_center_y_.at(selected_detection_index_);
    selected_bbx_size_w_ = bbx_size_w_.at(selected_detection_index_);
    selected_bbx_size_h_ = bbx_size_h_.at(selected_detection_index_);

    RCLCPP_INFO(
      node_->get_logger(),
      "PromptDetectionRunner selected detection index=%d detection_id=%d class_name='%s' detection_count=%zu selection_reason='%s'",
      selected_detection_index_,
      selected_detection_id_,
      selected_class_name_.c_str(),
      detection_ids_.size(),
      selection_reason_.c_str());
  }

  /**
   * @brief Emit the selected detection fields for downstream AnyGrasp filtering.
   *
   * @return Event parameters containing one selected detection.
   */
  virtual capabilities2_events::EventParameters param_on_success() override
  {
    capabilities2_events::EventParameters parameters = PromptServiceRunner::param_on_success();
    parameters.set_value("selected_detection_index", selected_detection_index_, capabilities2_events::OptionType::INT);
    parameters.set_value("detection_id", selected_detection_id_, capabilities2_events::OptionType::INT);
    parameters.set_value("class_id", selected_class_id_, capabilities2_events::OptionType::INT);
    parameters.set_value("class_name", selected_class_name_, capabilities2_events::OptionType::STRING);
    parameters.set_value("bbx_center_x", selected_bbx_center_x_, capabilities2_events::OptionType::INT);
    parameters.set_value("bbx_center_y", selected_bbx_center_y_, capabilities2_events::OptionType::INT);
    parameters.set_value("bbx_size_w", selected_bbx_size_w_, capabilities2_events::OptionType::INT);
    parameters.set_value("bbx_size_h", selected_bbx_size_h_, capabilities2_events::OptionType::INT);
    parameters.set_value("image_width", image_width_, capabilities2_events::OptionType::INT);
    parameters.set_value("image_height", image_height_, capabilities2_events::OptionType::INT);
    parameters.set_value("selection_reason", selection_reason_, capabilities2_events::OptionType::STRING);
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

  virtual bool prompt_uses_cache() const override
  {
    return false;
  }

  virtual bool response_is_success() const override
  {
    return PromptServiceRunner::response_is_success() && status_message_.empty();
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
   * @brief Validate that the YOLO detection arrays are aligned and the selected index is valid.
   *
   * @return True when the selected detection can be projected into downstream fields.
   */
  bool validate_selection()
  {
    if (detection_ids_.empty())
    {
      status_message_ = "No detections available to select.";
      return false;
    }

    const std::size_t detection_count = detection_ids_.size();
    const bool arrays_match =
      class_ids_.size() == detection_count &&
      class_names_.size() == detection_count &&
      confidences_.size() == detection_count &&
      bbx_center_x_.size() == detection_count &&
      bbx_center_y_.size() == detection_count &&
      bbx_size_w_.size() == detection_count &&
      bbx_size_h_.size() == detection_count;
    if (!arrays_match)
    {
      status_message_ = "YOLO detection arrays are inconsistent in length.";
      return false;
    }

    if (selected_detection_index_ < 0 || selected_detection_index_ >= static_cast<int>(detection_count))
    {
      status_message_ = "Selected detection index is out of range.";
      return false;
    }

    return true;
  }

  void log_failure() const
  {
    RCLCPP_WARN(
      node_->get_logger(),
      "PromptDetectionRunner failed: %s detection_count=%zu selected_detection_index=%d response='%s' frame_id='%s' class_ids=%zu class_names=%zu confidences=%zu bbx_center_x=%zu bbx_center_y=%zu bbx_size_w=%zu bbx_size_h=%zu",
      status_message_.c_str(),
      detection_ids_.size(),
      selected_detection_index_,
      prompt_response_.c_str(),
      frame_id_.c_str(),
      class_ids_.size(),
      class_names_.size(),
      confidences_.size(),
      bbx_center_x_.size(),
      bbx_center_y_.size(),
      bbx_size_w_.size(),
      bbx_size_h_.size());
  }

  /**
   * @brief Read a value from a vector with a fallback when the index is out of range.
   *
   * @tparam T Vector element type.
   * @param values Source vector.
   * @param index Requested element index.
   * @param fallback Value returned when the index is invalid.
   * @return Vector element or fallback.
   */
  template<typename T>
  static T value_at(const std::vector<T> & values, std::size_t index, const T & fallback)
  {
    if (index >= values.size())
    {
      return fallback;
    }
    return values[index];
  }

  std::string target_object_;
  std::string frame_id_;
  std::vector<int> detection_ids_;
  std::vector<int> class_ids_;
  std::vector<std::string> class_names_;
  std::vector<double> confidences_;
  std::vector<int> bbx_center_x_;
  std::vector<int> bbx_center_y_;
  std::vector<int> bbx_size_w_;
  std::vector<int> bbx_size_h_;
  int image_width_{0};
  int image_height_{0};
  int selected_detection_index_{-1};
  int selected_detection_id_{-1};
  int selected_class_id_{-1};
  int selected_bbx_center_x_{0};
  int selected_bbx_center_y_{0};
  int selected_bbx_size_w_{0};
  int selected_bbx_size_h_{0};
  std::string selected_class_name_;
  std::string selection_reason_;
  std::string status_message_;
};
}  // namespace capabilities2_runner