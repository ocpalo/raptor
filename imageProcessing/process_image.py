import atexit
from datetime import datetime
import logging
import textwrap
from turtle import color, width
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

        self.isLockedTargets = [False, False, False]
        self.start_time = None

    def cleanup(self):
        if self.cap.isOpened():
            self.cap.release()
            self.video_writer.release()
            self.logger.info(
                "{} | Video capture and video writer released".format(datetime.now()))

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
                if self.process_image:
                    yoloResults = self.yoloRecognition(frame)
                    if yoloResults[0]:
                        positionMessage = self.targetObjectPositionWithFOV()
                        self.mqtt_cli.publish(im_topic, positionMessage)
                        self.drawBoxAndInformation(frame, yoloResults[1], yoloResults[2], yoloResults[3], yoloResults[4])
            else:
                ok, bbox = self.tracker.update(frame)
                if not ok:
                    self.tracker = cv2.TrackerCSRT_create()
                    self.initObj = False
                else:
                    p1 = (int(bbox[0]), int(bbox[1]))
                    p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
                    cv2.rectangle(frame, p1, p2, (255, 0, 0), 1, 1)
                    align = str(((bbox[0] + (bbox[2])/2)-self.w/2)*1.3/self.w) + \
                        "," + str(((bbox[1] + (bbox[3])/2) -
                                  self.h/2)*1.3/self.h)
                    cv2.line(frame, (self.w//2, self.h//2),
                             (bbox[0] + (bbox[2])//2, bbox[1] + (bbox[3])//2), (255, 0, 0), 2)
                    self.mqtt_cli.publish(im_topic, align)
                    x = str(((bbox[0] + (bbox[2])/2)-self.w/2)*1.3/self.w)
                    y = str(((bbox[1] + (bbox[3])/2) -
                             self.h/2)*1.3/self.h)
                    cv2.putText(frame, "X: " + x[:7], (10, self.w - 40),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (112, 0, 0), 1)
                    cv2.putText(frame, "Y: " + y[:7], (10, self.w - 20),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (112, 0, 0), 1)

            self.showFPS(frame)
            self.drawCameraUI(frame)
            self.video_writer.write(frame)
            cv2.imshow("Frame", frame)

            if cv2.waitKey(1) & 0XFF == ord('q'):
                break

    def drawCameraUI(self, frame):

        cv2.putText(frame, "Targets:", (self.h - 120, self.w - 85),
                                cv2.FONT_HERSHEY_TRIPLEX, .7, (10, 238, 171), 1)

        defaultTargetDateColor = (40,0,0)

        target1DataColor = defaultTargetDateColor
        target2DataColor = defaultTargetDateColor
        target3DataColor = defaultTargetDateColor

        if self.isLockedTargets[0]:
            target1DataColor = (223, 202, 11)
        
        if self.isLockedTargets[1]:
            target2DataColor = (223, 202, 11)

        if self.isLockedTargets[2]:
            target3DataColor = (223, 202, 11)

        cv2.putText(frame, "UAV 1: " + ("Locked" if self.isLockedTargets[0] else "-"), (self.h - 120, self.w - 60),
                                cv2.FONT_HERSHEY_SIMPLEX, .5, target1DataColor, 1)
        cv2.putText(frame, "UAV 2: " + ("Locked" if self.isLockedTargets[1] else "-"), (self.h - 120, self.w - 40),
                                cv2.FONT_HERSHEY_SIMPLEX, .5, target2DataColor, 1)
        cv2.putText(frame, "UAV 3: " + ("Locked" if self.isLockedTargets[2] else "-"), (self.h - 120, self.w - 20),
                                cv2.FONT_HERSHEY_SIMPLEX, .5, target3DataColor, 1)

        #mission status text
        self.drawMissionStatusText(frame)

    def drawMissionStatusText(self, frame):
        text = ""
        color = (255,255,255)
        if self.land:
            text = "Landing"
            color = (8, 7, 7)
        elif self.initObj:
            
            if self.start_time is None:
                self.start_time = datetime.now()
            diff = 10 - ((datetime.now() - self.start_time).seconds)
            
            text = "Tracking"
            color = (146, 255, 149)

            counterText = ""
            counterColor = (40,0,0)

            if diff >= 0:
                counterText = str(diff)
                if diff <= 3:
                    counterColor = (40, 44, 215)
            else:
                self.initObj = None

            if diff <= 1 and diff >= -1:
                    text = "Locked"
                    color = (7, 7, 206) 
            
            cv2.putText(frame, counterText , ((self.h // 2)+55, 40),
                                cv2.FONT_HERSHEY_DUPLEX, 1, counterColor, 1)

        else:
            text = "Searching"
            color = (45, 233, 236)
            self.start_time = datetime.now()
            
        cv2.putText(frame, text , ((self.h // 2)-60, 40),
                                cv2.FONT_HERSHEY_TRIPLEX, .7, color, 1)

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

                ok = self.tracker.init(
                    frame, (self.x_axis, self.y_axis, self.box_width, self.box_height))
                self.initObj = True

    def showFPS(self, frame):
        font = cv2.FONT_HERSHEY_PLAIN
        self.new_frame_time = time.time()
        fps = 1/(self.new_frame_time-self.prev_frame_time)
        self.prev_frame_time = self.new_frame_time
        fps = int(fps)
        fps = str(fps)
        cv2.putText(frame, fps, (7, 20), font, 1,
                    (0, 0, 0), 2, cv2.LINE_AA)

    def landCallback(self, client, userdata, message):
        self.land = True
        self.logger.info("{} | Land Flag is set: {}".format(
            datetime.now(), self.land))

    def processImageCallback(self, client, userdata, message):
        self.process_image = not self.process_image
        self.logger.info("{} | Process Image Flag is set: {}".format(
            datetime.now(), self.process_image))

    def lockTargetIdCallback(self, client, userdata, message):
        targetId = str(message.payload.decode("utf-8"))
        self.isLockedTargets[int(targetId) - 1] = True

    def setCallbacks(self):
        self.mqtt_cli.subscribe("raptor/land", self.landCallback)
        self.mqtt_cli.subscribe("raptor/processImage",
                                self.processImageCallback)
        self.mqtt_cli.subscribe("raptor/lock", self.lockTargetIdCallback)
