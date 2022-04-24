# HTTP Client for UAV project written in Go

### MQTT client-side for Raptor written in GO

##### Libraries and Other Tools Versions

* mosquitto version 1.6.9
	* sudo apt install mosquitto
* paho mqtt golang
	* go get github.com/eclipse/paho.mqtt.golang
	* go get github.com/gorilla/websocket
	* go get golang.org/x/net/proxy

##### Run

- Terminal 1
	+ mosquitto
- Terminal 2 (To watch all published topics)
	+ mosquitto_sub -h localhost -u user -P pass -v -t '#'
- Terminal 3
	+ go run main.go
