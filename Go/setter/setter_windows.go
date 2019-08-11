package setter

import (
	"github.com/reujab/wallpaper"
	util "github.com/ypingcn/BingWallpaper/Go/util"
)

// WallpaperSetter ...
type WallpaperSetter interface {
	SetWallpaper(string) error
}

type WindowsSetter struct{}

// NewSetter ...
func New(desktop, command string) (WallpaperSetter, error) {
	return WindowsSetter{}, nil
}

// SetWallpaper for Windows
func (setter WindowsSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return err
	}
	if err := wallpaper.SetFromFile(imgPath); err != nil {
		return err
	}
	return nil
}
