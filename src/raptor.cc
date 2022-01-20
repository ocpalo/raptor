#include "raptor.h"

#include <chrono>
#include <string>
#include <thread>

#include "base_drone.h"

namespace drone {

raptor::raptor(std::string const& conn_url)
    : base_drone(conn_url), _climqtt(drone::SERVER_ADDRESS, drone::CLIENT_ID) {
  _climqtt.connect();
}

bool raptor::move2(float heading) {
  if (std::abs(heading - this->_heading) > 3) {
    move_forward(3);
    set_heading(heading);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    return false;
  }
  return true;
}

void raptor::stopPublish() { _publish = false; }

void raptor::publish_telemetry() {
  while (this->_publish) {
    _climqtt.publish(drone::mqtt::TELEMETRY_TOPIC, buildTelemetryMessage());
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
  }
}

std::string raptor::buildTelemetryMessage() {
  std::string str = "";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(position_.lat_deg_);
  str += " ";
  str += std::to_string(position_.lon_deg_);
  str += " ";
  str += std::to_string(position_.rel_alt_);
  str += " ";
  str += std::to_string(attitude_.roll_deg_);
  str += " ";
  str += std::to_string(attitude_.pitch_deg_);
  str += " ";
  str += std::to_string(attitude_.yaw_deg_);
  str += " ";
  str += std::to_string(speed_m_s_);
  str += " ";
  str += std::to_string(battery_remaning_percent_);
  str += " ";
  str += std::to_string(1);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  str += " ";
  str += std::to_string(0);
  return str;
}

}  // namespace drone