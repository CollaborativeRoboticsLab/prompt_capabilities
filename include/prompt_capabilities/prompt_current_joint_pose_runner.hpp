#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{
namespace prompt_detail
{
/**
 * @brief Format paired name and numeric value lists into readable text.
 *
 * @param names Ordered names.
 * @param values Ordered values.
 * @param value_suffix Optional suffix appended after each value.
 * @return Human-readable paired description.
 */
inline std::string describe_named_values(
  const std::vector<std::string> & names,
  const std::vector<double> & values,
  const std::string & value_suffix = "")
{
  if (names.empty() || names.size() != values.size())
  {
    return "invalid paired vector payload";
  }

  std::ostringstream stream;
  stream.setf(std::ios::fixed);
  stream.precision(6);
  for (size_t index = 0; index < names.size(); ++index)
  {
    if (index > 0)
    {
      stream << ", ";
    }
    stream << names[index] << "=" << values[index] << value_suffix;
  }
  return stream.str();
}
}  // namespace prompt_detail

/**
 * @brief Prompt the current manipulator joint state through prompt_tools.
 */
class PromptCurrentJointPoseRunner : public PromptServiceRunner
{
public:
  /**
   * @brief Default constructor required for pluginlib.
   */
  PromptCurrentJointPoseRunner()
  : PromptServiceRunner()
  {
  }

  /**
   * @brief Build a prompt describing the current manipulator joint state.
   *
   * @param parameters Event parameters containing joint names and positions.
   * @param prompt Output prompt string sent to prompt_tools.
   */
  void generate_prompt(capabilities2_events::EventParameters & parameters, std::string & prompt) override
  {
    const auto joint_names = std::any_cast<std::vector<std::string>>(
      parameters.get_value("joint_names", std::vector<std::string>{}));
    const auto joint_positions = std::any_cast<std::vector<double>>(
      parameters.get_value("joint_positions", std::vector<double>{}));
    const int joint_count = std::any_cast<int>(parameters.get_value("joint_count", 0));

    prompt =
      "The current manipulator joint state contains " + std::to_string(joint_count) +
      " joints. The joint positions in radians are " +
      prompt_detail::describe_named_values(joint_names, joint_positions, " rad") + ".";

    RCLCPP_INFO(node_->get_logger(), "Prompt generated: %s", prompt.c_str());
  }
};
}  // namespace capabilities2_runner