#ifndef ONECLICKBINGWALLPAPERTRANSLATION_H
#define ONECLICKBINGWALLPAPERTRANSLATION_H

#include <DSettings>

void GenerateSettingTranslate()
{
    auto base = QObject::tr("Basic");                            // base
    auto baseFile = QObject::tr("File");                         // base.file
    auto baseFileImageFolder = QObject::tr("Image Folder");      // base.file.image-folder
    auto baseUpdate = QObject::tr("Update");                     // base.update
    auto baseUpdateType = QObject::tr("Type");                   // base.update.type
    auto baseUpdateCustomCmd = QObject::tr("Custom Update Command");  // base.update.custom-command
    auto baseUpdateDesktop = QObject::tr("Desktop Environment"); // base.update.desktop
    auto baseNotification = QObject::tr("Notification");         // base.notification
    auto baseDomain = QObject::tr("Domain");                     // base.domain
    auto baseAutoupdate = QObject::tr("Auto Update");            // base.autoupdate
    auto baseAutoupdateInterval = QObject::tr("Interval");       // base.autoupdate.interval
    auto baseSecurity = QObject::tr("Security");                 // base.security
    auto baseOther = QObject::tr("Other");                       // base.other

    auto baseNotificationText = QObject::tr("Send Notification");
    auto baseDomainSubdomainText = QObject::tr("Subdomain for bing.com");
    auto baseAutoupdateStartupenableText = QObject::tr("Autoupdate Bingwallpaper when tool startup");
    auto baseSecurityPythoncheckText = QObject::tr("Check setting Python file's MD5");
    auto baseOtherLanguageUpdateHintText = QObject::tr("Show hint of restart for updating language")

        auto restoreDefaults = QObject::tr("Restore Defaults");
}

#endif // ONECLICKBINGWALLPAPERTRANSLATION_H
