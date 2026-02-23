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
 * text values
 */
class PromptTextRunner : public PromptServiceRunner
{
public:
  PromptTextRunner() : PromptServiceRunner()
  {
  }

  /**
   * @brief generate the prompt used for prompting the text.
   *
   * @param parameters tinyXML2 parameters
   * @return std::string
   */
  virtual void generate_prompt(capabilities2_events::EventParameters& parameters, std::string& prompt) override
  {
    std::string data = std::any_cast<std::string>(parameters.get_value("text", std::string{}));

    prompt = "The response was " + data;

    RCLCPP_INFO(node_->get_logger(), "Generated prompt: %s", prompt.c_str());
  }
};

}  // namespace capabilities2_runner
