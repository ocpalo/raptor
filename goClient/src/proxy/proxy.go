package proxy

import (
	"log"
	t "time"
	"uav_client/src/common"
	"uav_client/src/http/post"
)

var connectedToServer bool = false

func connectToServer() (valid int) {
	status := post.Post(common.PostApiLogin, &common.LoginInfo, common.LogOut{})
	// try to log in. At worst, spends 300 millisecond. If no connection established, returns early
	// and wait for next scheduled time (for now, default scheduled time is 500ms)
	if status != common.StatusSuccess {
		connectedToServer = false
		for i := 0; i < 2; i++ {
			t.Sleep(t.Millisecond * 100)
			status = post.Post(common.PostApiLogin, &common.LoginInfo, common.LogOut{})
			if status == common.StatusSuccess {
				connectedToServer = true
				return common.StatusSuccess
			}
		}
		// if log in failed, early return
		if status != common.StatusSuccess {
			return 0
		}
	}
	return common.StatusSuccess
}

func HandleStatus(status int) bool {
	if status == common.StatusLoginRequired {
		connectedToServer = false
		return false
	} else if status == common.StatusInvalidRequest ||
		status == common.StatusUnauthorizedAccess ||
		status == common.StatusInvalidUrl ||
		status == common.StatusInternalError {
		log.Println("HTTP Status:", status)
		return false
	} else {
		// TODO:: send telemetry response information to autonomous node and process
		//		  or process here then send processed information to autonomous node
		return true
	}
}

func Task() {
	if !connectedToServer {
		status := connectToServer()
		// try to log in. At worst, spends 300 millisecond. If no connection established, returns early
		// and wait for next scheduled time (for now, default scheduled time is 500ms)
		if status != common.StatusSuccess {
			log.Println("Connection to server failed")
			return
		}
	}
}
