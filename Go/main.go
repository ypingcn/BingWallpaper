package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"path"
	"time"

	notify "github.com/ypingcn/BingWallpaper/Go/notify"
	setter "github.com/ypingcn/BingWallpaper/Go/setter"
	util "github.com/ypingcn/BingWallpaper/Go/util"
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

var optAuto = flag.Bool("auto", false, "auto detect desktop environment(Beta)")
var optRandom = flag.Bool("random", false, "random pick image from default folder to set wallpaper")
var optSilent = flag.Bool("silent", false, "do not send notify after finish setting")
var optDesktop = flag.String("d", "", "desktop environment: xfce etc, Linux only")
var optCommand = flag.String("c", "", "command in your device to set desktop background,%s will be replaced with the true images path(not end with )")

func main() {
	flag.Parse()

	baseURLs := []string{"https://www.bing.com", "https://www2.bing.com", "https://www4.bing.com", "https://cn.bing.com"}
	apiPath := "/HPImageArchive.aspx?format=js&idx=0&n=1"

	downloadFinished := false
	imgName := ""
	imgPath := path.Join(util.GetUserHomePath(), "BingWallpaper")
	notifyTitle := ""
	notifyName := ""

	if *optRandom {
		rand.Seed(time.Now().UnixNano())
		files, err := ioutil.ReadDir(imgPath)
		if err != nil {
			log.Fatal(err)
		}
		imgName = files[rand.Int()%len(files)].Name()
		notifyTitle = "RANDOM"
		notifyName = imgName
	} else {
		for _, baseURL := range baseURLs {
			apiURL := baseURL + apiPath
			config, err := util.DownloadConfig(apiURL)
			if err != nil {
				log.Println("get config error for ", apiURL, " try next one")
				continue
			}

			bingConfig := &BingConfig{}
			err = json.Unmarshal([]byte(config), bingConfig)
			if err != nil {
				log.Println("json unmarshal error for ", apiURL)
				continue
			}
			notifyTitle = "NEWEST"
			notifyName = bingConfig.Images[0].Copyright

			fileName, err := util.GetValueFromURLArgument(bingConfig.Images[0].URL, "id")

			now := time.Now().Format("2006-01-02")
			if err != nil {
				imgName = fmt.Sprintf("BingWallpaper-%s.jpg", now)
			} else {
				imgName = fmt.Sprintf("BingWallpaper-%s-%s.jpg", now, fileName)
			}

			if err := util.DownloadFile(baseURL+bingConfig.Images[0].URL, imgPath, imgName); err != nil {
				log.Println("download file error ", baseURL+bingConfig.Images[0].URL)
				log.Println(err)
				continue
			}

			downloadFinished = true

			break
		}

		if !downloadFinished {
			log.Fatal("error")
			return
		}
	}

	localSetter, err := setter.New(*optDesktop, *optCommand)

	if err != nil {
		log.Fatal(err)
		return
	}

	localSetter.SetWallpaper(path.Join(imgPath, imgName))

	if !*optSilent {
		notify.Notify("Bingwallpaper", notifyTitle, notifyName, "")
	}
}
