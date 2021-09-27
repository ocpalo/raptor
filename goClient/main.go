package main

import (
	"fmt"
	_ "github.com/go-co-op/gocron"
	"uav_client/src/get"
	"uav_client/src/post"
)

const (
	commonId = "http://localhost:8080"
	getServerTime = commonId + "/api/sunucusaati"
	getApiLogout = commonId + "/api/cikis"
	postSendTelemetry = commonId + "/api/telemetri_gonder"
	postLockInfo = commonId + "/api/kilitlenme_bilgisi"
	postApiLogin = commonId + "/api/giris"
)

func main() {
	status := post.Post(postSendTelemetry, &telemetry, &telemetryResp)
	fmt.Println("Telemetri", status, telemetryResp)
	var time ServerTime
	status = get.Get(getServerTime, &time)
	fmt.Println("Time", status, time)
	// TODO:: test if empty struct creates a problem during decoding response body into struct
	status = get.Get(getApiLogout, LogOut{})
	fmt.Println("Logout", status)
	status = post.Post(postLockInfo, &lockInfo, LogOut{})
	fmt.Println("Kilitlenme", status)
	status = post.Post(postApiLogin, &loginInfo, LogOut{})
	fmt.Println("Login", status)
}
