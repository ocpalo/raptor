#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "mqtt.h"
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
  drone::raptor raptor(argv[1]);
  raptor.arm();
  raptor.takeoff();
  raptor.offboard_init();
  auto fut = std::async(std::launch::async, &drone::raptor::publish_telemetry,
                        &raptor);
  raptor.move_m(10, {.forward = 3});
  raptor.move_m(10, {.right = 3});
  raptor.stopPublish();
  return 0;
}