package main

import (
	"fmt"
	_ "github.com/go-co-op/gocron"
	 "uav_client/src/post"
)


func main() {
	resp, err := getApiServerTime()
	fmt.Println(resp, err)
	status := post.Post(&telemetry, &telemetryResp)
	fmt.Println(status, telemetryResp)
}
