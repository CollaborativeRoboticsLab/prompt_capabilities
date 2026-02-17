#pragma once
#include <string>
#include <pluginlib/class_list_macros.hpp>
#include <capabilities2_runner/service_runner.hpp>
#include <prompt_msgs/msg/model_option.hpp>
#include <prompt_msgs/srv/prompt.hpp>

namespace capabilities2_runner
{
/**
 * @brief prompt service runner
 *
 * This class is a base class and a wrapper around the capabilities2 service runner
 * and is used to call on the prompt_tools/prompt service, providing it as a capability
 * that prompts capabilitie plans values
 */
class PromptServiceRunner : public ServiceRunner<prompt_msgs::srv::Prompt>
{
public:
  PromptServiceRunner() : ServiceRunner()
  {
  }

  /**
   * @brief Starter function for starting the service runner
   *
   * @param node shared pointer to the capabilities node. Allows to use ros node related functionalities
   * @param run_config runner configuration loaded from the yaml file
   * @param bond_id unique identifier for the group of connections associated with this runner trigger event
   */
  virtual void start(rclcpp::Node::SharedPtr node, const runner_opts& run_config, const std::string& bond_id) override
  {
    init_service(node, run_config, "/prompt/prompt");

    // emit start event
    emit_started(bond_id, "", param_on_started());
  }

protected:
  /**
   * @brief Generate a request from parameters given.
   *
   * This function is used in conjunction with the trigger function to inject type erased parameters
   * into the typed action
   *
   * A pattern needs to be implemented in the derived class
   *
   * @param parameters
   * @return prompt_msgs::srv::Prompt::Request the generated request
   */
  virtual typename prompt_msgs::srv::Prompt::Request generate_request(capabilities2_events::EventParameters& parameters) override
  {
    bool flush = std::any_cast<bool>(parameters.get_value("flush", false));
    RCLCPP_INFO(node_->get_logger(), "Generating prompt request with flush option: %s", flush ? "true" : "false");

    std::string uuid = std::any_cast<std::string>(parameters.get_value("uuid", std::string{}));
    RCLCPP_INFO(node_->get_logger(), "Generating prompt request with uuid: %s", uuid.c_str());

    prompt_msgs::srv::Prompt::Request request;

    if (!uuid.empty())
      request.uuid = uuid;
    
    request.prompt.model_family = "openai";
    request.prompt.use_cache = true;
    request.prompt.use_chat_mode = false;
    request.prompt.flush_cache = flush;

    prompt_msgs::msg::ModelOption modelOption1;
    modelOption1.key = "model";
    modelOption1.value = "gpt-5.1";

    request.prompt.options.push_back(modelOption1);

    prompt_msgs::msg::ModelOption modelOption2;
    modelOption2.key = "stream";
    modelOption2.value = false;
    modelOption2.type = prompt_msgs::msg::ModelOption::BOOL_TYPE;

    request.prompt.options.push_back(modelOption2);

    generate_prompt(parameters, request.prompt.prompt);
    
    return request;
  }

  /**
   * @brief generate the prompt used for prompting the data.
   *
   * @param parameters tinyXML2 parameters
   * @return std::string
   */
  virtual void generate_prompt(capabilities2_events::EventParameters& parameters, std::string& prompt) = 0;

  virtual void process_response(typename prompt_msgs::srv::Prompt::Response::SharedPtr response) override
  {
    if (response->response.buffered)
      RCLCPP_INFO(node_->get_logger(), "information buffered with uuid %s", response->uuid.c_str());
    else
      RCLCPP_INFO(node_->get_logger(), "response received : %s", response->response.response.c_str());
  }
};

}  // namespace capabilities2_runner
