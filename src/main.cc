#include <iostream>
#include <string>

#include "base_drone.h"
#include "raptor.h"

void usage(const std::string &bin_name) {
  std::cerr
      << "Usage : " << bin_name << " <connection_url>\n"
      << "Connection URL format should be :\n"
      << " For TCP : tcp://[server_host][:server_port]\n"
      << " For UDP : udp://[bind_host][:bind_port]\n"
      << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
      << "For example, to connect to the simulator use URL: udp://:14540\n";
}

int main(int argc, char **argv) {
  if (argc != 2) {
    usage(argv[0]);
    return 1;
  }

  drone::raptor b(argv[1]);
  try {
    b.arm();
    b.takeoff(5);
    b.offboard_init();
    b.move_m(20, {.right = -2});
  } catch (std::runtime_error &ex) {
    debug_print("Land mode actived, runtime_error:");
    debug_print(ex.what());
    b.land();
  }

  debug_print("We did it");

  return 0;
}