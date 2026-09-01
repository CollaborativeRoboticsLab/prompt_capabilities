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

## PromptNamedPosesRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| uuid              | string | Optional prompt-cache uuid used to append the named-pose catalog into an existing prompt context. |
| flush             | bool   | Whether to flush the accumulated prompt cache after this named-pose catalog is appended. |

Runtime Input From Upstream Capability:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| pose_names        | string[] | Ordered configured named-pose identifiers. |
| descriptions      | string[] | Ordered descriptions matching `pose_names`. |
| pose_count        | int    | Number of configured named poses included in the prompt. |

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
| uuid              | string | Prompt uuid returned by prompt_tools. |
| buffered          | bool   | Whether prompt_tools buffered the request instead of returning a final answer. |
| success           | bool   | Whether prompt_tools reported success for the request. |

## PromptDetectionRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| target_object     | string | Optional object description used by the LLM to choose among detections. |
| uuid              | string | Optional prompt-cache uuid. |
| flush             | bool   | Whether to flush prompt cache after this request. |

Runtime Input From Upstream Capability:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| frame_id          | string | Detection frame id. |
| image_width       | int    | Image width from the upstream YOLO detection result. |
| image_height      | int    | Image height from the upstream YOLO detection result. |
| detection_ids     | int[]  | Detection identifiers aligned with the detection arrays. |
| class_id          | int[]  | Detected class ids aligned with `detection_ids`. |
| class_name        | string[] | Detected class names aligned with `detection_ids`. |
| confidence        | double[] | Detection confidences aligned with `detection_ids`. |
| bbx_center_x      | int[]  | Bounding-box center x coordinates aligned with `detection_ids`. |
| bbx_center_y      | int[]  | Bounding-box center y coordinates aligned with `detection_ids`. |
| bbx_size_w        | int[]  | Bounding-box widths aligned with `detection_ids`. |
| bbx_size_h        | int[]  | Bounding-box heights aligned with `detection_ids`. |

Output Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| selected_detection_index | int | Zero-based detection index selected by the LLM. |
| detection_id      | int    | Selected detection id for downstream filtered-grasps requests. |
| class_id          | int    | Selected detection class id. |
| class_name        | string | Selected detection class name. |
| bbx_center_x      | int    | Selected bounding-box center x. |
| bbx_center_y      | int    | Selected bounding-box center y. |
| bbx_size_w        | int    | Selected bounding-box width. |
| bbx_size_h        | int    | Selected bounding-box height. |
| image_width       | int    | Forwarded image width. |
| image_height      | int    | Forwarded image height. |
| selection_reason  | string | Short explanation returned by the LLM. |
| response          | string | Raw prompt response text. |
| uuid              | string | Prompt uuid returned by prompt_tools. |
| message           | string | Failure message when prompt-based detection selection fails. |

## PromptGraspSelectorRunner

Input Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| task_hint         | string | Optional task hint used by the LLM to choose among grasp candidates. |
| uuid              | string | Optional prompt-cache uuid. |
| flush             | bool   | Whether to flush prompt cache after this request. |

Runtime Input From Upstream Capability:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| frame_ids         | string[] | Frame ids for the grasp candidates. |
| positions_x       | double[] | X positions for the grasp candidates. |
| positions_y       | double[] | Y positions for the grasp candidates. |
| positions_z       | double[] | Z positions for the grasp candidates. |
| orientations_x    | double[] | Quaternion x values for the grasp candidates. |
| orientations_y    | double[] | Quaternion y values for the grasp candidates. |
| orientations_z    | double[] | Quaternion z values for the grasp candidates. |
| orientations_w    | double[] | Quaternion w values for the grasp candidates. |

Output Parameters:

| Parameter Name    | Type   | Description |
| ---               | ---    | ---         |
| selected_grasp_index | int | Zero-based grasp index selected by the LLM. |
| selection_reason  | string | Short explanation returned by the LLM. |
| frame_ids         | string[] | Pass-through frame ids for downstream pose selection. |
| positions_x       | double[] | Pass-through X positions for downstream pose selection. |
| positions_y       | double[] | Pass-through Y positions for downstream pose selection. |
| positions_z       | double[] | Pass-through Z positions for downstream pose selection. |
| orientations_x    | double[] | Pass-through quaternion x values for downstream pose selection. |
| orientations_y    | double[] | Pass-through quaternion y values for downstream pose selection. |
| orientations_z    | double[] | Pass-through quaternion z values for downstream pose selection. |
| orientations_w    | double[] | Pass-through quaternion w values for downstream pose selection. |
| response          | string | Raw prompt response text. |
| uuid              | string | Prompt uuid returned by prompt_tools. |
| message           | string | Failure message when prompt-based grasp selection fails. |