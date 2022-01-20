cmake_minimum_required(VERSION 3.15)

# Specify your project's name
project(main)

# Specify at least C++11, better C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -fno-omit-frame-pointer -fsanitize=address,undefined -ggdb3")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -Wall -Wextra -Wno-error=deprecated-declarations  -fno-omit-frame-pointer -fsanitize=address,undefined -ggdb3")

# Remove -Werror _PRINT_IN_DEBUG_ macro during release
# Enable strict handling of warnings
add_definitions("-D_PRINT_IN_DEBUG_")

# Finds MAVSDK when installed system wide.
find_package(MAVSDK REQUIRED)
find_package(PahoMqttCpp REQUIRED)

# Specify your app's executable name, and list of source files used to create it.
add_executable(main
    src/main.cc
    src/base_drone.cc
    src/raptor.cc
    src/util.cc
    src/mqtt.cc
    # ... any other source files
)

target_include_directories(main PRIVATE "header/")

# Specify your app's executable name and a list of linked libraries
target_link_libraries(main
    MAVSDK::mavsdk             #All apps link against mavsdk library
    MAVSDK::mavsdk_action      # If action plugin used by app ...
    MAVSDK::mavsdk_telemetry   #If telemetry plugin used by app ...
    MAVSDK::mavsdk_offboard
    MAVSDK::mavsdk_param
    paho-mqttpp3 
    paho-mqtt3as
    # ... Any other linked libraries
)
