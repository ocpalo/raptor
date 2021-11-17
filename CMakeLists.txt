cmake_minimum_required(VERSION 3.15)

# Specify your project's name
project(main)

# Specify at least C++11, better C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Remove -Werror flag during release and _PRINT_IN_DEBUG_ macro inside debug.h
# Enable strict handling of warnings
add_definitions("-Wall -Wextra -Werror")

# Finds MAVSDK when installed system wide.
find_package(MAVSDK REQUIRED)

# Specify your app's executable name, and list of source files used to create it.
add_executable(main
    src/main.cc
    src/base_drone.cc
    src/raptor.cc
    src/util.cc
    # ... any other source files
)

target_include_directories(main PRIVATE "header/")

# Specify your app's executable name and a list of linked libraries
target_link_libraries(main
    MAVSDK::mavsdk             #All apps link against mavsdk library
    MAVSDK::mavsdk_action      # If action plugin used by app ...
    MAVSDK::mavsdk_telemetry   #If telemetry plugin used by app ...
    MAVSDK::mavsdk_offboard
    # ... Any other linked libraries
)
