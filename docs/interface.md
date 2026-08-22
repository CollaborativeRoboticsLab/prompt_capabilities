# Interface Information

Following are the details of the interfaces provided by the Prompt Capabilities package. A runner that intends to connect with one of these interfaces should support the input and ouput parameters described below.

The state-based prompt runners consume runtime input from upstream capabilities. In a concrete plan, place the matching state runner immediately before the prompt runner so the required values are available in sequence. If the final goal is fabric plan generation rather than prompt-only context capture, keep that example under `fabric_capabilities/plans`.

## PromptCapabilityRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| CapabilitySpecs   | string[] | Capability specifications collected from upstream capabilities. |
| uuid              | string | The uuid used to keep track of the prompts and their responses. |
| flush             | bool   | Whether to flush the accumulated information for the current prompt. |

Output Parameters:

None

## PromptPlanRunner

Input Parameters:

| Parameter Name   | Type   | Description |
| ---              | ---    | ---         |
| replan           | bool   | Whether the iteration is a replanning step |
| task             | string | Task Description for the current iteration |
| failedElements  | string | A list of failed elements from the previous iteration, if any. |
| uuid              | string | The uuid used to keep track of the prompts and their responses. |
| flush             | bool   | Whether to flush the accumulated information for the current prompt. |

Output Parameters:

| Parameter Name   | Type   | Description |
| ---              | ---    | ---         |
| plan             | string | The generated plan for the current iteration. |

## PromptSpeechRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| task              | string | The task or context to send to the LLM for generating a speakable response. |
| uuid              | string | The uuid used to keep track of the prompts and their responses. |
| flush             | bool   | Whether to flush the accumulated information for the current prompt before generating the speech response. |

Output Parameters:

None

## PromptPoseRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| uuid              | string | The uuid to associate the prompt with. This is used to keep track of the prompts and their responses. |
| flush             | bool   | Whether to flush the accumulated information for the current prompt. |
| x                 | double | The x-coordinate of the robot's pose. |
| y                 | double | The y-coordinate of the robot's pose. |
| z                 | double | The z-coordinate of the robot's pose. |
| qx                | double | The x-coordinate of the robot's orientation quaternion. |
| qy                | double | The y-coordinate of the robot's orientation quaternion. |
| qz                | double | The z-coordinate of the robot's orientation quaternion. |
| qw                | double | The w-coordinate of the robot's orientation quaternion. |

Output Parameters:

None

## PromptCurrentCartesianPoseRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| uuid              | string | Optional prompt-cache uuid used to append the Cartesian pose into an existing prompt context. |
| flush             | bool   | Whether to flush the accumulated prompt cache after this pose is appended. |

Runtime Input From Upstream Capability:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| x                 | double | Current Cartesian x position. |
| y                 | double | Current Cartesian y position. |
| z                 | double | Current Cartesian z position. |
| qx                | double | Quaternion x component. |
| qy                | double | Quaternion y component. |
| qz                | double | Quaternion z component. |
| qw                | double | Quaternion w component. |
| roll              | double | Roll angle in radians. |
| pitch             | double | Pitch angle in radians. |
| yaw               | double | Yaw angle in radians. |

Output Parameters:

None

## PromptCurrentJointPoseRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| uuid              | string | Optional prompt-cache uuid used to append the joint pose into an existing prompt context. |
| flush             | bool   | Whether to flush the accumulated prompt cache after this joint pose is appended. |

Runtime Input From Upstream Capability:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| joint_names       | string[] | Ordered manipulator joint names. |
| joint_positions   | double[] | Ordered manipulator joint positions matching `joint_names`. |
| joint_count       | int    | Number of joints included in the prompt. |

Output Parameters:

None

## PromptGripperStateRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| uuid              | string | Optional prompt-cache uuid used to append the gripper state into an existing prompt context. |
| flush             | bool   | Whether to flush the accumulated prompt cache after this gripper state is appended. |

Runtime Input From Upstream Capability:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| joint_names       | string[] | Ordered gripper joint names. |
| joint_positions   | double[] | Ordered gripper joint positions matching `joint_names`. |
| joint_count       | int    | Number of gripper joints included in the prompt. |

Output Parameters:

None

## PromptTextRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| uuid              | string | The uuid to associate the prompt with. This is used to keep track of the prompts and their responses. |
| flush             | bool   | Whether to flush the accumulated information for the current prompt. |
| text              | string | The text to be processed by the LLM. |

Output Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| response          | string | The response generated by the LLM for the given text. |