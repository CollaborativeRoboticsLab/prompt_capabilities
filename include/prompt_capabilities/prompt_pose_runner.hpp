#pragma once
#include <string>
#include <pluginlib/class_list_macros.hpp>
#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{
/**
 * @brief prompt pose runner
 *
 * This class is a wrapper around the capabilities2 service runner and is used to pass
 * data to prompt_tools/prompt service, providing it as a capability that prompts
 * robot pose values
 */
class PromptPoseRunner : public PromptServiceRunner
{
public:
  PromptPoseRunner() : PromptServiceRunner()
  {
  }

  /**
   * @brief generate the prompt used for prompting the capabilities.
   *
   * @param parameters tinyXML2 parameters
   * @return std::string
   */
  virtual void generate_prompt(capabilities2_events::EventParameters& parameters, std::string& prompt,
                               bool& flush) override
  {
    double x, y, z = 0.0;
    double qx, qy, qz = 0.0;
    double qw = 1.0;

    if (parameters.has_value("x"))
      double x = std::any_cast<double>(parameters.get_value("x"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'x' parameter found in event parameters. Defaulting to 0.");

    if (parameters.has_value("y"))
      double y = std::any_cast<double>(parameters.get_value("y"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'y' parameter found in event parameters. Defaulting to 0.");

    if (parameters.has_value("z"))
      double z = std::any_cast<double>(parameters.get_value("z"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'z' parameter found in event parameters. Defaulting to 0.");

    if (parameters.has_value("qx"))
      double qx = std::any_cast<double>(parameters.get_value("qx"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'qx' parameter found in event parameters. Defaulting to 0.");

    if (parameters.has_value("qy"))
      double qy = std::any_cast<double>(parameters.get_value("qy"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'qy' parameter found in event parameters. Defaulting to 0.");

    if (parameters.has_value("qz"))
      double qz = std::any_cast<double>(parameters.get_value("qz"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'qz' parameter found in event parameters. Defaulting to 0.");

    if (parameters.has_value("qw"))
      double qw = std::any_cast<double>(parameters.get_value("qw"));
    else
      RCLCPP_WARN(node_->get_logger(), "No 'qw' parameter found in event parameters. Defaulting to 1.");

    prompt = "The position of the robot is given as a standard ros2 geometry_msgs::msg::Pose of which the content "
             "are x: " +
             std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z) +
             ", qx: " + std::to_string(qx) + ", qy: " + std::to_string(qy) + ", qz: " + std::to_string(qz) +
             ", qw: " + std::to_string(qw);

    flush = false;

    RCLCPP_INFO(node_->get_logger(), "Prompt generated: %s", prompt.c_str());
  }
};

}  // namespace capabilities2_runner
