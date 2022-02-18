import atexit
import logging
from datetime import date, datetime
import random
import paho.mqtt.client as mqtt


class uav_mqtt:
    def __init__(self, host, port, client_id):
        self.logger = logging.getLogger("mqtt")
        self.logger.setLevel("INFO")
        self.log_file_handler = logging.FileHandler(
            filename="mqtt.log", mode="w")
        self.logger.addHandler(self.log_file_handler)

        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                pass
            else:
                self.logger.error(
                    "{} | Failed to connect mqtt. Terminating process".format(datetime.now()))
                exit(0)
        self.client = mqtt.Client(f'{client_id}-{random.randint(0, 1000)}')
        self.client.on_connect = on_connect
        self.client.connect(host, port)
        self.logger.info("{} | Connected to mqtt host: {}, port: {}, client id: {}".format(
            datetime.now(), host, port, client_id))
        atexit.register(self.disconnect)

    def publish(self, topic, message):
        res = self.client.publish(topic, message)
        status = res[0]
        if status == 0:
            pass
        else:
            self.logger.error("{} | Failed to publish message. Status: {}, Topic: {}, Message: {}".format(
                status, datetime.now(), topic, message))

    def subscribe(self, topic, callback_fn):
        self.client.subscribe(topic)
        self.client.message_callback_add(topic, callback_fn)
        self.logger.info(
            "{} | Subscribed to topic {}".format(datetime.now(), topic))

    def disconnect(self):
        self.client.disconnect()
        self.logger.info("{} | mqtt disconnected".format(datetime.now()))
