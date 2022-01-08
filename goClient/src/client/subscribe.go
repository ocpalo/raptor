package client

import (
	"fmt"
	"log"

	mqtt "github.com/eclipse/paho.mqtt.golang"
)

type Client struct {
	port              int
	host              string
	subscribed_topics []string
	client            mqtt.Client
}

var f mqtt.MessageHandler = func(client mqtt.Client, msg mqtt.Message) {
	fmt.Printf("TOPIC: %s\n", msg.Topic())
	fmt.Printf("MSG: %s\n", msg.Payload())
}

func contains(slice []string, value string) bool {
	for _, n := range slice {
		if value == n {
			return true
		}
	}
	return false
}

func (c *Client) Init(port int, host string) {
	c.host = host
	c.port = port
	opts := mqtt.NewClientOptions().AddBroker(fmt.Sprintf("tcp://%s:%d", host, port))
	opts.SetClientID("temp")
	opts.SetDefaultPublishHandler(f)
	c.client = mqtt.NewClient(opts)
	if token := c.client.Connect(); token.Wait() && token.Error() != nil {
		panic(token.Error())
	}
}

func (c *Client) Subscribe(topic string) {
	if token := c.client.Subscribe(topic, 1, f); token.Wait() && token.Error() != nil {
		log.Fatalln(token.Error())
		return
	}
	c.subscribed_topics = append(c.subscribed_topics, topic)
}

func (c *Client) Publish(topic, message string) {
	token := c.client.Publish(topic, 1, false, message)
	token.Wait()
}

func (c *Client) Unsubscribe(topic string) {
	if contains := contains(c.subscribed_topics, topic); !contains {
		log.Println("Topic is not in subscribed topic list, returning from function")
		return
	}
	if token := c.client.Unsubscribe(topic); token.Wait() && token.Error() != nil {
		log.Fatalln(token.Error())
		return
	}
}

func (c *Client) Disconnect() {
	c.client.Disconnect(250)
}
