#ifndef ONECLICKBINGWALLPAPERTRANSLATION_H
#define ONECLICKBINGWALLPAPERTRANSLATION_H

#include <DSettings>

void GenerateSettingTranslate()
{
    auto base = QObject::tr("Basic");                      // base
    auto baseUpdate = QObject::tr("Update");               // base.update
    auto baseUpdateType = QObject::tr("Type");             // base.update.type
    auto baseNotification = QObject::tr("Notification");   // base.notification
    auto baseDomain = QObject::tr("Domain");               // base.domain
    auto baseAutoupdate = QObject::tr("Auto Update");      // base.autoupdate
    auto baseAutoupdateInterval = QObject::tr("Interval"); // base.autoupdate.interval
    auto baseSecurity = QObject::tr("Security");           // base.security

    auto baseNotificationText = QObject::tr("Send Notification");
    auto baseDomainSubdomainText = QObject::tr("Subdomain for bing.com");
    auto baseAutoupdateStartupenableText = QObject::tr("Autoupdate Bingwallpaper when tool startup");
    auto baseSecurityPythoncheckText = QObject::tr("Check setting Python file's MD5");

    auto restoreDefaults = QObject::tr("Restore Defaults");
}

#endif // ONECLICKBINGWALLPAPERTRANSLATION_H
