package main

import (
	"bytes"
	"encoding/json"
	"fmt"
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

// TODO::remove error status in the future, nil check is sufficient
func post(api interface{}) (*http.Response, error) {
	body, err := json.Marshal(api)
	if err != nil {
		return nil, err
	}

	resp, err := http.Post("", "", bytes.NewBuffer(body))
	if err != nil {
		return nil, err
	}

	return resp, nil
}

func DecodeHttpStatus(response *http.Response) int {
	return response.StatusCode
}

func DecodeResponseBody(response *http.Response, str interface{}) error{
	err := json.NewDecoder(response.Body).Decode(&str)
	if err != nil {
		return err
	}
	return nil
}

func Post(api interface{}, apiResponse interface{}) int {
	post, err := post(&api)
	if err != nil {
		return 0
	}
	defer func(Body io.ReadCloser) {
		err := Body.Close()
		if err != nil {
			fmt.Println("HANDLE ME")
		}
	}(post.Body)

	status := DecodeHttpStatus(post)
	err = DecodeResponseBody(post, &apiResponse)
	if err != nil {
		return 0
	}
	return status
}
