package notify

import (
	toast "gopkg.in/toast.v1"
)

// display a notification for Windows
func Notify(appName, title, content, iconPath string) {
	notify(appName, title, content, iconPath)
}

func notify(appName, title, content, iconPath string) toast.Notification {
	return toast.Notification{
		AppID:   appName,
		Title:   title,
		Message: content,
		Icon:    iconPath,
	}
}
