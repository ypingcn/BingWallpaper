#include "OneClickBingWallpaper.h"

#include <DApplication>

DWIDGET_USE_NAMESPACE

#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    QSettings settings("ypingcn","oneclickwallpaper");
    
    // change this part in the future
    ////////////////////////////////////////////////////////////
    QLocale locale = QLocale::system();
    QString localeName = QLocale::countryToString(locale.country());
    QString localeShortName;

    if(localeName == QString("China"))
    {
        localeShortName = "zh-CN";
    }
    else
    {
        localeShortName = "en-US";
    }
    if(!settings.contains("lang"))
        settings.setValue("lang",localeShortName);

    ////////////////////////////////////////////////////////////

    QTranslator translator;
    const QString i18nFilePathPrefix = "/usr/bin/OneClickBingWallpaper/i18n/";
    const QString i18nFilePath = i18nFilePathPrefix+settings.value("lang").toString()+".qm";
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
