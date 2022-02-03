import atexit
from datetime import datetime
import logging
import cv2
from http import client
from mqtt import uav_mqtt

im_topic = "im/coord"

class ProcessImage:
  def __init__(self, mqtt_client):
    self.mqtt_cli = mqtt_client
    self.process_image = False
    self.land = False
    self.logger = logging.getLogger("image")
    self.logger.setLevel("INFO")
    self.log_file_handler = logging.FileHandler(filename="image.log", mode="w")
    self.logger.addHandler(self.log_file_handler)
    self.logger.info("{} | Creating ProcessImage instance.".format(datetime.now()))
    
    self.cap = cv2.VideoCapture("udpsrc port=5600 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=H264,framerate=30/1 ! rtph264depay ! avdec_h264 ! videoconvert ! appsink",cv2.CAP_GSTREAMER)
    if not self.cap.isOpened():
      self.logger.error("{} | Failed to open video capture.".format(datetime.now()))

    self.logger.info("{} | Video capture opened".format(datetime.now()))

    w=int(self.cap.get(cv2.CAP_PROP_FRAME_WIDTH ))
    h=int(self.cap.get(cv2.CAP_PROP_FRAME_HEIGHT ))
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    self.video_writer = cv2.VideoWriter("output.avi", fourcc, 25, (w, h))
    self.logger.info("{} | VideoWriter created. Width: {}, Height: {}, Fourcc: {}".format(datetime.now(), w, h, fourcc))
    atexit.register(self.cleanup)

  def cleanup(self):
    if self.cap.isOpened():
      self.cap.release()
      self.video_writer.release()
      self.logger.info("{} | Video capture and video writer released".format(datetime.now()))

  # TODO this function should return x,y message
  def process(self):
    while not self.land:
      ret,frame = self.cap.read()
      if not ret:
        self.logger.warning("{} | Reading from VideoCapture failed. Terminating process".format(datetime.now()))
        exit(1)
      
      # TODO:: Implement image processing here
      if self.process_image:
        self.mqtt_cli.publish(im_topic, "1")
      # end of implementation
      self.video_writer.write(frame)
      cv2.imshow("Frame", frame)

      if cv2.waitKey(1)&0XFF == ord('q'):
        break

  def landCallback(self, client, userdata, message):
    self.land = True
    self.logger.info("{} | Land Flag is set: {}".format(datetime.now(), self.land))

  def processImageCallback(self, client, userdata, message):
    self.process_image = ~self.process_image
    self.logger.info("{} | Process Image Flag is set: {}".format(datetime.now(), self.process_image))

