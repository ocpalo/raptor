#ifndef TARGET_DRONE_H
#define TARGET_DRONE_H

#include <atomic>
#include <string>

#include "base_drone.h"
#include "mqtt.h"

namespace drone {
static constexpr int mock_drone_initial_heading[] = {120, 240, 360};

class target_drone final : public base_drone {
  int initialHeading;
  int id_;
  std::atomic<bool> _publish_telemetry = true;
  drone::mqtt::client_mqtt _climqtt;

 public:
  target_drone(std::string const& connection_url, int mission, int id);
  ~target_drone() = default;

  void publish_telemetry();
  void stop_publish_telemetry();
  void set_id(int id);
  std::string build_telemetry_message();

  void mission_1();
  void mission_2();
  void mission_3();
  void control_wasd();
  void run(char* argv);
};
}  // namespace drone

#endif
