#ifndef ONECLICKBINGWALLPAPERCONFIG_H
#define ONECLICKBINGWALLPAPERCONFIG_H

#include <QString>
#include <QLocale>
#include <QSettings>

#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define PATCH_VERSION 0

class OneClickBingWallpaperConfig
{
public:
    static QString pyFileMD5;       // Please run ./config.sh first to generate/update pyFileMD5
    static QString latestCommitHash;  // Please run ./config.sh first to generate/update latestCommitHash
    static QString pyFilePath;
    static QString i18nFilePathPrefix;
    static void updateLanguagesSetting(QSettings &settings)
    {
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
    }
    static QString geti18nFilePath(QSettings & settings)
    {
        return i18nFilePathPrefix+settings.value("lang").toString()+".qm";
    }
};

#endif // !ONECLICKBINGWALLPAPERCONFIG_H