package setter

import (
	"fmt"
	"os"
	"os/exec"
	"strings"

	util "github.com/ypingcn/BingWallpaper/Go/util"
)

// WallpaperSetter ...
type WallpaperSetter interface {
	SetWallpaper(string) error
}

// CinnamonSetter : for Cinnamon
type CinnamonSetter struct{}

// DeepinSetter : for Deepin
type DeepinSetter struct{}

// GnomeSetter : for Gnome
type GnomeSetter struct{}

// KDESetter : for KDE
type KDESetter struct{}

// MateSetter : for Mate
type MateSetter struct{}

// WMSetter : for WM
type WMSetter struct{}

// XfceSetter : for Xfce
type XfceSetter struct{}

// CommonSetter : for Common
type CommonSetter struct {
	command string
}

// NewSetter ...
func New(desktop, command string) (WallpaperSetter, error) {
	if desktop == "" {
		session := os.Getenv("DESKTOP_SESSION")
		keywords := map[string]string{
			"cinnamon": "cinnamon",
			"deepin":   "deepin",
			"gnome":    "gnome",
			"mate":     "mate",
			"plasma":   "kde",
			"xfce":     "xfce",
		}
		for val := range keywords {
			if strings.Compare(val, session) == 0 {
				desktop = val
				break
			}
		}
	}
	switch desktop {
	case "cinnamon":
		return CinnamonSetter{}, nil
	case "deepin":
		return DeepinSetter{}, nil
	case "gnome":
		return GnomeSetter{}, nil
	case "kde":
		return KDESetter{}, nil
	case "mate":
		return MateSetter{}, nil
	case "wm":
		return WMSetter{}, nil
	case "xfce":
		return XfceSetter{}, nil
	}
	if command != "" {
		return CommonSetter{command: command}, nil
	}
	return nil, fmt.Errorf("not supported desktop environment for %s with %s command", desktop, command)
}

// SetWallpaper ...
func (setter CinnamonSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return err
	}

	command := fmt.Sprintf("gsettings set org.cinnamon.desktop.background picture-uri \"file:///%s\"", imgPath)

	_, err := exec.Command("sh", "-c", command).Output()
	if err != nil {
		return err
	}
	return nil
}

// SetWallpaper ...
func (setter DeepinSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return err
	}

	command := fmt.Sprintf("gsettings set com.deepin.wrap.gnome.desktop.background picture-uri \"file:///%s\"", imgPath)
	_, err := exec.Command("sh", "-c", command).Output()
	if err != nil {
		return err
	}
	return nil
}

// SetWallpaper ...
func (setter GnomeSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return nil
	}

	command := fmt.Sprintf(`
		gsettings set org.gnome.desktop.background draw-background false && 
		gsettings set org.gnome.desktop.background picture-uri file://%s && 
		gsettings set org.gnome.desktop.background draw-background true
	`, imgPath)
	_, err := exec.Command("sh", "c", command).Output()
	if err != nil {
		return nil
	}
	return nil
}

// SetWallpaper ...
func (setter KDESetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return nil
	}

	command := fmt.Sprintf(`
		var Desktops = desktops();
		d = Desktops[0];
		d.wallpaperPlugin= "org.kde.image";
		d.currentConfigGroup = Array("Wallpaper","org.kde.image","General");
		d.writeConfig("Image","%s");
	`, imgPath)
	_, err := exec.Command("sh", "-c", command).Output()
	if err != nil {
		return err
	}
	return nil
}

// SetWallpaper ...
func (setter MateSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return err
	}
	command := fmt.Sprintf("gsettings set org.mate.background picture-filename %s", imgPath)
	_, err := exec.Command("sh", "-c", command).Output()
	if err != nil {
		return err
	}
	return nil
}

// SetWallpaper ...
func (setter WMSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return err
	}
	command := fmt.Sprintf("feh --bg-fill %s", imgPath)
	_, err := exec.Command("sh", "-c", command).Output()
	if err != nil {
		return err
	}
	return nil
}

// SetWallpaper ...
func (setter XfceSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return err
	}
	command := fmt.Sprintf("xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/workspace0/last-image -s %s", imgPath)
	_, err := exec.Command("sh", "-c", command).Output()
	if err != nil {
		return err
	}
	return nil
}

// SetWallpaper ...
func (setter CommonSetter) SetWallpaper(imgPath string) error {
	if err := util.CheckImagePath(imgPath); err != nil {
		return err
	}
	command := fmt.Sprintf(setter.command, imgPath)
	_, err := exec.Command("sh", "-c", command).Output()
	if err != nil {
		return err
	}
	return nil
}
