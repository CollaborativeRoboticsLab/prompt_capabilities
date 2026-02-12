#pragma once
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{

/**
 * @brief prompt capabilities runner
 *
 * This class is a wrapper around the capabilities2 service runner and is used to pass
 * data to prompt_tools/prompt service, providing it as a capability that prompts
 * robot capabilities.
 */
class PromptCapabilityRunner : public PromptServiceRunner
{
public:
  PromptCapabilityRunner() : PromptServiceRunner()
  {
  }

  /**
   * @brief generate the prompt used for prompting the capabilities.
   *
   * @param parameters tinyXML2 parameters
   * @return std::string
   */
  virtual void generate_prompt(capabilities2_events::EventParameters& parameters, std::string& prompt, bool& flush) override
  {
    std::string data = "";
    if (parameters.has_value("CapabilitySpecs"))
      data = std::any_cast<std::string>(parameters.get_value("CapabilitySpecs"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'CapabilitySpecs' parameter found in event parameters. Sending empty capabilities.");

    prompt = "The capabilities of the robot are given as follows " + data;
    flush  = false;
  }
};

}  // namespace capabilities2_runner
