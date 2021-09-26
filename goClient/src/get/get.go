package get

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
)


func get() (*http.Response, error) {
	resp, err := http.Get("")
	if err != nil {
		return nil, err
	}

	return resp, err
}

func decodeHttpStatus(response *http.Response) int {
	return response.StatusCode
}

func decodeResponseBody(response *http.Response, str interface{}) error {
	err := json.NewDecoder(response.Body).Decode(&str)
	if err != nil {
		return err
	}
	return nil
}

func Get(i interface{}) int {
	resp, err := get()
	if err != nil {
		return 0
	}
	defer func(Body io.ReadCloser) {
		err := Body.Close()
		if err != nil {
			fmt.Println("HANDLE ME", err)
		}
	}(resp.Body)

	err = decodeResponseBody(resp, &i)
	if err != nil {
		return 0
	}
	status := decodeHttpStatus(resp)
	return status
}