# Capabilities2_runner_prompt

Provides capabiilites2 runners for PromptTools stack. These are required by the LLM to gather information about the robot or for it to generate a plan for a new task.

## Supported Runners

| Runner                    | Focus  | Description  |
| ---                       | ---   | ---          |
| Prompt Capability runner  | Plan Generation | prompts LLM about the capabilities available on the robot |
| Prompt Plan runner        | Plan Generation  | prompts LLM requesting a new execution plan for a given task |
| Prompt Speech runner      | Information Gathering | prompts the LLM to generate speech text for downstream synthesis |
| Prompt Pose runner        | Information Gathering | prompts LLM about the pose of the robot |
| Prompt Text runner        | Information Gathering | forwards text context to the LLM through the prompt service |

For information about these runners' interfaces, please refer to [Interface Information](./docs/interface.md) section.

## Examples

Examples depend on [CollaborativeRoboticsLab/capabilities2](https://github.com/CollaborativeRoboticsLab/capabilities2) and [CollaborativeRoboticsLab/prompt_tools](https://github.com/CollaborativeRoboticsLab/prompt_tools). Following examples have been tested against turtlebot3 simulation using [CollaborativeRoboticsLab/turtlebot3-docker](https://github.com/CollaborativeRoboticsLab/turtlebot3-docker)

| Example | Description |
| ---     | ---         |
| [prompt_1.xml](./plans/prompt_1.xml) | Implements listening for robot's pose and prompting them to the LLM |  
| [prompt_2.xml](./plans/prompt_2.xml) | Implements prompting the LLM for a plan for a new task and setting it to Fabric. This example also utilize an externally provided UUID to keep the Prompt Tools side cache consistent accross different runners. |

## Setup

Above examples depend on the capabilities2 and prompt_tools packages. You can clone these packages in your workspace and build them using colcon build.

```bash
cd ~/colcon_ws/src
git clone https://github.com/CollaborativeRoboticsLab/capabilities2.git
git clone https://github.com/CollaborativeRoboticsLab/fabric.git
git clone https://github.com/CollaborativeRoboticsLab/prompt_tools.git
git clone https://github.com/CollaborativeRoboticsLab/prompt_capabilities.git

cd ~/colcon_ws
colcon build --symlink-install
```

To setup the simulation, you can use the turtlebot3 world in Gazebo. You can launch the simulation using the following command,

```bash
git clone https://github.com/CollaborativeRoboticsLab/turtlebot3-docker.git
cd turtlebot3-docker/docker
docker compose pull
xhost +local:root
docker compose up
```

## Running the examples

To run the examples, first make sure that the simulation is running and then on seperate terminals run,

```bash
source install/setup.bash
ros2 launch capabilities2_server capabilities2_server.launch.py
```

Replace <your_openai_api_key> with your actual OpenAI API key in the command below.

```bash
export OPENAI_API_KEY=<your_openai_api_key>
source install/setup.bash
ros2 launch prompt_bridge prompt_bridge.launch.py
```

```bash
source install/setup.bash
ros2 launch prompt_capabilities system.launch.py filename:=prompt_1.xml
```

Change `filename:=prompt_1.xml` to match the correct plan