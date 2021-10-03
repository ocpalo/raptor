package main

import (
	"fmt"
	"os"
	t "time"
	"uav_client/src/common"
	"uav_client/src/get"
	"uav_client/src/post"
	"uav_client/src/proxy"

	"github.com/go-co-op/gocron"
)

func print(i interface{}, str string) {
	fmt.Println(str, i)
}

const debug = 1

func main() {
	// TODO:: Testing purpose only, do not forget to remove before merging into main
	TestBuildLockInfo()
	TestBuildTelemetryRequest()

	status := post.Post(common.PostSendTelemetry, &common.TelemReq, &common.TelemReq)
	fmt.Println("Telemetri", status, common.TelemReq)
	//var time ServerTime
	// TODO:: test if empty struct creates a problem during decoding response body into struct
	status = get.Get(common.GetApiLogout, common.LogOut{})
	fmt.Println("Logout", status)
	status = post.Post(common.PostLockInfo, &common.LockInfo, common.LogOut{})
	fmt.Println("Kilitlenme", status)
	status = post.Post(common.PostApiLogin, &common.LoginInfo, common.LogOut{})
	fmt.Println("Login", status)

	scheduler := gocron.NewScheduler(t.UTC)

	/*
		scheduler.Every(500).Millisecond().Do(get.Get, getServerTime, &time)
		scheduler.Every(500).Millisecond().Do(post.Post, postSendTelemetry, &telemetry, &telemetryResp)
		if debug == 1 {
			scheduler.Every(501).Millisecond().Do(print, &time, "Server Time:")
			scheduler.Every(501).Millisecond().Do(print, &telemetryResp, "Telemetry Response:")
		}
	*/
	scheduler.Every(1).Second().Do(proxy.Task, proxy.FutLanded)

	scheduler.StartAsync()

	select {
	case <-proxy.FutLanded:
		status := get.Get(common.GetApiLogout, common.LogOut{})
		if status != common.StatusSuccess {
			proxy.ForceLogout()
		}
		os.Exit(0)
	}
}
