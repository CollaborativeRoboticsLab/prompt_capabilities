#pragma once

#include <string>

#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{
/**
 * @brief Prompt the current manipulator Cartesian pose through prompt_tools.
 */
class PromptCurrentCartesianPoseRunner : public PromptServiceRunner
{
public:
  /**
   * @brief Default constructor required for pluginlib.
   */
  PromptCurrentCartesianPoseRunner()
  : PromptServiceRunner()
  {
  }

  /**
   * @brief Build a prompt describing the current manipulator Cartesian pose.
   *
   * @param parameters Event parameters containing the resolved pose.
   * @param prompt Output prompt string sent to prompt_tools.
   */
  void generate_prompt(capabilities2_events::EventParameters & parameters, std::string & prompt) override
  {
    const double x = std::any_cast<double>(parameters.get_value("x", 0.0));
    const double y = std::any_cast<double>(parameters.get_value("y", 0.0));
    const double z = std::any_cast<double>(parameters.get_value("z", 0.0));
    const double qx = std::any_cast<double>(parameters.get_value("qx", 0.0));
    const double qy = std::any_cast<double>(parameters.get_value("qy", 0.0));
    const double qz = std::any_cast<double>(parameters.get_value("qz", 0.0));
    const double qw = std::any_cast<double>(parameters.get_value("qw", 1.0));
    const double roll = std::any_cast<double>(parameters.get_value("roll", 0.0));
    const double pitch = std::any_cast<double>(parameters.get_value("pitch", 0.0));
    const double yaw = std::any_cast<double>(parameters.get_value("yaw", 0.0));

    prompt =
      "The current manipulator Cartesian pose is x: " + std::to_string(x) + ", y: " +
      std::to_string(y) + ", z: " + std::to_string(z) + ", qx: " + std::to_string(qx) +
      ", qy: " + std::to_string(qy) + ", qz: " + std::to_string(qz) + ", qw: " +
      std::to_string(qw) + ". The equivalent roll, pitch, yaw orientation in radians is roll: " +
      std::to_string(roll) + ", pitch: " + std::to_string(pitch) + ", yaw: " +
      std::to_string(yaw) + ".";

    RCLCPP_INFO(node_->get_logger(), "Prompt generated: %s", prompt.c_str());
  }
};
}  // namespace capabilities2_runner