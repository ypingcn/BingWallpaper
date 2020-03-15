package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io/ioutil"
	"math/rand"
	"os"
	"path"
	"time"

	logrus "github.com/sirupsen/logrus"

	"github.com/ypingcn/BingWallpaper/Go/conf"
	"github.com/ypingcn/BingWallpaper/Go/notify"
	"github.com/ypingcn/BingWallpaper/Go/setter"
	"github.com/ypingcn/BingWallpaper/Go/util"
)

const (
	configFileName = "./BingWallpaper.conf"
	logFileName    = "./BingWallpaper.log"
	imgFolderName  = "BingWallpaper"
)

var optConf = flag.String("conf", configFileName, "config file path")
var optDisableConf = flag.Bool("disable-file-conf", false, "disable all file configuration")

var optDuration = flag.Int64("duration", 0, "update duration(seconds)")
var optAuto = flag.Bool("auto", false, "auto detect desktop environment(Beta)")
var optSilent = flag.Bool("silent", false, "do not send notify after finish setting")
var optRandom = flag.Bool("random", false, "random pick image from default folder to set wallpaper")
var optImgPath = flag.String("imgpath", path.Join(util.GetUserHomePath(), imgFolderName), "image folder for random mode")
var optDesktop = flag.String("d", "", "desktop environment: xfce etc, Linux only")
var optCommand = flag.String("c", "", "command in your device to set desktop background,%s will be replaced with the true images path(not end with )")

var log = logrus.New()

func init() {
	logrus.SetLevel(logrus.InfoLevel)
	log.Out = os.Stdout
	file, err := os.OpenFile(logFileName, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0666)
	if err != nil {
		log.Info("Failed to log to file, using default stderr.", err)
	} else {
		log.Out = file
	}
}

func setWallpaper(config conf.UpdateWallpaperConfig) {
	log.Info("setWallpaper begin")

	defer func() {
		log.Info("setWallpaper end")
	}()

	baseURLs := []string{"https://www.bing.com", "https://www2.bing.com", "https://www4.bing.com", "https://cn.bing.com"}
	apiPath := "/HPImageArchive.aspx?format=js&idx=0&n=1"

	downloadFinished := false
	imgName := ""
	imgPath := config.Path
	notifyTitle := ""
	notifyName := ""

	if config.Random {
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

			bingConfig := &conf.BingConfig{}
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

	localSetter, err := setter.New(config.Desktop, config.Command)

	if err != nil {
		log.Fatal(err)
		return
	}

	localSetter.SetWallpaper(path.Join(imgPath, imgName))

	if !config.Silent {
		notify.Notify("Bingwallpaper", notifyTitle, notifyName, "")
	}
}

func main() {
	flag.Parse()

	updateConfig := conf.UpdateWallpaperConfig{
		AutoDetect:     *optAuto,
		Silent:         *optSilent,
		Random:         *optRandom,
		UpdateDuration: *optDuration,
		Path:           *optImgPath,
		Desktop:        *optDesktop,
		Command:        *optCommand,
	}

	if !*optDisableConf {
		if err := updateConfig.ReadFromFile(*optConf); err != nil {
			panic(err)
		}
	}

	timer := time.NewTimer(time.Second * 0)
	for {
		select {
		case <-timer.C:
			setWallpaper(updateConfig)
		}

		if updateConfig.UpdateDuration <= 0 {
			break
		}
		timer.Reset(time.Second * time.Duration(updateConfig.UpdateDuration))

	}
}
