from time import sleep
from mqtt import uav_mqtt
from process_image import ProcessImage
import cv2

if __name__ == '__main__':
    client = uav_mqtt('localhost', 1883, "raptor")
    client.publish("msg", "msg")
    process_image = ProcessImage(client)
    process_image.setCallbacks()
    process_image.process()
    cv2.destroyAllWindows()
