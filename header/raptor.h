#include <atomic>
#include <string>

#include "base_drone.h"
#include "mqtt.h"

namespace drone {

class raptor : public base_drone {
  std::atomic<bool> _publish = true;
  drone::mqtt::client_mqtt _climqtt;

 public:
  raptor(std::string const& connection_url);
  ~raptor() = default;

  void publish_telemetry();
  bool move2(float heading);
  void stopPublish();

 private:
  std::string buildTelemetryMessage();
};
}  // namespace drone