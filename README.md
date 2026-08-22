# prompt_capabilities

Provides capabilities2 runners that forward robot state and free-form task text into the Prompt Tools stack. These runners are used either to append context to the LLM cache or to pair that context with downstream plan generation.

## Supported Runners

| Runner | Description |
| --- | --- |
| `PromptTextRunner` | Sends free-form text to prompt_tools. |
| `PromptPoseRunner` | Sends explicit pose fields to prompt_tools. |
| `PromptCurrentCartesianPoseRunner` | Reads the current manipulator Cartesian pose through `moveit2_capabilities/CurrentCartesianPoseRunner` and prompts it to the LLM. |
| `PromptCurrentJointPoseRunner` | Reads the current manipulator joint pose through `moveit2_capabilities/CurrentJointPoseRunner` and prompts it to the LLM. |
| `PromptGripperStateRunner` | Reads the current gripper joint state through `gripper_capabilities/GripperStateRunner` and prompts it to the LLM. |
| `PromptSpeechRunner` | Prompts the LLM to generate speech text for downstream synthesis. |

For interface details, see [docs/interface.md](./docs/interface.md).

## Example Plans

- [plans/prompt_1.xml](./plans/prompt_1.xml): prompt a supplied pose to the LLM.
- [plans/prompt_2.xml](./plans/prompt_2.xml): prompt context and generate a plan with a shared UUID.
- [plans/prompt_3.xml](./plans/prompt_3.xml): get the current gripper joint state and prompt it to the LLM.
- [plans/prompt_4.xml](./plans/prompt_4.xml): get the current gripper joint state, prompt it, then generate a plan to open the gripper.
- [plans/prompt_5.xml](./plans/prompt_5.xml): get the current manipulator Cartesian pose and prompt it to the LLM.
- [plans/prompt_6.xml](./plans/prompt_6.xml): get the current manipulator Cartesian pose, prompt it, then generate a plan to move the gripper 5 cm backward.
- [plans/prompt_7.xml](./plans/prompt_7.xml): get the current manipulator joint pose and prompt it to the LLM.
- [plans/prompt_8.xml](./plans/prompt_8.xml): get the current manipulator joint pose, prompt it, then generate a plan to rotate `wrist_1` by 45 degrees clockwise.
- [plans/prompt_9.xml](./plans/prompt_9.xml): generate a plan to move the robot to named pose `pre_grasp`.
- [plans/prompt_10.xml](./plans/prompt_10.xml): generate a plan to move through named poses `pre_grasp`, `grasp_pose`, and `post_grasp`.

Plans that combine a prompt step with `fabric_capabilities/FabricGeneratePlanRunner` use a shared `uuid` so prompt_tools can accumulate the state description before the plan-generation request is sent.

## Build

```bash
cd ~/colcon_ws
colcon build --packages-up-to prompt_capabilities --symlink-install
```

## Runtime

Start the capabilities server and the prompt bridge, then run one of the prompt plans through your normal capabilities2 launch flow.