import atexit
from datetime import datetime
import logging
import cv2
from http import client
from mqtt import uav_mqtt
import numpy as np
import math
import time

im_topic = "im/coord"


class ProcessImage:
    def __init__(self, mqtt_client):
        self.net = cv2.dnn.readNet("yolo_files/yolov4-tiny-custom_best.weights",
                                   "yolo_files/yolov4-tiny-custom.cfg")

        self.net.setPreferableBackend(cv2.dnn.DNN_BACKEND_CUDA)
        self.net.setPreferableTarget(cv2.dnn.DNN_TARGET_CUDA)

        self.classes = []
        with open("yolo_files/obj.names", "r") as f:
            self.classes = [line.strip() for line in f.readlines()]

        self.layer_names = self.net.getLayerNames()
        self.output_layers = [self.layer_names[i[0] - 1]
                              for i in self.net.getUnconnectedOutLayers()]
        self.colors = np.random.uniform(0, 255, size=(len(self.classes), 3))
        self.prev_frame_time = 0
        self.new_frame_time = 0
        self.center_x, self.center_y, self.x_axis, self.y_axis, self.box_width, self.box_height = 0, 0, 0, 0, 0, 0
        self.mqtt_cli = mqtt_client
        self.mqtt_cli.client.loop_start()
        self.process_image = False
        self.land = False
        self.logger = logging.getLogger("image")
        self.logger.setLevel("INFO")
        self.log_file_handler = logging.FileHandler(
            filename="image.log", mode="w")
        self.logger.addHandler(self.log_file_handler)
        self.logger.info(
            "{} | Creating ProcessImage instance.".format(datetime.now()))

        self.cap = cv2.VideoCapture(
            "udpsrc port=5600 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=H264,framerate=30/1 ! rtph264depay ! avdec_h264 ! videoconvert ! appsink", cv2.CAP_GSTREAMER)
        if not self.cap.isOpened():
            self.logger.error(
                "{} | Failed to open video capture.".format(datetime.now()))

        self.logger.info("{} | Video capture opened".format(datetime.now()))

        self.w = 416
        self.h = 416

        self.tracker = cv2.TrackerCSRT_create()
        self.initObj = None
        time.sleep(1)

        fourcc = cv2.VideoWriter_fourcc(*'XVID')
        self.video_writer = cv2.VideoWriter(
            "output.avi", fourcc, 25, (self.w, self.h))
        self.logger.info("{} | VideoWriter created. Width: {}, Height: {}, Fourcc: {}".format(
            datetime.now(), self.w, self.h, fourcc))
        atexit.register(self.cleanup)

    def cleanup(self):
        if self.cap.isOpened():
            self.cap.release()
            self.video_writer.release()
            self.logger.info(
                "{} | Video capture and video writer released".format(datetime.now()))

    # TODO this function should return x,y message
    def process(self):
        while not self.land:
            ret, frame = self.cap.read()

            if not ret:
                self.logger.warning(
                    "{} | Reading from VideoCapture failed. Terminating process".format(datetime.now()))
                exit(1)

            if not self.initObj:
                self.logger.info(
                    "{} | Tracker lost the object".format(datetime.now()))
            # TODO:: Implement image processing here
                if self.process_image:
                    yoloResults = self.yoloRecognition(frame)
                    if yoloResults[0]:
                        positionMessage = self.targetObjectPositionWithFOV()
                        self.mqtt_cli.publish(im_topic, positionMessage)
                        self.drawBoxAndInformation(
                            frame, yoloResults[1], yoloResults[2], yoloResults[3], yoloResults[4])
            else:
                ok, bbox = self.tracker.update(frame)
                if not ok:
                    self.tracker = cv2.TrackerCSRT_create()
                    self.initObj = False
                else:
                    print("Tracker tracking!")
                    p1 = (int(bbox[0]), int(bbox[1]))
                    p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
                    cv2.rectangle(frame, p1, p2, (255, 0, 0), 2, 1)
                    align = str(((bbox[0] + (bbox[2])/2)-self.w/2)*1.3/self.w) + \
                        "," + str(((bbox[1] + (bbox[3])/2) -
                                  self.h/2)*1.3/self.h)
                    print(align)
                    self.mqtt_cli.publish(im_topic, align)

            self.showFPS(frame)

            # end of implementation
            self.video_writer.write(frame)
            cv2.imshow("Frame", frame)

            if cv2.waitKey(1) & 0XFF == ord('q'):
                break

    def yoloRecognition(self, frame):
        # Detecting objects
        blob = cv2.dnn.blobFromImage(
            frame, 0.00392, (self.w, self.h), (0, 0, 0), True, crop=False)
        self.net.setInput(blob)
        outs = self.net.forward(self.output_layers)

        isObjectFound = False
        class_ids = []
        confidences = []
        boxes = []
        for out in outs:
            for detection in out:
                scores = detection[5:]
                class_id = np.argmax(scores)
                confidence = scores[class_id]
                if confidence > 0.5:
                    # Object detected
                    isObjectFound = True
                    self.center_x = int(detection[0] * self.w)
                    self.center_y = int(detection[1] * self.h)
                    self.box_width = int(detection[2] * self.w)
                    self.box_height = int(detection[3] * self.h)

                    # Rectangle coordinates
                    self.x_axis = int(self.center_x - self.box_width / 2)
                    self.y_axis = int(self.center_y - self.box_height / 2)

                    boxes.append([self.x_axis, self.y_axis,
                                 self.box_width, self.box_height])
                    confidences.append(float(confidence))
                    class_ids.append(class_id)

        # to eliminate multiple boxes for same object
        indexes = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.4)
        center_coordinates = str(self.center_x) + "," + str(self.center_y)

        return [isObjectFound, boxes, indexes, class_ids, center_coordinates]

    def targetObjectPositionWithFOV(self, horizontal_fov=1.3, vertical_fov=1.3):

        align = str((self.center_x-self.w/2)*horizontal_fov/self.w) + \
            "," + str((self.center_y-self.h/2)*vertical_fov/self.h)
        return align

    def drawBoxAndInformation(self, frame, boxes, indexes, class_ids, center_coordinates):

        font = cv2.FONT_HERSHEY_PLAIN
        for i in range(len(boxes)):
            if i in indexes:
                self.x_axis, self.y_axis, self.box_width, self.box_height = boxes[i]
                label = str(self.classes[class_ids[i]])
                if label == self.classes[0]:
                    color = (0, 255, 0)
                elif label == self.classes[1]:
                    color = (0, 0, 255)
                cv2.rectangle(frame, (self.x_axis, self.y_axis), (self.x_axis +
                              self.box_width, self.y_axis + self.box_height), color, 2)

                ok = self.tracker.init(
                    frame, (self.x_axis, self.y_axis, self.box_width, self.box_height))
                self.initObj = True

                cv2.putText(frame, label, (self.x_axis,
                            self.y_axis + 30), font, 3, color, 3)
                cv2.putText(frame, ".", (self.center_x, self.center_y),
                            font, 3, (255, 0, 0), 3)
                cv2.putText(frame, center_coordinates, (self.center_x,
                            self.center_y+30), font, 1, (0, 0, 255), 2)

    def showFPS(self, frame):
        font = cv2.FONT_HERSHEY_PLAIN
        self.new_frame_time = time.time()
        fps = 1/(self.new_frame_time-self.prev_frame_time)
        self.prev_frame_time = self.new_frame_time
        fps = int(fps)
        fps = str(fps)
        cv2.putText(frame, fps, (7, 70), font, 3,
                    (100, 255, 0), 3, cv2.LINE_AA)

    def landCallback(self, client, userdata, message):
        self.land = True
        self.logger.info("{} | Land Flag is set: {}".format(
            datetime.now(), self.land))

    def processImageCallback(self, client, userdata, message):
        self.process_image = not self.process_image
        self.logger.info("{} | Process Image Flag is set: {}".format(
            datetime.now(), self.process_image))

    def setCallbacks(self):
        self.mqtt_cli.subscribe("raptor/land", self.landCallback)
        self.mqtt_cli.subscribe("raptor/processImage",
                                self.processImageCallback)
