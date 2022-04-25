# Workstation

## System

![Untitled Diagram drawio (1)](https://user-images.githubusercontent.com/44481097/151444354-6076e238-f2ca-4cfc-bdaa-c018c288c8fa.png)

## Languages and Versions

* C++20
* Java8
* Python 3.6 and above
* Go 1.16.2 and above

## How To Run Entire Project

1. Run PX4 Simulation and Gazebo with Script
   1. `cd PX4-Autopilot/`
   2. `Tools/gazebo_sitl_multiple_run.sh -s "typhoon_h480:1,iris:3"`
      * Opens Gazebo with one Typhoon H480 and 3 iris.
      * Typhoon mavlink port is 14540, Iris's are 14541-14543
      * Typhoon gstreamer udp port is 5600
2. Run QGroundControl (Optional)
   1. ./QGroundControl.AppImage
      * In my computer its inside ~/Downloads folder. I go to that directory and run this command there
3. Run MQTT Broker (Optional)
   1. `mosquitto`
4. Run Java Server (Optional)
   1. `docker-compose up --build`
      * Run Docker container for Java Server
5. Run Go Client (Optional)
   1. `go run main.go`
      * Run Go Client to communicate with Java Server
6. Run C++ Movement Code
   1. `cd build` (if there is no folder named build, create it with `mkdir build`
   2. `cmake ..`
   3. `make`
   4. For Typhoon          : `./main udp://:14540`
   5. For Iris(mock drones): `./main udp://:1454<1|2|3> <[1-4]> <[1-3]>` optionals in order: port, mission, drone_id

#### Setup

1. Install [CMake](https://cmake.org) (Minimum Version required 3.15 and above)
1. Install [ROS Noetic](http://wiki.ros.org/noetic/Installation/Ubuntu)
2. Clone PX4 Source Code and run bash script
   1. `git clone https://github.com/PX4/PX4-Autopilot.git --recursive`
   2. `bash ./PX4-Autopilot/Tools/setup/ubuntu.sh`
   3. `sudo apt install libignition-common3-graphics`
   4. restart the computer
4. Install [QGroundControl](https://docs.qgroundcontrol.com/master/en/releases/daily_builds.html)
5. Install [Mosquitto](https://mosquitto.org)
   1. `sudo apt install mosquitto` 
6. Install or update dependencies for C++
   1. Update C++ Version
      1. `sudo apt install gcc-10 && sudo apt install g++-10`
      2. `sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10`
      3. `sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10`
      4. Test with
         1. gcc --version
         2. g++ --version
   2. Install [MAVSDK](https://mavsdk.mavlink.io/v0.44/en/cpp/)
   3. Install [Paho MQTT CPP](https://github.com/eclipse/paho.mqtt.cpp)
7. Install or update dependencies for Go
   1. Install [Paho MQTT for Go](https://github.com/eclipse/paho.mqtt.golang)

#### Additional Links

* [PX4 Installation Docs](https://docs.px4.io/v1.12/en/dev_setup/dev_env_linux_ubuntu.html)
* [PX4 Gazebo Docs](https://docs.px4.io/v1.12/en/simulation/gazebo.html)
