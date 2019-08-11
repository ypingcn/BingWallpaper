package notify

import (
	"fmt"
	"os/exec"
)

// display a notification for linux
func Notify(appName, title, content, iconPath string) {
	notify(appName, title, content, iconPath)
}

func notify(appName, title, content, iconPath string) {
	command := exec.Command("notify-send", "-i", iconPath, fmt.Sprintf("%s - %s", appName, title), content)
	command.Run()
}
