package main

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
)

// TODO:: Delete after testing src/get/get.go
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
