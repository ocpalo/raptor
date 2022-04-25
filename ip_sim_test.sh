#!/bin/bash

### I wrote this script to test image processing part of the project.It does not run Java-Go processes. 
### To make use of this script, do not forget to modify image processing mqtt flags such as `process_image` variable in process_image.py file.

# cleanup. Kill running mosquitto instance and docker instances
echo '->Killing mosquitto instance'
sudo pkill mosquitto
echo '->Docker system prune and docker kill'
echo y | sudo docker system prune
sudo docker kill fuavserver
sudo docker kill h2db-fuav

working_directory=$(pwd)

readonly ws_autonomous="$working_directory/cppAutonomous"
readonly ws_server="$working_directory/serverapi"
readonly ws_client="$working_directory/goClient"
readonly ws_imageproc="$working_directory/imageProcessing"

## This script assumes px4 autopilot installed at $HOME directory, update this if necessary
readonly px4_autopilot="$HOME/PX4-Autopilot/Tools"

echo
echo "->C++ Autonomous path: $ws_autonomous"
echo "->Java Server path: $ws_server"
echo "->Go Client path: $ws_client"
echo "->Python Image Processing path: $ws_imageproc"
echo -e "->PX4 Multiple Vehicle Simulation script path: $px4_autopilot\n"

# To check if the user executes the script in user mode. If in root mode, $HOME variable is equal to '/root' hence the script should fail.
#[[ ! -d $ws ]] && { echo -e "\n->$HOME/Desktop directory does not exists.\n->Do not run this script in root user"; exit 1; }

# Clean and compile C++
echo -e "\n->Cleaning the building directory: $ws_autonomous/build"
[[ -d $ws_autonomous/build ]] && sudo rm -rf $ws_autonomous/build
echo -e "->Creating directory 'build'"
mkdir $ws_autonomous/build
echo -e "->Running cmake and make commands\n"
cd $ws_autonomous/build && cmake ..
make

[[ $? -ne 0 ]] && { echo -e "\n->Building C++ failed.\n->Terminating the script execution\n"; exit 1; } 

echo "->Compiled C++"

# Running gazebo simulation enviroment for multiple vehicles"
echo -e "->Running gazebo...\n"
gnome-terminal --tab --title="Gazebo" -- $px4_autopilot/gazebo_sitl_multiple_run.sh -s "typhoon_h480:1,iris:1"
sleep 3

echo -e "->Running mosquitto\n"
gnome-terminal --tab --title="Mosquitto" -- mosquitto

#echo -e "->Running mosquitto_sub\n"
#gnome-terminal --tab --title="Mosquitto Sub" -- mosquitto_sub -h localhost -v -t '#'

gnome-terminal --tab --title="->Drone Target 1" --working-directory="$ws_autonomous/build" -- bash -c "echo \"Run './main udp://:14540\"; exec bash"
gnome-terminal --tab --title="->Drone Target 1" --working-directory="$ws_autonomous/build" -- bash -c "echo \"Control the drone with wasd, Run './main udp://:14541 4 1\"; exec bash"

echo -e "->Creating terminal for image processing code, run python3 main.py in last terminal\n"
gnome-terminal --tab --title="->Image Proccessing" --working-directory="$ws_imageproc" -- bash -c "echo \"Run 'python3 main.py' here\"; exec bash"
