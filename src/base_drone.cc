#include "base_drone.h"
#include "debug.h"
#include <mavsdk/plugins/telemetry/telemetry.h>

#include <future>
#include <chrono>
#include <utility>
#include <exception>
#include <optional>

namespace drone{
    using namespace std::chrono_literals;

    base_drone::base_drone(std::string const& connection_url)
    {
        if (auto connection_result = _mavsdk.add_any_connection(connection_url); connection_result != mavsdk::ConnectionResult::Success) {
            throw std::invalid_argument("Connection url invalid: " + static_cast<int>(connection_result));
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
        debug_print("Subscribed heading");
        _mavsdk_telemetry->subscribe_heading([this](mavsdk::Telemetry::Heading heading){
            this->_heading = heading.heading_deg;
        });

        debug_print("Subscribed position");
        _mavsdk_telemetry->subscribe_position([this](mavsdk::Telemetry::Position pos){
            this->_lat_deg = pos.latitude_deg;
            this->_lon_deg = pos.longitude_deg;
            this->_abs_alt = pos.absolute_altitude_m;
            this->_rel_alt = pos.relative_altitude_m;
        });
    }

    base_drone::~base_drone(){}

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

    mavsdk::Action::Result base_drone::arm() {
        if (const auto arm_result = _mavsdk_action->arm(); arm_result != mavsdk::Action::Result::Success) {
            throw std::runtime_error("Arming failed, Action::Result: " + static_cast<int>(arm_result));
        }
        debug_print("Armed");
        return mavsdk::Action::Result::Success;
    }

    mavsdk::Action::Result base_drone::takeoff(std::optional<float> altitude) {   
        if(altitude.has_value()) 
        {  
            auto value = altitude.value();
            _mavsdk_action->set_takeoff_altitude(value);
            debug_print("Set takeoff altitude:", value);
        }
        if (const auto takeoff_result = _mavsdk_action->takeoff(); takeoff_result != mavsdk::Action::Result::Success) {
            throw std::runtime_error("Takeoff failed, Action::Result: " + static_cast<int>(takeoff_result));
        }
        debug_print("Takeoff started");

        auto in_air_promise = std::promise<void>{};
        auto in_air_future = in_air_promise.get_future();
        _mavsdk_telemetry->subscribe_landed_state([this, &in_air_promise](mavsdk::Telemetry::LandedState state) {
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

    mavsdk::Action::Result base_drone::land() {
        if (const auto land_result = _mavsdk_action->land(); land_result != mavsdk::Action::Result::Success) {
            debug_print("Landing failed, Action::Result: ", static_cast<int>(land_result));
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

    mavsdk::Offboard::Result base_drone::offboard_init() {
        debug_print("Starting Offboard velocity control in body coordinates");

        mavsdk::Offboard::VelocityBodyYawspeed stay{};
        _mavsdk_offboard->set_velocity_body(stay);

        if (mavsdk::Offboard::Result offboard_result = _mavsdk_offboard->start(); offboard_result != mavsdk::Offboard::Result::Success) {
            debug_print("Offboard start failed, Offboard::Result:", static_cast<int>(offboard_result));
            return offboard_result;
        }

        debug_print("Offboard started");
        return mavsdk::Offboard::Result::Success;
    }

    // Need to call offboard_init to switch offboard mode
    mavsdk::Offboard::Result base_drone::hold() {
        debug_print("Offboard control stopped");
        return _mavsdk_offboard->stop();
    }

    // TODO:: implement this using promise or something while waiting data from server
    void base_drone::offboard_hover(int sec) {
        debug_print("Hovering...");
        mavsdk::Offboard::VelocityBodyYawspeed stay{};
        _mavsdk_offboard->set_velocity_body(stay);
        previous_move_ = base_move{};
        std::this_thread::sleep_for(std::chrono::seconds(sec));
    }


    void base_drone::move(base_move move)
    {
        mavsdk::Offboard::VelocityBodyYawspeed msg{};
        msg.forward_m_s = move.forward;
        msg.right_m_s = move.right;
        msg.down_m_s = move.down;
        msg.yawspeed_deg_s = move.yaw;
        _mavsdk_offboard->set_velocity_body(msg);
        previous_move_ = move;
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    void base_drone::move_forward(float speed) {
        mavsdk::Offboard::VelocityBodyYawspeed msg{};
        msg.forward_m_s = speed;
        _mavsdk_offboard->set_velocity_body(msg);
        previous_move_.forward = speed;
    }

    void base_drone::move_right(float speed) {
        mavsdk::Offboard::VelocityBodyYawspeed msg{};
        msg.right_m_s = speed;
        _mavsdk_offboard->set_velocity_body(msg);
        previous_move_.right = speed;
    }

    void base_drone::move_down(float speed) {
        mavsdk::Offboard::VelocityBodyYawspeed msg{};
        msg.down_m_s = -speed;
        _mavsdk_offboard->set_velocity_body(msg);
        previous_move_.down = speed;
    }

    void base_drone::subscribe_heading(double rate_hz) {
        _mavsdk_telemetry->set_rate_position(rate_hz);
        _mavsdk_telemetry->subscribe_heading([this](mavsdk::Telemetry::Heading heading){
            this->_heading = heading.heading_deg;
            debug_print("Heading: ", this->_heading);
        });
        debug_print("Subscribed heading");
    }

    void base_drone::unsubscribe_heading() {
        _mavsdk_telemetry->subscribe_heading(nullptr);
        debug_print("Unsubscribed heading");
    }

    void base_drone::set_heading(float dest_heading) {
        auto yaw_rate = (dest_heading - _heading) / 3;

        mavsdk::Offboard::VelocityBodyYawspeed msg{};
        msg.yawspeed_deg_s = yaw_rate;
        msg.forward_m_s = previous_move_.forward;
        msg.right_m_s = previous_move_.right;
        _mavsdk_offboard->set_velocity_body(msg);
    }
}