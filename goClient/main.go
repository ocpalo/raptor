package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
)

type ServerTime struct {
	Hour   		int `json:"saat"`
	Minute 		int `json:"dakika"`
	Second      int `json:"saniye"`
	Millisecond int `json:"milisaniye"`
}

type TeamInfo struct {
	Username string `json:"kadi"`
	Passwd   string `json:"sifre"`
}

var loginInfo = TeamInfo{"go", "lang"}

func getApiServerTime() (time ServerTime, err error) {
	resp, err := http.Get("")
	if err != nil {
		return ServerTime{}, err
	}
	defer resp.Body.Close()

	var serverTime ServerTime
	err = json.NewDecoder(resp.Body).Decode(&serverTime)
	if err != nil {
		return ServerTime{}, err
	}

	return serverTime, err
}

func postApiLogin() (int , error) {
	body, err := json.Marshal(loginInfo)
	if err != nil {
		return 0, err
	}

	resp, err := http.Post("", "", bytes.NewBuffer(body))
	if err != nil {
		return 0, err
	}
	defer resp.Body.Close()
	
	return resp.StatusCode, err
}

func main() {
	time, err := getApiServerTime()
	fmt.Println(time, err)
	login, err := postApiLogin()
	fmt.Println(login, err)
}
