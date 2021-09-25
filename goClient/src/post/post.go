package post

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
)

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

func decodeHttpStatus(response *http.Response) int {
	return response.StatusCode
}

func decodeResponseBody(response *http.Response, str interface{}) error{
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

	status := decodeHttpStatus(post)
	err = decodeResponseBody(post, &apiResponse)
	if err != nil {
		return 0
	}
	return status
}
