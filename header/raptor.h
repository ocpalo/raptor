#ifndef RAPTOR_H
#define RAPTOR

#include <mavsdk/plugins/gimbal/gimbal.h>

#include <atomic>
#include <string>

#include "base_drone.h"
#include "mqtt.h"

namespace drone {

class raptor final : public base_drone {
  int id_ = 0;
  std::atomic<bool> _publish_telemetry = true;
  drone::mqtt::client_mqtt _climqtt;
  std::unique_ptr<mavsdk::Gimbal> _gimbal = nullptr;

 public:
  raptor(std::string const& connection_url);
  ~raptor() = default;

  void move2();
  void publish_telemetry();
  void stop_publish_telemetry();

 protected:
  mavsdk::Action::Result do_land() override;
};
}  // namespace drone

#endif
