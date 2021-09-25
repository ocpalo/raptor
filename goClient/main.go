package main

import (
	"fmt"
	_ "github.com/go-co-op/gocron"
)


func main() {
	resp, err := getApiServerTime()
	fmt.Println(resp, err)
	var telemetryResponse TelemetryResponse
	status := Post(&telemetry, &telemetryResponse)
	fmt.Println(status, telemetryResponse)
}
