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
  virtual void generate_prompt(capabilities2_events::EventParameters& parameters, std::string& prompt) override
  {
    double x, y, z, qx, qy, qz, qw;

    x = std::any_cast<double>(parameters.get_value("x", 0.0));
    y = std::any_cast<double>(parameters.get_value("y", 0.0));
    z = std::any_cast<double>(parameters.get_value("z", 0.0));
    qx = std::any_cast<double>(parameters.get_value("qx", 0.0));
    qy = std::any_cast<double>(parameters.get_value("qy", 0.0));
    qz = std::any_cast<double>(parameters.get_value("qz", 0.0));
    qw = std::any_cast<double>(parameters.get_value("qw", 1.0));

    prompt = "The position of the robot is given as a standard ros2 geometry_msgs::msg::Pose of which the content "
             "are x: " +
             std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z) +
             ", qx: " + std::to_string(qx) + ", qy: " + std::to_string(qy) + ", qz: " + std::to_string(qz) +
             ", qw: " + std::to_string(qw);

    RCLCPP_INFO(node_->get_logger(), "Prompt generated: %s", prompt.c_str());
  }
};

}  // namespace capabilities2_runner
