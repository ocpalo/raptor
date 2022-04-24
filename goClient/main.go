package main

import (
	"fmt"
	"os"
	t "time"
	"uav_client/src/common"
	"uav_client/src/http/post"
	client "uav_client/src/mqtt"
	"uav_client/src/proxy"

	"github.com/go-co-op/gocron"
)

func main() {
	var cli client.Client
	cli.Init(1883, "127.0.0.1")

	/*
		status := post.Post(common.PostSendTelemetry, &common.TelemReq, &common.TelemReq)
		fmt.Println("Telemetri", status, common.TelemReq)
		status = get.Get(common.GetApiLogout, common.LogOut{})
		fmt.Println("Logout", status)
		status = post.Post(common.PostLockInfo, &common.LockInfo, common.LogOut{})
		fmt.Println("Kilitlenme", status)
	*/
	status := post.Post(common.PostApiLogin, &common.LoginInfo, common.LogOut{})
	fmt.Println("Login", status)

	scheduler := gocron.NewScheduler(t.UTC)
	scheduler.Every(3).Second().Do(proxy.Task)
	cli.Subscribe("raptor/telemetry")
	cli.Subscribe("raptor/land")
	cli.Subscribe("raptor/lock")
	cli.Subscribe("target/telemetry")
	cli.Subscribe(("raptor/lock_info"))

	scheduler.StartAsync()

	select {
	case <-client.FutLanded:
		os.Exit(0)
	}
}
