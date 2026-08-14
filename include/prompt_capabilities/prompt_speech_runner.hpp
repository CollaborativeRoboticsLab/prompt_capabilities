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
 * for a speech text. This can be used requesting a new speech text at the beginning or when
 * new data is available.
 */
class PromptSpeechRunner : public PromptServiceRunner
{
public:
  PromptSpeechRunner() : PromptServiceRunner()
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
    std::string task = std::any_cast<std::string>(parameters.get_value("speech_text", std::string{}));
    prompt = "Generate a human like verbal response for " + task + ". Return only the speech text without explanations or comments.";

    RCLCPP_INFO(node_->get_logger(), "prompting with : %s", prompt.c_str());
  }

  virtual capabilities2_events::EventParameters param_on_success() override
  {
    capabilities2_events::EventParameters updated_parameters;

    if (!response_)
      return updated_parameters;

    updated_parameters.set_value("speech", response_->response.response, capabilities2_events::OptionType::STRING);

    RCLCPP_INFO(node_->get_logger(), "updated on_success parameters with received speech text");

    return updated_parameters;
  }
};

}  // namespace capabilities2_runner
