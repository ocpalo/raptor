#ifndef BASE_DRONE_H
#define BASE_DRONE_H

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

#include <string>
#include <utility>

#include "debug.h"

namespace drone {

struct base_move {
  float forward = 0;
  float right = 0;
  float down = 0;
  float yaw = 0;
  friend std::ostream& operator<<(std::ostream& stream,
                                  const base_move& base_move);
};

struct base_position {
  double lat_deg_;
  double lon_deg_;
  float abs_alt_;
  float rel_alt_;
  friend std::ostream& operator<<(std::ostream& stream,
                                  const base_position& base_position);
};

struct base_attitude {
  float roll_deg_;
  float pitch_deg_;
  float yaw_deg_;
  uint16_t timestamp_;
  friend std::ostream& operator<<(std::ostream& stream,
                                  const base_attitude& base_attitude);
};

class base_drone {
 private:
  mavsdk::Mavsdk _mavsdk;
  std::shared_ptr<mavsdk::System> _mavsdk_system;

 protected:
  std::unique_ptr<mavsdk::Action> _mavsdk_action = nullptr;
  std::unique_ptr<mavsdk::Offboard> _mavsdk_offboard = nullptr;
  std::unique_ptr<mavsdk::Telemetry> _mavsdk_telemetry = nullptr;

 protected:
  double _heading;
  base_position position_;
  base_move previous_move_;
  base_attitude attitude_;
  float battery_remaning_percent_;
  bool offboard_;
  float speed_m_s_;

 public:
  base_drone(std::string const& connection_url);
  virtual ~base_drone() = 0;

 private:
  static std::shared_ptr<mavsdk::System> get_system(mavsdk::Mavsdk& mavsdk);
  void set_telemetry_subscriptions(double hz);

 public:
  mavsdk::Action::Result arm();
  mavsdk::Action::Result takeoff(std::optional<float> altitude = {});
  mavsdk::Action::Result land();
  mavsdk::Offboard::Result hold();
  mavsdk::Offboard::Result offboard_init();
  void offboard_hover(int seconds);
  void move(base_move move);
  void move_m(double meter, base_move move);
  void down_m(double meter, base_move move);
  [[deprecated]] void move_forward(float speed);
  [[deprecated]] void move_right(float speed);
  [[deprecated]] void move_down(float speed);
  void set_heading(float dest_heading);

 protected:
  void subscribe_heading(double rate_hz);
  void unsubscribe_heading();
};

}  // namespace drone

#endif