package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	_ "github.com/go-co-op/gocron"
	"io"
	"net/http"
)

func getApiServerTime() (time ServerTime, err error) {
	resp, err := http.Get("")
	if err != nil {
		return ServerTime{}, err
	}
	defer func(Body io.ReadCloser) {
		err := Body.Close()
		if err != nil {
			fmt.Println(err)
		}
	}(resp.Body)

	var serverTime ServerTime
	err = json.NewDecoder(resp.Body).Decode(&serverTime)
	if err != nil {
		return ServerTime{}, err
	}

	return serverTime, err
}

// DEPRECATED
func postApiLogin() (int , error) {
	body, err := json.Marshal(loginInfo)
	if err != nil {
		return 0, err
	}

	resp, err := http.Post("", "", bytes.NewBuffer(body))
	if err != nil {
		return 0, err
	}
	defer func(Body io.ReadCloser) {
		err := Body.Close()
		if err != nil {
			fmt.Println(err)
		}
	}(resp.Body)

	return resp.StatusCode, err
}

// DEPRECATED
func postApiLockInfo() (int, error) {
	body, err := json.Marshal(lockInfo)
	if err != nil {
		return 0, err
	}

	resp, err := http.Post("", "", bytes.NewBuffer(body))
	if err != nil {
		return 0, err
	}
	defer func(Body io.ReadCloser) {
		err := Body.Close()
		if err != nil {
			fmt.Println(err)
		}
	}(resp.Body)

	return resp.StatusCode, err
}

func postApiTelemetry() (TelemetryResponse ,error) {
	body, err := json.Marshal(telemetry)
	if err != nil {
		return TelemetryResponse{}, err
	}

	resp, err := http.Post("", "", bytes.NewBuffer(body))
	if err != nil {
		return TelemetryResponse{}, err
	}
	defer func(Body io.ReadCloser) {
		err := Body.Close()
		if err != nil {
			fmt.Println(err)
		}
	}(resp.Body)

	err = json.NewDecoder(resp.Body).Decode(&telemetryResp)
	if err != nil {
		return TelemetryResponse{}, err
	}
	return telemetryResp, err
}

// use this instead of postApiLockInfo, postApiLogin
// needs to be tested
func postApi(api interface{}) (int, error) {
	body, err := json.Marshal(api)
	if err != nil {
		return 0, err
	}

	resp, err := http.Post("", "", bytes.NewBuffer(body))
	if err != nil {
		return 0, err
	}
	defer func(Body io.ReadCloser) {
		err := Body.Close()
		if err != nil {
			fmt.Println(err)
		}
	}(resp.Body)

	return resp.StatusCode, err
}


func main() {
	time, err := getApiServerTime()
	fmt.Println(time, err)
	login, err := postApiLogin()
	fmt.Println(login, err)
	lock, err := postApiLockInfo()
	fmt.Println(lock, err)
	login, err = postApi(loginInfo)
	fmt.Println(login, err)
	lock, err = postApi(lockInfo)
	fmt.Println(lock, err)
	telem, err := postApiTelemetry()
	fmt.Println(telem, err)
}
