#include "OneClickBingWallpaper.h"

#include <DApplication>

DWIDGET_USE_NAMESPACE

#include <QSettings>
#include <QTranslator>

#include "OneClickBingWallpaperConfig.h"

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    QSettings settings("ypingcn","oneclickwallpaper");

    OneClickBingWallpaperConfig::updateLanguagesSetting(settings);
    

    QTranslator translator;
    const QString i18nFilePath = OneClickBingWallpaperConfig::geti18nFilePath(settings);
    if(QFile::exists(i18nFilePath))
    {
        translator.load(i18nFilePath);
    }

    DApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.installTranslator(&translator);

    OneClickBingWallpaper w;

    return a.exec();
}
