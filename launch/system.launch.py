from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # get the plan name from launch arguments
    plan_file_name = LaunchConfiguration('filename')
    fabric_config = LaunchConfiguration('fabric_config')
    capabilities_config = LaunchConfiguration('capabilities_config')
    start_prompt_tools = LaunchConfiguration('start_prompt_tools')
    start_launch_proxy = LaunchConfiguration('start_launch_proxy')

    declare_plan_file_name = DeclareLaunchArgument(
        'filename',
        default_value='prompt_1.xml',
        description='Name of the plan file'
    )

    declare_fabric_config = DeclareLaunchArgument(
        'fabric_config',
        default_value=PathJoinSubstitution([FindPackageShare('prompt_capabilities'), 'config', 'fabric_config.yaml']),
        description='Absolute path to the fabric configuration file'
    )

    declare_capabilities_config = DeclareLaunchArgument(
        'capabilities_config',
        default_value=PathJoinSubstitution([FindPackageShare('prompt_capabilities'), 'config', 'capabilities2_config.yaml']),
        description='Absolute path to the capabilities2 configuration file'
    )

    declare_start_prompt_tools = DeclareLaunchArgument(
        'start_prompt_tools',
        default_value='true',
        description='Whether to start prompt_bridge for prompt-based plan generation; set false to disable it'
    )

    declare_start_launch_proxy = DeclareLaunchArgument(
        'start_launch_proxy',
        default_value='true',
        description='Start the capabilities2 launch proxy; disable for an external proxy'
    )

    # plan file folder
    plan_file_path = PathJoinSubstitution([FindPackageShare('prompt_capabilities'), 'plans', plan_file_name ])

    fabric_launch_path = PathJoinSubstitution([FindPackageShare('fabric_server'), 'launch', 'fabric.launch.py'])

    fabric = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(fabric_launch_path),
        launch_arguments={
            'plan_file_path': plan_file_path,
            'fabric_config': fabric_config,
            'capabilities_config': capabilities_config,
            'start_prompt_tools': start_prompt_tools,
            'start_launch_proxy': start_launch_proxy,
        }.items(),
    )
    
    return LaunchDescription([
        declare_plan_file_name,
        declare_fabric_config,
        declare_capabilities_config,
        declare_start_prompt_tools,
        declare_start_launch_proxy,
        fabric
    ])

