#include "base_drone.h"

#include <string>
#include <iostream>

void usage(const std::string& bin_name)
{
    std::cerr << "Usage : " << bin_name << " <connection_url>\n"
              << "Connection URL format should be :\n"
              << " For TCP : tcp://[server_host][:server_port]\n"
              << " For UDP : udp://[bind_host][:bind_port]\n"
              << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
              << "For example, to connect to the simulator use URL: udp://:14540\n";
}

#include "base_drone.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    drone::base_drone b(argv[1]);
    try{    
        b.arm();
        b.takeoff();
        b.offboard_init();
        b.move_forward(5);
        b.move_altitude(1);
        b.move_sideways(5);
        b.move_forward(-5);
        b.move_sideways(-5);
        b.hover(10);
        b.land();
    }
    catch(std::runtime_error& ex) {
        b.land();
    }

    return 0;
}