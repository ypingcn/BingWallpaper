#ifndef ONECLICKBINGWALLPAPERTRANSLATION_H
#define ONECLICKBINGWALLPAPERTRANSLATION_H

#include <DSettings>

void GenerateSettingTranslate()
{
    auto base = QObject::tr("Basic");                      // base
    auto baseUpdate = QObject::tr("Update");               // base.update
    auto baseUpdateType = QObject::tr("Type");             // base.update.type
    auto baseAutoupdate = QObject::tr("Auto Update");      // base.autoupdate
    auto baseAutoupdateInterval = QObject::tr("Interval"); // base.autoupdate.interval
    auto baseSecurity = QObject::tr("Security");           // base.security

    auto baseAutoupdateStartupenableText = QObject::tr("Autoupdate Bingwallpaper when tool startup");
    auto baseAutoupdateEnableText = QObject::tr("Autoupdate Bingwallpaper in specific interval");
    auto baseSecurityPythoncheckText = QObject::tr("Check setting Python file's MD5");
}

#endif // ONECLICKBINGWALLPAPERTRANSLATION_H
