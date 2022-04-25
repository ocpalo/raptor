#!/bin/bash

# cleanup. Kill running mosquitto instance and docker instances
echo '->Killing mosquitto instance'
sudo pkill mosquitto
echo '->Docker system prune and docker kill'
echo y | sudo docker system prune
sudo docker kill fuavserver
sudo docker kill h2db-fuav

readonly ws="$HOME/Desktop/ws"
readonly ws_raptor="$ws/raptor"
readonly ws_server="$ws/Fighter_UAV_Server_API"
readonly ws_client="$ws/goUavClient"
readonly px4_autopilot="$HOME/PX4-Autopilot/Tools"

echo
echo "->Workspace path: $ws"
echo "->C++ Raptor path: $ws_raptor"
echo "->Java Server path: $ws_server"
echo "->Go Client path: $ws_client"
echo -e "->PX4 Multiple Vehicle Simulation script path: $px4_autopilot\n"

# To check if the user executes the script in user mode. If in root mode, $HOME variable is equal to '/root' hence the script should fail.
[[ ! -d $ws ]] && { echo -e "\n->$HOME/Desktop directory does not exists.\n->Do not run this script in root user"; exit 1; }

# Clean and compile C++
echo -e "\n->Cleaning the building directory: $ws_raptor/build"
[[ -d $ws_raptor/build ]] && sudo rm -rf $ws_raptor/build
echo -e "->Creating directory 'build'"
mkdir $ws_raptor/build
echo -e "->Running cmake and make commands\n"
cd $ws_raptor/build && cmake ..
make

[[ $? -ne 0 ]] && { echo -e "\n->Building C++ failed.\n->Terminating the script execution\n"; exit 1; } 

echo "->Compiled C++"

# Running gazebo simulation enviroment for multiple vehicles"
echo -e "->Running gazebo...\n"
gnome-terminal --tab --title="Gazebo" -- $px4_autopilot/gazebo_sitl_multiple_run.sh -s "typhoon_h480:1,iris:1"
sleep 3

echo -e "->Running Java server...\n"
gnome-terminal --tab --title="Java Server" --working-directory="$ws_server" -- sudo docker-compose up --build
sleep 3s

echo "->Running go client..."
echo -e "->Remember to re-launch go client after every flight!\n"
gnome-terminal --tab --title="Go Client" --working-directory="$ws_client" -- bash -c "go run main.go; exec bash"

echo -e "->Running mosquitto\n"
gnome-terminal --tab --title="Mosquitto" -- mosquitto

echo -e "->Running mosquitto_sub\n"
gnome-terminal --tab --title="Mosquitto Sub" -- mosquitto_sub -h localhost -v -t '#'

echo -e "->Sleeping 10 second for waiting gazebo to set up properly\n"
sleep 10s
echo -e "->Creating terminals for drones\n"
gnome-terminal --tab --title="Raptor" --working-directory="$ws_raptor/build"
gnome-terminal --tab --title="->Drone Target 1" --working-directory="$ws_raptor/build" -- bash -c "echo \"Control the drone with wasd\";./main udp://:14541 4 1; exec bash"

echo -e "->Creating terminal for image processing code, run python3 main.py in last terminal\n"
gnome-terminal --tab --title="->Image Proccessing" --working-directory="$ws_raptor/scripts" -- bash -c "python3 main.py; exec bash"
