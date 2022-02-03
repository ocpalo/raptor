from time import sleep
from mqtt import uav_mqtt
from process_image import ProcessImage
import cv2

if __name__ == '__main__':
  client = uav_mqtt('localhost', 1883, "raptor")
  client.publish("msg","msg")
  process_image = ProcessImage(client)
  client.subscribe("raptor/land", process_image.landCallback)
  client.subscribe("raptor/processImage", process_image.processImageCallback)
  process_image.process()
  cv2.destroyAllWindows()