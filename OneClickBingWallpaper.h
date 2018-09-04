#ifndef ONECLICKGO_H
#define ONECLICKGO_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QFile>
#include <QMessageBox>
#include <QAction>
#include <QMenu>

class OneClickBingWallpaper : public QWidget
{
    Q_OBJECT

public:
    OneClickBingWallpaper(QWidget *parent = 0);
    ~OneClickBingWallpaper();
private:
    QSystemTrayIcon * trayIcon;
    QMenu * trayMenu;
    QAction * cinnamonAction;
    QAction * xfceAction;
    QAction * deepinAction;
    QAction * wmAction;
    QAction * quitAction;

    const QString pyFileMD5 = "fca6c6938c317e94e81517bba74ebfb3";
    const QString pyFilePath = "/usr/bin/OneClickBingWallpaper/BingWallpaper.py";

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void updateWallpaper();
};

#endif // ONECLICKGO_H
