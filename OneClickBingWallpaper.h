#ifndef ONECLICKGO_H
#define ONECLICKGO_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QFile>
#include <QMessageBox>
#include <QAction>
#include <QMenu>

#define PYFILE_MD5_CHECK

class OneClickBingWallpaper : public QWidget
{
    Q_OBJECT

public:
    OneClickBingWallpaper(QWidget *parent = 0);
    ~OneClickBingWallpaper();
private:
    QSystemTrayIcon * trayIcon;
    QMenu * trayMenu, * moreMenu;
    QAction * autoAction;
    QAction * cinnamonAction;
    QAction * deepinAction;
    QAction * gnomeAction;
    QAction * kdeAction;
    QAction * mateAction;
    QAction * wmAction;
    QAction * xfceAction;
    QAction * quitAction;

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void updateWallpaper();
};

#endif // ONECLICKGO_H
