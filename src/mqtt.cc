#include "mqtt.h"

#include "debug.h"

namespace drone {
namespace mqtt {

client_mqtt::client_mqtt(std::string const& server_address,
                         std::string const& client_id)
    : client_mqtt(server_address, client_id, MQTT_QOS) {}

client_mqtt::client_mqtt(std::string const& server_address,
                         std::string const& client_id, int QOS)
    : client_(server_address, client_id), QOS_(QOS) {
  debug_print("MQTT client created");
  debug_print("Setting QOS:", QOS_);
  debug_print("Setting connection options for client mqtt");
  connOpts_.set_keep_alive_interval(10);
  connOpts_.set_clean_session(true);
  connOpts_.set_automatic_reconnect(3, 10);
  debug_print("Configuration mqtt client connection options are done!");
}

void client_mqtt::publish(std::string const& topic, std::string const& msg) {
  client_.publish(::mqtt::message(topic, msg, QOS_, false));
  debug_print("Send message", msg);
}

bool client_mqtt::connect() {
  auto resp = client_.connect(connOpts_);
  return resp.is_session_present();
}

void client_mqtt::disconnect() { client_.disconnect(); }

bool client_mqtt::isConnected() const { return client_.is_connected(); }

bool client_mqtt::reconnect() {
  auto resp = client_.reconnect();
  return resp.is_session_present();
}
}  // namespace mqtt
}  // namespace drone