#include <iostream>
#include <string>

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
  climqtt.disconnect();
  return 0;
}