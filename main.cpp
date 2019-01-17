#include "OneClickBingWallpaper.h"

#include <DApplication>

DWIDGET_USE_NAMESPACE

#include <QSettings>
#include <QTranslator>

#include "OneClickBingWallpaperConfig.h"

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);

    a.setOrganizationName("ypingcn");
    a.setApplicationName("oneclickwallpaper");

    QSettings settings(a.organizationName(),a.applicationName());

    OneClickBingWallpaperConfig::updateLanguagesSetting(settings);
    

    QTranslator translator;
    const QString i18nFilePath = OneClickBingWallpaperConfig::geti18nFilePath(settings);
    if(QFile::exists(i18nFilePath))
    {
        translator.load(i18nFilePath);
    }

    a.setQuitOnLastWindowClosed(false);
    a.installTranslator(&translator);

    OneClickBingWallpaper w;

    return a.exec();
}
