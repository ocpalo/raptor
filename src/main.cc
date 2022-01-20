#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "mqtt.h"

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
  drone::mqtt::client_mqtt climqtt(drone::SERVER_ADDRESS, drone::CLIENT_ID);

  climqtt.connect();
  climqtt.publish(drone::mqtt::TELEMETRY_TOPIC, "hello world!");
  climqtt.subscribe("raptor/telemetry");
  int i = 0;
  while (i < 10) {
    auto resp = climqtt.consume();
    if (resp.has_value()) {
      std::cout << resp.value().first << " " << resp.value().second << "\n";
    }
    i++;
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
  }
  climqtt.disconnect();
  return 0;
}