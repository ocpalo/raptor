package client

import (
	"log"
	"time"
	"uav_client/src/common"
	"uav_client/src/http/get"
)

func forceLogout() {
	for i := 0; i < 10; i++ {
		status := get.Get(common.GetApiLogout, common.LogOut{})
		if status == common.StatusSuccess {
			FutLanded <- true
			break
		}
		log.Println("Forcing to logout")
		time.Sleep(time.Millisecond * 1000)
	}
}

func contains(slice []string, value string) bool {
	for _, n := range slice {
		if value == n {
			return true
		}
	}
	return false
}
