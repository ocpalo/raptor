package proxy

import (
	"log"
	t "time"
	"uav_client/src/common"
	"uav_client/src/get"
	"uav_client/src/post"
)

var FutLanded = make(chan bool)
var landed bool = false
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

func handleStatus(status int) bool {
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

func ForceLogout() {
	// After UAV sends Landed signal, we need to shut down this process and send GET method to server
	// If status from server is not SUCCESS, until we get a SUCCESS response we will send query 1 times in a second
	for i := 0; i < 10; i++ {
		status := get.Get(common.GetApiLogout, common.LogOut{})
		if status == common.StatusSuccess {
			break
		}
		log.Println("Forcing to logout")
		t.Sleep(t.Millisecond * 1000)
	}
}

func Task(futureLanded chan bool) {
	if !connectedToServer {
		status := connectToServer()
		// try to log in. At worst, spends 300 millisecond. If no connection established, returns early
		// and wait for next scheduled time (for now, default scheduled time is 500ms)
		if status != common.StatusSuccess {
			log.Println("Connection to server failed")
			return
		}
	}

	// TODO:: read from UNIX Socket, then send appropiate POST or GET method
	// 		  Suggestion : Reading from Autonomous socket might return integer like 1,2,3
	//  				   Using this integer, can send appropiate POST,GET using switch, case
	status := post.Post(common.PostSendTelemetry, &common.TelemReq, &common.TelemResp)
	handled := handleStatus(status)
	log.Println(status, handled)
	if !handled {
		// TODO:: Decide what happens if response from POST method is not SUCCESS
		return
	}

	// TODO:: modify landed variable when UAV landed, ipc needed
	// if landed, stop executing the process. when futureLanded set true, it terminates the program
	if landed {
		FutLanded <- true
		return
	}
}
