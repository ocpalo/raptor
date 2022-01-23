#include "raptor.h"

#include <chrono>
#include <sstream>
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

void raptor::land() {
  base_drone::land();
  _climqtt.publish(drone::mqtt::topics::LAND_TOPIC,
                   std::move(std::to_string(id_)));
}

void raptor::publish_telemetry() {
  while (_publish_telemetry) {
    _climqtt.publish(drone::mqtt::topics::TELEMETRY_TOPIC,
                     build_telemetry_message());
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
  }
}

void raptor::stop_publish_telemetry() { _publish_telemetry = false; }

std::string raptor::build_telemetry_message() {
  std::stringstream str;
  str << id_ << " " << position_.lat_deg_ << " " << position_.lon_deg_ << " "
      << position_.rel_alt_ << " " << attitude_.roll_deg_ << " "
      << attitude_.pitch_deg_ << " " << attitude_.yaw_deg_ << " " << speed_m_s_
      << " " << battery_remaning_percent_ << " " << 1 << " " << 0 << " " << 0
      << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0
      << " " << 0;
  return str.str();
}

}  // namespace drone