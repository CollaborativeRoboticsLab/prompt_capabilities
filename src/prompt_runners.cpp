#include <pluginlib/class_list_macros.hpp>
#include <capabilities2_runner/runner_base.hpp>
#include <prompt_capabilities/prompt_current_cartesian_pose_runner.hpp>
#include <prompt_capabilities/prompt_current_joint_pose_runner.hpp>
#include <prompt_capabilities/prompt_detection_runner.hpp>
#include <prompt_capabilities/prompt_gripper_state_runner.hpp>
#include <prompt_capabilities/prompt_grasp_selector_runner.hpp>
#include <prompt_capabilities/prompt_named_poses_runner.hpp>
#include <prompt_capabilities/prompt_text_runner.hpp>
#include <prompt_capabilities/prompt_speech_runner.hpp>
#include <prompt_capabilities/prompt_pose_runner.hpp>

// register runner plugins
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptTextRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptSpeechRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptPoseRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptCurrentCartesianPoseRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptCurrentJointPoseRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptNamedPosesRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptGripperStateRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptDetectionRunner, capabilities2_runner::RunnerBase)
PLUGINLIB_EXPORT_CLASS(capabilities2_runner::PromptGraspSelectorRunner, capabilities2_runner::RunnerBase)