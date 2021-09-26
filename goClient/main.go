package main

import (
	"fmt"
	_ "github.com/go-co-op/gocron"
	"uav_client/src/get"
	"uav_client/src/post"
)

const (
	commonId = "localhost"
	getServerTime = "/api/sunucusaati"
	getApiLogout = "/api/cikis"
	postSendTelemetry = "/api/telemetri_gonder"
	postLockInfo = "/api/kilitlenme_bilgisi"
	postApiLogin = "/api/giris"
)

func main() {
	resp, err := getApiServerTime()
	fmt.Println(resp, err)
	status := post.Post(postSendTelemetry, &telemetry, &telemetryResp)
	fmt.Println(status, telemetryResp)
	var time ServerTime
	status = get.Get(getServerTime, &time)
	fmt.Println(status, time)
	// TODO:: test if empty struct creates a problem during decoding response body into struct
	status = get.Get(getApiLogout, LogOut{})
	fmt.Println(status)
}
