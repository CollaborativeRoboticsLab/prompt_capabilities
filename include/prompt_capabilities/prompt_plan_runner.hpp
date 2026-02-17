#pragma once
#include <string>
#include <pluginlib/class_list_macros.hpp>
#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{
/**
 * @brief prompt capability runner
 *
 * This class is a wrapper around the capabilities2 service runner and is used to
 * call on the prompt_tools/prompt service, providing it as a capability that prompts
 * capabilitie plans values
 */
class PromptPlanRunner : public PromptServiceRunner
{
public:
  PromptPlanRunner() : PromptServiceRunner()
  {
  }

  /**
   * @brief generate the prompt used for prompting the occupancy grids.
   *
   * @param parameters tinyXML2 parameters
   * @return std::string
   */
  virtual void generate_prompt(capabilities2_events::EventParameters& parameters, std::string& prompt) override
  {
    bool replan = std::any_cast<bool>(parameters.get_value("replan", false));
    std::string task = std::any_cast<std::string>(parameters.get_value("task", std::string{}));
    std::string failedElements = std::any_cast<std::string>(parameters.get_value("FailedElements", std::string{}));

    if (!replan)
    {
      prompt = "Build a xml plan based on the availbale capabilities to acheive mentioned task of " + task +
               ". Return only the xml plan without explanations or comments.";
    }
    else
    {
      prompt = "Rebuild the xml plan based on the availbale capabilities to acheive mentioned task of " + task +
               ". Just give the xml plan without explanations or comments. These XML  "
               "elements had incompatibilities. " +
               failedElements + "Recorrect them as well";
    }

    RCLCPP_INFO(node_->get_logger(), "prompting with : %s", prompt.c_str());
  }

  /**
   * @brief Update on_success event parameters with new data if avaible.
   *
   * This function is used to inject new data into the XMLElement containing
   * parameters related to the on_success trigger event
   *
   * A pattern needs to be implemented in the derived class
   *
   * @param parameters pointer to the XMLElement containing parameters
   * @return pointer to the XMLElement containing updated parameters
   */
  virtual capabilities2_events::EventParameters param_on_success() override
  {
    std::string document_string = response_->response.response;

    capabilities2_events::EventParameters updated_parameters;
    updated_parameters.set_value("ReceivedPlan", document_string, capabilities2_events::OptionType::STRING);

    RCLCPP_INFO(node_->get_logger(), "updated on_success parameters with received plan");

    return updated_parameters;
  };
};

}  // namespace capabilities2_runner
