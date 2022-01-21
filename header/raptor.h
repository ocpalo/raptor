#include <atomic>
#include <string>

#include "base_drone.h"
#include "mqtt.h"

namespace drone {

class raptor : public base_drone {
  int id_;
  std::atomic<bool> _publish_telemetry = true;
  drone::mqtt::client_mqtt _climqtt;

 public:
  raptor(std::string const& connection_url);
  ~raptor() = default;

  bool move2(float heading);
  void land();
  void publish_telemetry();
  void stop_publish_telemetry();

 private:
  std::string build_telemetry_message();
};
}  // namespace drone