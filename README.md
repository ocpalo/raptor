# Raptor

The following video demonstrates the project.

https://user-images.githubusercontent.com/44481097/172425839-64f00fbb-4f06-40ac-8f32-a5a5b61d337e.mp4

## System

![Untitled Diagram drawio (1)](https://user-images.githubusercontent.com/44481097/172723526-022042e2-db67-4c0b-9874-2f5ff7c47aad.png)

## Requirements

We tested this repository on Ubuntu 20.04.3 LTS. 

* CMake
* ROS Noetic(For Gazebo)
* PX4 Autopilot
* Mosquitto

***Visit processes folders for detailed requirements***

## Languages and Versions

* C++20
* Java8
* Python 3.6 and above
* Go 1.16.2 and above

## How To Run Entire Project

We use Gazebo simulation environment to test our project. We have 2 scripts to run simulation environment and project. Before running scripts, make sure every dependency is installed.

* *sim_test.sh*
    * Default script for testing the changes. It opens all necessary terminals. It runs Client,Server,Mosquitto processes automatically but you still have to run image processing and autonomous movement processes manually.
* *ip_sim_test.sh*
    * Specialized script for testing image processing changes. It creates 2 UAV's in Gazebo environment. Main UAV only performs takeoff action. You use WASD keys to control target drone. Before using this script, do not forget to modify image processing flags. Please read the comments at the beginning of the script file.

#### Setup

1. Install [CMake](https://cmake.org) (Minimum Version required 3.15 and above)
1. Install [ROS Noetic](http://wiki.ros.org/noetic/Installation/Ubuntu)
2. Clone PX4 Source Code and run bash script
   1. `git clone https://github.com/PX4/PX4-Autopilot.git --recursive`
   2. `bash ./PX4-Autopilot/Tools/setup/ubuntu.sh`
   3. `sudo apt install libignition-common3-graphics`
   4. restart the computer
4. Install [QGroundControl](https://docs.qgroundcontrol.com/master/en/releases/daily_builds.html) (optional)
5. Install [Mosquitto](https://mosquitto.org)
   1. `sudo apt install mosquitto` 

#### Additional Links

* [PX4 Installation Docs](https://docs.px4.io/v1.12/en/dev_setup/dev_env_linux_ubuntu.html)
* [PX4 Gazebo Docs](https://docs.px4.io/v1.12/en/simulation/gazebo.html)
