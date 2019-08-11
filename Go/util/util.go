package util

import (
	"errors"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"os"
	"os/user"
	"path"
)

// CheckImagePath check path for image
func CheckImagePath(imgPath string) error {
	if imgPath == "" {
		return errors.New("empty path")
	}

	if _, err := os.Stat(imgPath); os.IsNotExist(err) {
		return errors.New("image path is not exist for" + imgPath)
	}
	return nil
}

// DownloadConfig get config json from specific url
func DownloadConfig(url string) ([]byte, error) {
	resp, err := http.Get(url)
	if err != nil {
		return []byte{}, err
	}
	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return []byte{}, err
	}

	return body, nil
}

// DownloadFile get file from url to filePath with fileName
func DownloadFile(url, filePath, fileName string) error {
	resp, err := http.Get(url)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	if _, err := os.Stat(filePath); os.IsNotExist(err) {
		os.MkdirAll(filePath, os.ModePerm)
	}

	localfile, err := os.Create(path.Join(filePath, fileName))
	if err != nil {
		return err
	}
	defer localfile.Close()

	_, err = io.Copy(localfile, resp.Body)
	return err
}

// GetValueFromURLArgument get value from a URL argument
func GetValueFromURLArgument(rawURL, valueName string) (string, error) {
	if len(rawURL) == 0 {
		return "", errors.New("empty string")
	}

	result, err := url.Parse(rawURL)
	if err != nil {
		return "", err
	}

	values, err := url.ParseQuery(result.RawQuery)
	if err != nil {
		return "", err
	}

	return values.Get(valueName), nil
}

// GetUserHomePath get user home path
func GetUserHomePath() string {
	usr, err := user.Current()
	if err != nil {
		log.Fatal(err)
		return ""
	}
	return usr.HomeDir
}
