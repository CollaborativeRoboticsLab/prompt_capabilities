#pragma once

#include <sstream>
#include <string>
#include <vector>

#include <prompt_capabilities/prompt_service_runner.hpp>

namespace capabilities2_runner
{
namespace prompt_detail
{
inline std::string describe_named_poses(
  const std::vector<std::string> & pose_names,
  const std::vector<std::string> & descriptions)
{
  if (pose_names.empty() || pose_names.size() != descriptions.size())
  {
    return "invalid named pose payload";
  }

  std::ostringstream stream;
  for (size_t index = 0; index < pose_names.size(); ++index)
  {
    if (index > 0)
    {
      stream << "; ";
    }
    stream << pose_names[index] << ": " << descriptions[index];
  }
  return stream.str();
}
}  // namespace prompt_detail

class PromptNamedPosesRunner : public PromptServiceRunner
{
public:
  PromptNamedPosesRunner()
  : PromptServiceRunner()
  {
  }

  void generate_prompt(capabilities2_events::EventParameters & parameters, std::string & prompt) override
  {
    const auto pose_names = std::any_cast<std::vector<std::string>>(
      parameters.get_value("pose_names", std::vector<std::string>{}));
    const auto descriptions = std::any_cast<std::vector<std::string>>(
      parameters.get_value("descriptions", std::vector<std::string>{}));
    const int pose_count = std::any_cast<int>(parameters.get_value("pose_count", 0));

    prompt =
      "The manipulator currently has " + std::to_string(pose_count) +
      " configured named poses. The pose catalog is: " +
      prompt_detail::describe_named_poses(pose_names, descriptions) + ".";

    RCLCPP_INFO(node_->get_logger(), "Prompt generated: %s", prompt.c_str());
  }
};
}  // namespace capabilities2_runner