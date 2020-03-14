package conf

import (
	"encoding/json"
	"io/ioutil"
	"os"
)

// BingImages ...
type BingImages struct {
	Startdate     string        `json:"startdate"`
	Fullstartdate string        `json:"fullstartdate"`
	Enddate       string        `json:"enddate"`
	URL           string        `json:"url"`
	URLbase       string        `json:"urlbase"`
	Copyright     string        `json:"copyright"`
	Copyrightlink string        `json:"copyrightlink"`
	Title         string        `json:"title"`
	Quiz          string        `json:"quiz"`
	Wp            bool          `json:"wp"`
	Hsh           string        `json:"hsh"`
	Drk           int           `json:"drk"`
	Top           int           `json:"top"`
	Bot           int           `json:"bot"`
	Hs            []interface{} `json:"hs,omitempty"`
}

// BingTooltips ...
type BingTooltips struct {
	Loading  string `json:"loading"`
	Previous string `json:"previous"`
	Next     string `json:"next"`
	Walle    string `json:"walle"`
	Walls    string `json:"walls"`
}

// BingConfig ...
type BingConfig struct {
	Images   []*BingImages `json:"images"`
	Tooltips *BingTooltips `json:"tooltips"`
}

// UpdateWallpaperConfig ...
type UpdateWallpaperConfig struct {
	AutoDetect     bool   `json:"autoDetect,omitempty"`
	Silent         bool   `json:"silent,omitempty"`
	Random         bool   `json:"random,omitempty"`
	UpdateDuration int64  `json:"duration,omitempty"`
	Path           string `json:"path,omitempty"`
	Desktop        string `json:"desktop,omitempty"`
	Command        string `json:"command,omitempty"`
}

// ReadFromFile for UpdateWallpaperConfig
func (config UpdateWallpaperConfig) ReadFromFile(path string) error {
	configFile, err := os.Open(path)
	if err != nil {
		return err
	}
	defer configFile.Close()

	rawConfig, err := ioutil.ReadAll(configFile)
	if err != nil {
		return err
	}

	json.Unmarshal(rawConfig, &config)

	return nil
}
