#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "mqtt.h"
#include "raptor.h"
#include "target_drone.h"

void usage(const std::string& bin_name) {
  std::cerr
      << "Usage : " << bin_name << " <connection_url> <1 || 2 || 3>\n"
      << "Connection URL format should be :\n"
      << " For TCP : tcp://[server_host][:server_port]\n"
      << " For UDP : udp://[bind_host][:bind_port]\n"
      << " For Serial : serial:///path/to/serial/dev[:baudrate]\n"
      << "For example, to connect to the simulator use URL: udp://:14540\n";
}

int main(int argc, char** argv) {
  if (!(argc != 4 || argc != 2)) {
    usage(argv[0]);
    return 1;
  }

  if (argc == 4) {
    drone::target_drone b(argv[1], std::stoi(argv[2]), std::stoi(argv[2]));
    try {
      b.arm();
      b.takeoff(5);
      b.offboard_init();
      b.run(argv[2]);
    } catch (std::runtime_error& ex) {
      debug_print("Land mode actived, runtime_error:");
      debug_print(ex.what());
      b.land();
    }
  } else {
    drone::raptor raptor(argv[1]);
    raptor.arm();
    raptor.takeoff();
    raptor.offboard_init();
    auto fut = std::async(std::launch::async, &drone::raptor::publish_telemetry,
                          &raptor);
    raptor.move_m(10, {.forward = 3});
    raptor.stop_publish_telemetry();
    raptor.land();
  }
  return 0;
}