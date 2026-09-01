# prompt_capabilities

Provides capabilities2 runners that forward robot state and free-form task text into the Prompt Tools stack. This package owns prompt-only examples whose primary outcome is appending context or requesting an LLM response, not generating an executable fabric plan.

## Supported Runners

| Runner | Description |
| --- | --- |
| `PromptTextRunner` | Sends free-form text to prompt_tools. |
| `PromptPoseRunner` | Sends explicit pose fields to prompt_tools. |
| `PromptCurrentCartesianPoseRunner` | Reads the current manipulator Cartesian pose through `moveit2_capabilities/CurrentCartesianPoseRunner` and prompts it to the LLM. |
| `PromptCurrentJointPoseRunner` | Reads the current manipulator joint pose through `moveit2_capabilities/CurrentJointPoseRunner` and prompts it to the LLM. |
| `PromptNamedPosesRunner` | Prompts the configured manipulator named poses and their descriptions for downstream reasoning. |
| `PromptGripperStateRunner` | Reads the current gripper joint state through `gripper_capabilities/GripperStateRunner` and prompts it to the LLM. |
| `PromptDetectionRunner` | Prompts over upstream YOLO detections and emits one selected detection for downstream bbox-guided grasp filtering. |
| `PromptGraspSelectorRunner` | Prompts over upstream AnyGrasp candidates and emits one selected grasp index while preserving the candidate arrays. |
| `PromptSpeechRunner` | Prompts the LLM to generate speech text for downstream synthesis. |

For interface details, see [docs/interface.md](./docs/interface.md).

## Example Plans

- [plans/prompt_1.xml](./plans/prompt_1.xml): prompt a supplied pose to the LLM.
- [plans/prompt_2.xml](./plans/prompt_2.xml): get the current gripper joint state and prompt it to the LLM.
- [plans/prompt_3.xml](./plans/prompt_3.xml): get the current manipulator Cartesian pose and prompt it to the LLM.
- [plans/prompt_4.xml](./plans/prompt_4.xml): get the current manipulator joint pose and prompt it to the LLM.
- [plans/prompt_5.xml](./plans/prompt_5.xml): get the configured manipulator named poses and prompt them to the LLM.

## Build

```bash
cd ~/colcon_ws
colcon build --packages-up-to prompt_capabilities --symlink-install
```

## Runtime

Start the capabilities server and the prompt bridge, then run one of the prompt-only plans through your normal capabilities2 launch flow.