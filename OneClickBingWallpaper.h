#ifndef ONECLICKBINGWALLPAPER_H
#define ONECLICKBINGWALLPAPER_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QFile>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <qsettingbackend.h>

#include <DSettingsDialog>
#include <DSettings>
#include <DSettingsOption>

DCORE_BEGIN_NAMESPACE
class DSettings;
DCORE_END_NAMESPACE

DCORE_USE_NAMESPACE

#define PYFILE_MD5_CHECK

class OneClickBingWallpaper : public QWidget
{
    Q_OBJECT

public:
    OneClickBingWallpaper(QWidget *parent = 0);
    ~OneClickBingWallpaper();
private:
    QSystemTrayIcon * trayIcon;
    QMenu * trayMenu, * moreMenu, * langMenu;

    QAction * autoAction;
    QAction * cinnamonAction;
    QAction * deepinAction;
    QAction * gnomeAction;
    QAction * kdeAction;
    QAction * mateAction;
    QAction * wmAction;
    QAction * xfceAction;

    QAction * zhAction;
    QAction * enAction;

    QAction * settingAction;
    QAction * aboutAction;

    QAction * quitAction;

    QString configPath;
    Dtk::Core::QSettingBackend * backend;
    DSettings * dsettings;

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void updateWallpaper();
    void updateLanguage();

    void showSettingWidget();
    void showAboutWidget();
};

#endif // ONECLICKBINGWALLPAPER_H
