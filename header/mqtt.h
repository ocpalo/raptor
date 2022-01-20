#ifndef MQTT_H
#define MQTT_H

#include "mqtt/client.h"

namespace drone {
constexpr auto SERVER_ADDRESS{"tcp://localhost:1883"};
constexpr auto CLIENT_ID{"raptor"};

namespace mqtt {
constexpr auto TELEMETRY_TOPIC{"raptor/telemetry"};
constexpr auto TELEMETRY_RESPONSE_TOPIC{"raptor/telemetry/response"};
constexpr auto LAND_TOPIC{"raptor/land"};
constexpr auto MQTT_QOS{0};

class client_mqtt {
 public:
  client_mqtt(std::string const& server_address, std::string const& client_id);
  client_mqtt(std::string const& server_address, std::string const& client_id,
              int QOS);
  void publish(std::string const& topic, std::string const& msg);
  bool connect();
  void disconnect();
  bool isConnected() const;
  bool reconnect();

 private:
  ::mqtt::client client_;
  ::mqtt::connect_options connOpts_;
  int QOS_ = MQTT_QOS;
};
}  // namespace mqtt
}  // namespace drone

#endif
