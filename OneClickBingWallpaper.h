#ifndef ONECLICKBINGWALLPAPER_H
#define ONECLICKBINGWALLPAPER_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QFile>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <qsettingbackend.h>

#include <DSettingsDialog>
#include <DSettings>
#include <DSettingsOption>

DCORE_BEGIN_NAMESPACE
class DSettings;
DCORE_END_NAMESPACE

DCORE_USE_NAMESPACE

struct DesktopEnvironmentType
{
public:
    QString name;
    QString argument;
};

struct LanguageType
{
public:
    QString name;
    QString value;
};

static QVector<DesktopEnvironmentType> vDesktopEnvironments = {
    {QObject::tr("Cinnamon"),"-d cinnamon"}, {QObject::tr("Deepin"),"-d deepin"},
    {QObject::tr("Gnome"),"-d gnome"},{QObject::tr("KDE"),"-d kde"},{QObject::tr("LXQt"),"-d lxqt"},
    {QObject::tr("Mate"),"-d mate"},{QObject::tr("WM"),"-d wm"},{QObject::tr("Xfce"),"-d xfce"}
};

static QVector<LanguageType> vLanguages = {
    {QObject::tr("简体中文"),"zh-CN"}, {QObject::tr("English"),"en-US"}
};

class OneClickBingWallpaper : public QWidget
{
    Q_OBJECT

public:
    OneClickBingWallpaper(QWidget *parent = 0);
    ~OneClickBingWallpaper();
private:
    QSystemTrayIcon * trayIcon;
    QMenu * trayMenu, * moreMenu, * langMenu;

    QAction * updateAction;
    QVector<QAction*> vDEActions;

    QVector<QAction*> vLangActions;

    QAction * settingAction;
    QAction * aboutAction;
    QAction * visitBingAction;

    QAction * quitAction;

    QString configPath;
    Dtk::Core::QSettingBackend * backend;
    DSettings * dsettings;

    QTimer * timer;

    QString imagePath;

    void initAction();
    void initMenu();
    void initConnect();
    void initSettingOptions();
    void initOther();

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void settingsValueChanged(const QString &key, const QVariant &value);
    void updateWallpaper(QString argument);
    void updateLanguage(QString value);

    void showSettingWidget();
    void showAboutWidget();
    void visitBing();
};

#endif // ONECLICKBINGWALLPAPER_H
