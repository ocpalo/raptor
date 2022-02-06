#include "raptor.h"

#include <chrono>
#include <sstream>
#include <string>
#include <thread>

#include "base_drone.h"
#include "util.h"

namespace drone {

raptor::raptor(std::string const& conn_url)
    : base_drone(conn_url), _climqtt(drone::SERVER_ADDRESS, drone::CLIENT_ID) {
  _gimbal.reset(new mavsdk::Gimbal{_mavsdk_system});

  mavsdk::Gimbal::Result gimbal_result =
      _gimbal->take_control(mavsdk::Gimbal::ControlMode::Primary);
  if (gimbal_result != mavsdk::Gimbal::Result::Success) {
    std::cerr << "Could not take gimbal control: " << gimbal_result << '\n';
  } else {
    _gimbal->set_pitch_and_yaw(-30.0f, 0.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }

  _climqtt.connect();
}

void raptor::move2() {
  _climqtt.subscribe(drone::mqtt::topics::TELEMETRY_RESPONSE_TOPIC);
  int targetCount = 2;
  while (true) {
    auto opt_msg = _climqtt.consume();
    if (opt_msg.has_value()) {
      auto msg = opt_msg.value();
      auto out = util::split(msg.second, ',');

      double dest_lat = std::stod(out[5]);
      double dest_lon = std::stod(out[6]);

      if (std::stoi(out[4]) == id_) continue;

      auto dest_heading = util::bearing(position_.lat_deg_, position_.lon_deg_,
                                        dest_lat, dest_lon);

      if (util::haversine(position_.lat_deg_, position_.lon_deg_, dest_lat,
                          dest_lon) < 3) {
        move({.forward = 0,
              .down = (position_.rel_alt_ - std::stof(out[7]) - 2),
              .yaw = static_cast<float>(dest_heading - _heading)});
        //_climqtt.publish(mqtt::topics::LOCK, out[4]);
        // targetCount--;
      } else {
        move({.forward = 3,
              .down = (position_.rel_alt_ - std::stof(out[7]) - 2),
              .yaw = static_cast<float>(dest_heading - _heading)});
        using namespace std::chrono_literals;
      }
    } else {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(50ms);
    }

    if (!targetCount) break;
  }
}  // namespace drone

void raptor::publish_telemetry() {
  while (_publish_telemetry) {
    _climqtt.publish(drone::mqtt::topics::TELEMETRY_TOPIC,
                     build_telemetry_message());
    std::this_thread::sleep_for(std::chrono::milliseconds(90));
  }
}

void raptor::stop_publish_telemetry() { _publish_telemetry = false; }

mavsdk::Action::Result raptor::do_land() {
  _climqtt.publish(drone::mqtt::topics::LAND_TOPIC,
                   std::move(std::to_string(id_)));
  auto resp = base_drone::do_land();
  return resp;
}

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