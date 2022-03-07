#ifndef RAPTOR_H
#define RAPTOR

#include <mavsdk/plugins/gimbal/gimbal.h>

#include <atomic>
#include <string>

#include "base_drone.h"
#include "mqtt.h"
#include "timer.h"

namespace drone {

class raptor final : public base_drone {
  enum class STATE { INIT, SEARCH, LOCK };

  bool request_process_image_ = false;
  int id_ = 0;
  STATE state_ = STATE::INIT;
  int target_count_ = 3;
  int current_target_id_ = -1;
  drone::mqtt::client_mqtt _climqtt;
  std::unique_ptr<mavsdk::Gimbal> _gimbal = nullptr;
  drone::util::Timer timer;

 public:
  raptor(std::string const& connection_url);
  ~raptor() = default;

  void move2();
  void publish_telemetry();

 protected:
  mavsdk::Action::Result do_land() override;

 private:
  void state_search();
  void state_lock();
};
}  // namespace drone

#endif
