import cv2

# Before running, Set QGroundControl port to 5601
cap = cv2.VideoCapture("udpsrc port=5600 ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=H264,framerate=30/1 ! rtph264depay ! avdec_h264 ! videoconvert ! appsink",cv2.CAP_GSTREAMER)
if not cap.isOpened() :
	print('VideoCapture not opened')
	exit(-1)
	
w=int(cap.get(cv2.CAP_PROP_FRAME_WIDTH ))
h=int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT ))
fourcc = cv2.VideoWriter_fourcc(*'XVID')
video_writer = cv2.VideoWriter("output.avi", fourcc, 25, (w, h))

while True:
	ret, frame = cap.read()
	if not ret:
		print('frame empty')
		break

	video_writer.write(frame)
	cv2.imshow('image', frame)

	if cv2.waitKey(1)&0XFF == ord('q'):
		break

cap.release()
cv2.destroyAllWindows()
