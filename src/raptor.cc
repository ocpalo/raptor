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
  _climqtt.connect();
}

void raptor::move2() {
  _climqtt.subscribe(drone::mqtt::topics::TELEMETRY_RESPONSE_TOPIC);
  int targetCount = 2;
  while (true) {
    auto opt_msg = _climqtt.consume();
    if (opt_msg.has_value()) {
      move({.forward = 3});
      auto msg = opt_msg.value();

      auto out = util::split(msg.second, ',');

      std::cout << "TeamId, Dest Lat-Lon: " << std::stoi(out[4]) << " "
                << std::stod(out[5]) << " " << std::stod(out[6]) << "\n";
      // dest lat-long
      double dest_lat = std::stod(out[5]);
      double dest_lon = std::stod(out[6]);

      if (std::stoi(out[4]) == id_) continue;

      std::cout << "Bearing :"
                << util::bearing(position_.lat_deg_, position_.lon_deg_,
                                 dest_lat, dest_lon)
                << "\n";
      auto dest_heading = util::bearing(position_.lat_deg_, position_.lon_deg_,
                                        dest_lat, dest_lon);

      if (std::abs(dest_heading - this->_heading) > 1) {
        set_heading(dest_heading);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(50ms);
      }

      std::cout << "Haversine: "
                << util::haversine(position_.lat_deg_, position_.lon_deg_,
                                   dest_lat, dest_lon)
                << "\n";
      if (util::haversine(position_.lat_deg_, position_.lon_deg_, dest_lat,
                          dest_lon) < 5) {
        move({});
        _climqtt.publish(mqtt::topics::LOCK, out[4]);
        targetCount--;
      }
    } else {
      std::cout << "No value\n";
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(50ms);
    }

    if (!targetCount) break;
  }
}

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