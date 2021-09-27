package main

import (
	"fmt"
	"github.com/go-co-op/gocron"
	_ "github.com/go-co-op/gocron"
	"os"
	t "time"
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
	StatusSuccess         		= 200
	StatusInvalidRequest       	= 400
	StatusLoginRequired      	= 401
	StatusUnauthorizedAccess 	= 403
	StatusInvalidUrl    		= 404
	StatusInternalError 		= 500
)

func print(i interface{}, str string) {
	fmt.Println(str, i)
}

const debug = 1

var connectedToServer bool = false
var landed bool = false
var futureLanded = make(chan bool)

func task(futureLanded chan bool) {
	if !connectedToServer {
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
					break
				}
			}
			// if log in failed, early return
			if status != StatusSuccess {
				return
			}
		}
	}

	status := post.Post(postSendTelemetry, &telemetry, &telemetryResp)
	if status == StatusLoginRequired {
		return
	} else if status == StatusInvalidRequest ||
			  status == StatusUnauthorizedAccess ||
			  status == StatusInvalidUrl ||
			  status == StatusInternalError {
		fmt.Println("HTTP Status:", status)
	} else {
		// TODO:: send telemetry response information to autonomous node and process
		//		  or process here then send processed information to autonomous node
	}

	// TODO:: modify landed variable when UAV landed, ipc needed
	// if landed, stop executing the process. when futureLanded set true, it terminates the program
	if landed {
		status := get.Get(getApiLogout, LogOut{})
		if status == StatusSuccess {
			futureLanded <- true
			return
		}
	}
}

func main() {
	status := post.Post(postSendTelemetry, &telemetry, &telemetryResp)
	fmt.Println("Telemetri", status, telemetryResp)
	var time ServerTime
	// TODO:: test if empty struct creates a problem during decoding response body into struct
	status = get.Get(getApiLogout, LogOut{})
	fmt.Println("Logout", status)
	status = post.Post(postLockInfo, &lockInfo, LogOut{})
	fmt.Println("Kilitlenme", status)
	status = post.Post(postApiLogin, &loginInfo, LogOut{})
	fmt.Println("Login", status)

	scheduler := gocron.NewScheduler(t.UTC)

	scheduler.Every(500).Millisecond().Do(get.Get, getServerTime, &time)
	scheduler.Every(500).Millisecond().Do(post.Post, postSendTelemetry, &telemetry, &telemetryResp)
	if debug == 1 {
		scheduler.Every(501).Millisecond().Do(print, &time, "Server Time:")
		scheduler.Every(501).Millisecond().Do(print, &telemetryResp, "Telemetry Response:")
	}
	scheduler.Every(1).Second().Do(task, futureLanded)

	scheduler.StartAsync()

	select {
	case <-futureLanded:
		os.Exit(0)
	}

}
