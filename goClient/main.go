package main

import (
	"fmt"
	"log"
	"os"
	t "time"
	"uav_client/src/get"
	"uav_client/src/post"

	"github.com/go-co-op/gocron"
)

const (
	commonId          = "http://localhost:8080"
	getServerTime     = commonId + "/api/sunucusaati"
	getApiLogout      = commonId + "/api/cikis"
	postSendTelemetry = commonId + "/api/telemetri_gonder"
	postLockInfo      = commonId + "/api/kilitlenme_bilgisi"
	postApiLogin      = commonId + "/api/giris"
)

/*
200: İstek başarılı
 204: Gönderilen paketin Formatı Yanlış
 400: İstek hatalı veya geçersiz. Böyle bir durumda hata kodu sayfa içeriği olarak gönderilir.
Hata kodlarının açıklamaları Hata kodları başlığında bulunmaktadır.
 401: Kimliksiz erişim denemesi. Oturum açmanız gerekmektedir.
 403: Yetkisiz erişim denemesi. Yönetici yetkilerine sahip olmayan bir hesap ile
yöneticilere özel bağlantılara giriş yapılmaya çalışmaktadır.
 404: Geçersiz URL.
500: Sunucu içi hata.
*/
const (
	StatusSuccess            = 200
	StatusInvalidRequest     = 400
	StatusLoginRequired      = 401
	StatusUnauthorizedAccess = 403
	StatusInvalidUrl         = 404
	StatusInternalError      = 500
)

func print(i interface{}, str string) {
	fmt.Println(str, i)
}

const debug = 1

var connectedToServer bool = false
var landed bool = true
var futureLanded = make(chan bool)

func connectToServer() (valid int) {
	status := post.Post(postApiLogin, &loginInfo, LogOut{})
	// try to log in. At worst, spends 300 millisecond. If no connection established, returns early
	// and wait for next scheduled time (for now, default scheduled time is 500ms)
	if status != StatusSuccess {
		connectedToServer = false
		for i := 0; i < 2; i++ {
			t.Sleep(t.Millisecond * 100)
			status = post.Post(postApiLogin, &loginInfo, LogOut{})
			if status == StatusSuccess {
				connectedToServer = true
				return StatusSuccess
			}
		}
		// if log in failed, early return
		if status != StatusSuccess {
			return 0
		}
	}
	return StatusSuccess
}

func handleStatus(status int) bool {
	if status == StatusLoginRequired {
		return false
	} else if status == StatusInvalidRequest ||
		status == StatusUnauthorizedAccess ||
		status == StatusInvalidUrl ||
		status == StatusInternalError {
		log.Println("HTTP Status:", status)
		return false
	} else {
		// TODO:: send telemetry response information to autonomous node and process
		//		  or process here then send processed information to autonomous node
		return true
	}
}

func forceLogout() {
	// After UAV sends Landed signal, we need to shut down this process and send GET method to server
	// If status from server is not SUCCESS, until we get a SUCCESS response we will send querys 2 times in a second
	for {
		status := get.Get(getApiLogout, LogOut{})
		if status == StatusSuccess {
			break
		}
		log.Println("Forcing to logout")
		t.Sleep(t.Millisecond * 500)
	}
}

func task(futureLanded chan bool) {
	if !connectedToServer {
		status := connectToServer()
		// try to log in. At worst, spends 300 millisecond. If no connection established, returns early
		// and wait for next scheduled time (for now, default scheduled time is 500ms)
		if status != StatusSuccess {
			log.Println("Connection to server failed")
			return
		}
	}

	// TODO:: read from UNIX Socket, then send appropiate POST or GET method
	// 		  Suggestion : Reading from Autonomous socket might return integer like 1,2,3
	//  				   Using this integer, can send appropiate POST,GET using switch, case
	status := post.Post(postSendTelemetry, &telemetry, &telemetryResp)
	handled := handleStatus(status)
	if !handled {
		// TODO:: Decide what happens if response from POST method is not SUCCESS
		return
	}

	// TODO:: modify landed variable when UAV landed, ipc needed
	// if landed, stop executing the process. when futureLanded set true, it terminates the program
	if landed {
		futureLanded <- true
		return
	}
}

func main() {
	// TODO:: Testing purpose only, do not forget to remove before merging into main
	TestBuildLockInfo()
	TestBuildTelemetryRequest()

	status := post.Post(postSendTelemetry, &telemetry, &telemetryResp)
	fmt.Println("Telemetri", status, telemetryResp)
	//var time ServerTime
	// TODO:: test if empty struct creates a problem during decoding response body into struct
	status = get.Get(getApiLogout, LogOut{})
	fmt.Println("Logout", status)
	status = post.Post(postLockInfo, &lockInfo, LogOut{})
	fmt.Println("Kilitlenme", status)
	status = post.Post(postApiLogin, &loginInfo, LogOut{})
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
	scheduler.Every(1).Second().Do(task, futureLanded)

	scheduler.StartAsync()

	select {
	case <-futureLanded:
		status := get.Get(getApiLogout, LogOut{})
		if status != StatusSuccess {
			forceLogout()
		}
		os.Exit(0)
	}
}
