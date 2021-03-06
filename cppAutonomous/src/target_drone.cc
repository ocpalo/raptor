#include "target_drone.h"

#include <chrono>
#include <string>
#include <thread>

#include "base_drone.h"

/*
 *
 *  -----------------------------
 * |             td1             |
 * |                             |
 * |                             |
 * |                             |
 * |                             |
 * |                             |
 * |     td2              td3    |
 *  -----------------------------
 *
 *  td = target_drone
 */

namespace drone {

target_drone::target_drone(std::string const& conn_url, int mission, int id)
    : base_drone(conn_url),
      initialHeading(mock_drone_initial_heading[id - 1]),
      id_(id),
      _climqtt(drone::SERVER_ADDRESS, std::to_string(id_)) {
  _climqtt.connect();
}

void target_drone::mission_1() {
  debug_print(".**************MISSION 1 started**************");
  using namespace std::chrono_literals;

  while (std::abs(initialHeading - this->_heading) > 3) {
    set_heading(initialHeading);
    std::this_thread::sleep_for(50ms);
  }

  // Move forward 40 meter, 10m/s
  move_m(40, {.forward = 10});
  offboard_hover(5);

  float speed = 3.0;
  while (1) {
    move_m(40, {.forward = speed});
    move_m(40, {.right = speed});
    move_m(40, {.forward = -speed});
    move_m(40, {.right = -speed});
  }
}

void target_drone::mission_2() {
  debug_print(".**************MISSION 2 started**************");
  using namespace std::chrono_literals;

  while (std::abs(initialHeading - this->_heading) > 3) {
    set_heading(initialHeading);
    std::this_thread::sleep_for(50ms);
  }

  // Move forward 40 meter, 10m/s
  move_m(40, {.forward = 10});
  offboard_hover(3);

  float speed = 3.0;
  while (1) {
    move_m(40, {.forward = speed, .yaw = speed * 4});
    move_m(40, {.right = speed, .yaw = speed * 4});
    move_m(40, {.forward = -speed, .yaw = speed * 4});
    move_m(40, {.right = -speed, .yaw = speed * 4});
  }
}

void target_drone::mission_3() {
  debug_print(".**************MISSION 3 started**************");
  using namespace std::chrono_literals;

  while (std::abs(initialHeading - this->_heading) > 3) {
    set_heading(initialHeading);
    std::this_thread::sleep_for(50ms);
  }

  // Move forward 40 meter, 10m/s
  move_m(40, {.forward = 10});
  offboard_hover(3);

  float speed = 3.0;
  while (1) {
    move({.forward = speed, .yaw = speed * 4});
    std::this_thread::sleep_for(std::chrono::minutes(1));
  }
}

void target_drone::control_wasd() {
  using namespace std::chrono_literals;

  char c = '0';
  int ws_speed, ad_speed;
  ws_speed = 0;
  ad_speed = 0;
  while (c != 'q') {
    std::cin >> c;
    switch (c) {
      case 'w':
        ws_speed++;
        move({.forward = ws_speed, .right = ad_speed});
        break;
      case 'a':
        ad_speed--;
        move({.forward = ws_speed, .right = ad_speed});
        break;
      case 's':
        ws_speed--;
        move({.forward = ws_speed, .right = ad_speed});
        break;
      case 'd':
        ad_speed++;
        move({.forward = ws_speed, .right = ad_speed});
        break;
      default:
        break;
    }
    std::this_thread::sleep_for(0.03s);
  }
}

void target_drone::run(char* argv) {
  auto fut = std::async(std::launch::async,
                        &drone::target_drone::publish_telemetry, this);
  using namespace std::chrono_literals;
  std::jthread thr;
  switch (*argv) {
    case '1':
      thr = std::jthread(&target_drone::mission_1, this);
      break;
    case '2':
      thr = std::jthread(&target_drone::mission_2, this);
      break;
    case '3':
      thr = std::jthread(&target_drone::mission_3, this);
      break;
    case '4':
      control_wasd();

    default:
      break;
  }

  char c = '0';
  while (c != 'q') {
    std::cin >> c;
    std::this_thread::sleep_for(0.01s);
  }
  move({});
  stop_publish_telemetry();
}

void target_drone::set_id(int id) { id_ = id; }

void target_drone::publish_telemetry() {
  while (_publish_telemetry) {
    _climqtt.publish(drone::mqtt::topics::TARGET_TELEMETRY_TOPIC,
                     build_telemetry_message());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
void target_drone::stop_publish_telemetry() { _publish_telemetry = false; }

std::string target_drone::build_telemetry_message() {
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