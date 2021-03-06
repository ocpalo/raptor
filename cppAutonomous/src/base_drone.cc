#include "base_drone.h"

#include <mavsdk/plugins/telemetry/telemetry.h>

#include <chrono>
#include <cmath>
#include <exception>
#include <future>
#include <optional>
#include <utility>

#include "debug.h"
#include "mavsdk/plugins/param/param.h"
#include "util.h"

namespace drone {
using namespace std::chrono_literals;

base_drone::base_drone(std::string const& connection_url) : offboard_(false) {
  if (auto connection_result = _mavsdk.add_any_connection(connection_url);
      connection_result != mavsdk::ConnectionResult::Success) {
    throw std::invalid_argument("Connection url invalid: " +
                                static_cast<int>(connection_result));
  }
  _mavsdk_system = get_system(_mavsdk);
  _mavsdk_action.reset(new mavsdk::Action{_mavsdk_system});
  _mavsdk_offboard.reset(new mavsdk::Offboard{_mavsdk_system});
  _mavsdk_telemetry.reset(new mavsdk::Telemetry{_mavsdk_system});

  while (!_mavsdk_telemetry->health_all_ok()) {
    debug_print("Waiting for system to be ready");
    std::this_thread::sleep_for(1s);
  }
  debug_print("System is ready");
  debug_print("Subscription to telemetry operation begin");
  set_telemetry_subscriptions(2);

#ifdef _PRINT_IN_DEBUG_
  debug_print("Setting `COM_RCL_EXCEPT` parameter to 4");
  auto param = mavsdk::Param(_mavsdk_system);
  param.set_param_int("COM_RCL_EXCEPT", 4);
#endif
}

base_drone::~base_drone() {}

std::shared_ptr<mavsdk::System> base_drone::get_system(mavsdk::Mavsdk& mavsdk) {
  debug_print("Waiting to discover system...");
  auto prom = std::promise<std::shared_ptr<mavsdk::System>>{};
  auto fut = prom.get_future();

  mavsdk.subscribe_on_new_system([&mavsdk, &prom]() {
    auto system = mavsdk.systems().back();

    if (system->has_autopilot()) {
      debug_print("Discovered autopilot");
      mavsdk.subscribe_on_new_system(nullptr);
      prom.set_value(system);
    }
  });

  if (fut.wait_for(3s) == std::future_status::timeout) {
    throw std::runtime_error("No autopilot found");
  }
  return fut.get();
}

void base_drone::set_telemetry_subscriptions(double hz) {
  debug_print("Subscribed heading");
  _mavsdk_telemetry->set_rate_position(hz);
  _mavsdk_telemetry->subscribe_heading(
      [this](mavsdk::Telemetry::Heading heading) {
        this->_heading = heading.heading_deg;
      });

  debug_print("Subscribed position");
  _mavsdk_telemetry->subscribe_position(
      [this](mavsdk::Telemetry::Position pos) {
        this->position_.lat_deg_ = pos.latitude_deg;
        this->position_.lon_deg_ = pos.longitude_deg;
        this->position_.abs_alt_ = pos.absolute_altitude_m;
        this->position_.rel_alt_ = pos.relative_altitude_m;
      });

  debug_print("Subscribed euler angle");
  auto result = _mavsdk_telemetry->set_rate_attitude(hz);
  if (result != mavsdk::Telemetry::Result::Success)
    debug_print("Set Rate Attitude failed. Default rate used");
  _mavsdk_telemetry->subscribe_attitude_euler(
      [this](mavsdk::Telemetry::EulerAngle euler_angle) {
        this->attitude_.roll_deg_ = euler_angle.roll_deg;
        this->attitude_.pitch_deg_ = euler_angle.pitch_deg;
        this->attitude_.yaw_deg_ = euler_angle.yaw_deg;
        this->attitude_.timestamp_ = euler_angle.timestamp_us;
      });

  debug_print("Subscribed battery");
  _mavsdk_telemetry->set_rate_battery(hz);
  _mavsdk_telemetry->subscribe_battery(
      [this](mavsdk::Telemetry::Battery battery) {
        this->battery_remaning_percent_ = battery.remaining_percent;
      });

  debug_print("Subscribed velocity NED");
  _mavsdk_telemetry->set_rate_position_velocity_ned(hz);
  _mavsdk_telemetry->subscribe_velocity_ned(
      [this](mavsdk::Telemetry::VelocityNed velocity) {
        this->speed_m_s_ = std::max(
            {velocity.north_m_s, velocity.east_m_s, velocity.down_m_s});
      });
}

mavsdk::Action::Result base_drone::arm() {
  debug_print("Set: Arm");
  auto result = do_arm();
  debug_print("Armed");
  return result;
}

mavsdk::Action::Result base_drone::takeoff(std::optional<float> altitude) {
  return do_takeoff(altitude);
}

mavsdk::Action::Result base_drone::land() { return do_land(); }

mavsdk::Offboard::Result base_drone::hold() { return do_hold(); };

mavsdk::Offboard::Result base_drone::offboard_init() {
  return do_offboard_init();
}

void base_drone::offboard_hover(int seconds) { do_offboard_hover(seconds); }

void base_drone::move(base_move move) { do_move(std::move(move)); }

void base_drone::move_m(double meter, base_move move) {
  do_move_m(meter, std::move(move));
}

void base_drone::down_m(double meter, base_move move) {
  do_down_m(meter, std::move(move));
}

void base_drone::set_heading(float dest_heading) {
  do_set_heading(dest_heading);
}

mavsdk::Action::Result base_drone::do_arm() {
  if (const auto arm_result = _mavsdk_action->arm();
      arm_result != mavsdk::Action::Result::Success) {
    throw std::runtime_error("Arming failed, Action::Result: " +
                             static_cast<int>(arm_result));
  }
  return mavsdk::Action::Result::Success;
}

mavsdk::Action::Result base_drone::do_takeoff(std::optional<float> altitude) {
  if (altitude.has_value()) {
    auto value = altitude.value();
    _mavsdk_action->set_takeoff_altitude(value);
    debug_print("Set takeoff altitude:", value);
  }
  if (const auto takeoff_result = _mavsdk_action->takeoff();
      takeoff_result != mavsdk::Action::Result::Success) {
    throw std::runtime_error("Takeoff failed, Action::Result: " +
                             static_cast<int>(takeoff_result));
  }
  debug_print("Takeoff started");

  auto in_air_promise = std::promise<void>{};
  auto in_air_future = in_air_promise.get_future();
  _mavsdk_telemetry->subscribe_landed_state(
      [this, &in_air_promise](mavsdk::Telemetry::LandedState state) {
        if (state == mavsdk::Telemetry::LandedState::InAir) {
          debug_print("Taking off has finished.");
          _mavsdk_telemetry->subscribe_landed_state(nullptr);
          in_air_promise.set_value();
        }
      });

  in_air_future.wait_for(20s);
  if (in_air_future.wait_for(10s) == std::future_status::timeout) {
    throw std::runtime_error("Takeoff timed out.");
  }

  return mavsdk::Action::Result::Success;
}

mavsdk::Action::Result base_drone::do_land() {
  if (const auto land_result = _mavsdk_action->land();
      land_result != mavsdk::Action::Result::Success) {
    debug_print("Landing failed, Action::Result: ",
                static_cast<int>(land_result));
    return mavsdk::Action::Result::Unknown;
  }

  while (_mavsdk_telemetry->in_air()) {
    debug_print("Vehicle is landing...");
    std::this_thread::sleep_for(3s);
  }
  debug_print("Landed!");
  std::this_thread::sleep_for(3s);
  return mavsdk::Action::Result::Success;
}

mavsdk::Offboard::Result base_drone::do_offboard_init() {
  debug_print("Starting Offboard velocity control in body coordinates");

  mavsdk::Offboard::VelocityBodyYawspeed stay{};
  _mavsdk_offboard->set_velocity_body(stay);

  if (mavsdk::Offboard::Result offboard_result = _mavsdk_offboard->start();
      offboard_result != mavsdk::Offboard::Result::Success) {
    debug_print("Offboard start failed, Offboard::Result:",
                static_cast<int>(offboard_result));
    return offboard_result;
  }

  debug_print("Offboard started");
  this->offboard_ = true;
  return mavsdk::Offboard::Result::Success;
}

// Need to call offboard_init to switch offboard mode
mavsdk::Offboard::Result base_drone::do_hold() {
  debug_print("Offboard control stopped");
  this->offboard_ = false;
  return _mavsdk_offboard->stop();
}

// TODO:: implement this using promise or something while waiting data from
// server
void base_drone::do_offboard_hover(int sec) {
  debug_print("Hovering...");
  mavsdk::Offboard::VelocityBodyYawspeed stay{};
  _mavsdk_offboard->set_velocity_body(stay);
  previous_move_ = base_move{};
  std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void base_drone::do_move(base_move move) {
  mavsdk::Offboard::VelocityBodyYawspeed msg{};

  msg.forward_m_s = move.forward;
  msg.right_m_s = move.right;
  msg.down_m_s = move.down;
  msg.yawspeed_deg_s = move.yaw;
  _mavsdk_offboard->set_velocity_body(msg);
  previous_move_ = move;
}

void base_drone::do_move_m(double meter, base_move move) {
  base_position prev = position_;
  while (drone::util::haversine(prev.lat_deg_, prev.lon_deg_,
                                position_.lat_deg_, position_.lon_deg_) +
             0.5 <
         meter) {
    this->move(move);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
  }
  this->move({});
}

// Improvements needed
void base_drone::do_down_m(double meter, base_move move) {
  move.down = move.down == 0 ? 3 : move.down;
  auto current_altitude = position_.rel_alt_;

  auto distance_covered = 0;
  auto down_speed = 0;
  mavsdk::Offboard::VelocityBodyYawspeed msg{};
  while (std::abs(distance_covered - meter < 0.5)) {
    down_speed = position_.rel_alt_ < 3 ? 0 : move.down;
    msg.forward_m_s = move.forward;
    msg.right_m_s = move.right;
    msg.down_m_s = down_speed;
    msg.yawspeed_deg_s = move.yaw;
    _mavsdk_offboard->set_velocity_body(msg);

    previous_move_ = move;
    distance_covered += std::abs(position_.rel_alt_ - current_altitude);
  }

  this->move({});
}

void base_drone::do_set_heading(float dest_heading) {
  auto yaw_rate = dest_heading - _heading;

  mavsdk::Offboard::VelocityBodyYawspeed msg{};
  msg.yawspeed_deg_s = yaw_rate;
  msg.forward_m_s = previous_move_.forward;
  msg.right_m_s = previous_move_.right;
  msg.down_m_s = previous_move_.down;
  _mavsdk_offboard->set_velocity_body(msg);
}

void base_drone::subscribe_heading(double rate_hz) {
  _mavsdk_telemetry->set_rate_position(rate_hz);
  _mavsdk_telemetry->subscribe_heading(
      [this](mavsdk::Telemetry::Heading heading) {
        this->_heading = heading.heading_deg;
      });
  debug_print("Subscribed heading");
}

void base_drone::unsubscribe_heading() {
  _mavsdk_telemetry->subscribe_heading(nullptr);
  debug_print("Unsubscribed heading");
}

std::ostream& operator<<(std::ostream& stream,
                         const drone::base_move& base_move) {
  std::cout << "Forward:" << base_move.forward << " Right:" << base_move.right
            << " Down:" << base_move.down << " Yaw:" << base_move.yaw << "\n";
  return stream;
}

std::ostream& operator<<(std::ostream& stream,
                         const drone::base_position& base_position) {
  std::cout << "Lat:" << base_position.lat_deg_
            << " Lon:" << base_position.lon_deg_
            << " Abs_Alt:" << base_position.abs_alt_
            << " Rel_Alt:" << base_position.rel_alt_ << "\n";
  return stream;
}

std::ostream& operator<<(std::ostream& stream,
                         const drone::base_attitude& base_attitude) {
  std::cout << "Roll:" << base_attitude.roll_deg_
            << " Pitch:" << base_attitude.pitch_deg_
            << " Yaw:" << base_attitude.yaw_deg_
            << " Timestamp:" << base_attitude.timestamp_ << "\n";
  return stream;
}
}  // namespace drone