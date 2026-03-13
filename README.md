# Capabilities2_runner_prompt

Provides capabiilites2 runners for PromptTools stack

## Supported Runners

| Runner | Description  |
| ---    | ---          |
| Prompt Capability runner | prompts LLM about the capabilities available on the robot |
| Prompt Occupancy runner | prompts LLM about the occupancy grid of the robot |
| Prompt Plan runner | prompts LLM requesting a new execution plan for a given task |
| Prompt Pose runner | prompts LLM about the pose of the robot |
| Prompt Text runner | prompts LLM about audio response to the robot |

## Examples

Examples depend on [CollaborativeRoboticsLab/capabilities2](https://github.com/CollaborativeRoboticsLab/capabilities2) and [CollaborativeRoboticsLab/prompt_tools](https://github.com/CollaborativeRoboticsLab/prompt_tools). Following examples have been tested against turtlebot3 simulation using [CollaborativeRoboticsLab/turtlebot3-docker](https://github.com/CollaborativeRoboticsLab/turtlebot3-docker)

| Example | Description |
| ---     | ---         |
| [prompt_1.xml](./plans/prompt_1.xml) | Implements requesting for robot's capabilities and prompting them to the LLM |
| [prompt_2.xml](./plans/prompt_2.xml) | Implements listening for robot's pose and prompting them to the LLM 
| [prompt_3.xml](./plans/prompt_3.xml) | Implements prompting the LLM for a plan for a new task and setting it to Fabric. This example also utilize an externally provided UUID to keep the Prompt Tools side cache consistent accross different runners. |


To run the examples, first make sure that the simulation is running and then on seperate terminals run,

```bash
source install/setup.bash
ros2 launch capabilities2_server capabilities2_server.launch.py
```

```bash
export OPENAI_API_KEY=
source install/setup.bash
ros2 launch prompt_bridge prompt_bridge.launch.py
```

```bash
source install/setup.bash
ros2 launch prompt_capabilities system.launch.py filename:=prompt_1.xml
```

Change `filename:=prompt_1.xml` to match the correct plan