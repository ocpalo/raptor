#include <atomic>
#include <string>

#include "base_drone.h"
#include "mqtt.h"

namespace drone {
int constexpr mock_drone_initial_heading[] = {120, 240, 360};

class test_drone final : public base_drone {
  int initialHeading;
  int id_;
  std::atomic<bool> _publish_telemetry = true;
  drone::mqtt::client_mqtt _climqtt;

 public:
  test_drone(std::string const& connection_url, int mission, int id);
  ~test_drone() = default;

  void publish_telemetry();
  void stop_publish_telemetry();
  void set_id(int id);
  std::string build_telemetry_message();

  void mission_1();
  void mission_2();
  void mission_3();
  void run(char* argv);
};
}  // namespace drone