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
	status := post.Post(common.PostSendTelemetry, &common.TelemReq, &common.TelemReq)
	fmt.Println("Telemetri", status, common.TelemReq)
	status = get.Get(common.GetApiLogout, common.LogOut{})
	fmt.Println("Logout", status)
	status = post.Post(common.PostLockInfo, &common.LockInfo, common.LogOut{})
	fmt.Println("Kilitlenme", status)
	status = post.Post(common.PostApiLogin, &common.LoginInfo, common.LogOut{})
	fmt.Println("Login", status)

	scheduler := gocron.NewScheduler(t.UTC)
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
