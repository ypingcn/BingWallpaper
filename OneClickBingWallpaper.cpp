#include "OneClickBingWallpaper.h"
#include "OneClickBingWallpaperConfig.h"


#include <DApplication>

DWIDGET_USE_NAMESPACE

#include <QDir>
#include <QProcess>
#include <QFile>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QSettings>
#include <QTranslator>
#include <QDesktopWidget>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QTextBrowser>

#include <DAboutDialog>

OneClickBingWallpaper::OneClickBingWallpaper(QWidget *parent)
    : QWidget(parent)
{
    DApplication * app;
    imagePath = QString("%1/%2").arg(
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
        "BingWallpaper");
    configPath = QString("%1/%2/%3.conf").arg(
            QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first(), 
            app->organizationName(), 
            app->applicationName()
        );
    backend = new QSettingBackend(configPath);
    dsettings = DSettings::fromJsonFile(":/config/settings.json");
    dsettings->setBackend(backend);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        updateWallpaper("--auto");
    });
    auto interval = dsettings->option("base.autoupdate.interval");
    if(interval->value().toInt() != -1)
    {
        timer->setInterval(interval->value().toInt()*60*1000);
        timer->start();
    }

    initAction();
    initMenu();
    initConnect();
    initSettingOptions();
    initOther();
}

OneClickBingWallpaper::~OneClickBingWallpaper()
{

}

void OneClickBingWallpaper::initAction()
{        
    for(int i = 0 ; i < vDesktopEnvironments.size() ; i++)
    {
        DesktopEnvironmentType item = vDesktopEnvironments[i];
        QAction * action = new QAction(item.name,this);
        connect(action,&QAction::triggered,[this,item]()
        {
            updateWallpaper(item.argument);
        });
        vDEActions.push_back( action );
    }
    for(int i = 0 ; i < vLanguages.size(); i++)
    {
        LanguageType item = vLanguages[i];
        QAction * action = new QAction(item.name,this);
        connect(action, &QAction::triggered, [this,item]()
        {
            updateLanguage(item.value);
        });
        vLangActions.push_back(action);
    }

    updateAction = new QAction(tr("Update"),this);

    settingAction = new QAction(tr("Setting"),this);

    aboutAction = new QAction(tr("About"),this);

    quitAction = new QAction(tr("Quit"),this);
}

void OneClickBingWallpaper::initMenu()
{   
    trayMenu = new QMenu(this);
    moreMenu = new QMenu(tr("Specific DE"),this);
    langMenu = new QMenu(tr("Language"),this);

    QList<QAction*> actionList;
    for(auto item : vDEActions)
    {
        actionList << item ;
    }
    moreMenu->addActions(actionList);

    actionList.clear();
    for(auto item : vLangActions)
    {
        actionList << item ;
    }
    langMenu->addActions(actionList);

    trayMenu->addAction(updateAction);
    trayMenu->addMenu(moreMenu);
    trayMenu->addMenu(langMenu);
    trayMenu->addAction(settingAction);
    trayMenu->addAction(aboutAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    
    QIcon icon = QIcon(":/OneClickBingWallpaper.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->show();
    trayIcon->setContextMenu(trayMenu);
}

void OneClickBingWallpaper::initConnect()
{
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(dsettings,SIGNAL(valueChanged(QString,QVariant)),this,SLOT(settingsValueChanged(QString,QVariant)));

    connect(updateAction, &QAction::triggered, [this](){
        updateWallpaper("--auto");
    });

    connect(settingAction,SIGNAL(triggered()),this,SLOT(showSettingWidget()));

    connect(aboutAction,SIGNAL(triggered()),this,SLOT(showAboutWidget()));
    
    connect(quitAction,&QAction::triggered,[](){
        DApplication * app;
        app->exit(0);
    });
}

void OneClickBingWallpaper::initSettingOptions()
{
    auto interval = dsettings->option("base.autoupdate.interval");
    QMap<QString, QVariant> intervalOptions;
    intervalOptions.insert("keys", QStringList() << "-1" << "1" << "5" << "10" << "15" << "30" << "60");
    intervalOptions.insert("values", QStringList() << tr("Disable") << tr("Every 1 minute") << tr("Every 5 minute") << tr("Every 10 minute") 
                                                                    << tr("Every 15 minute") << tr("Every 30 minute") << tr("Every 60 minute"));
    interval->setData("items",intervalOptions);

    auto updateType = dsettings->option("base.update.type");
    QMap<QString, QVariant> updateTypeOptions;
    updateTypeOptions.insert("keys", QStringList() << "lastest" << "random" );
    updateTypeOptions.insert("values", QStringList() << tr("Lastest") << tr("Random") );
    updateType->setData("items",updateTypeOptions);

    auto subdomainType = dsettings->option("base.domain.subdomain");
    QMap<QString, QVariant> subdomainTypeOptions;
    subdomainTypeOptions.insert("keys", QStringList() << "auto" << "https://www.bing.com" << "https://cn.bing.com" << "https://www2.bing.com" << "https://www4.bing.com" );
    subdomainTypeOptions.insert("values", QStringList() << tr("Auto") << "https://www.bing.com" << "https://cn.bing.com" << "https://www2.bing.com" << "https://www4.bing.com" );
    subdomainType->setData("items",subdomainTypeOptions);
}

void OneClickBingWallpaper::initOther()
{
    auto startupEnable = dsettings->option("base.autoupdate.startup-enable");
    if( startupEnable->value().toBool() )
    {
        updateWallpaper("--auto");
    }
}

void OneClickBingWallpaper::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Context:
            this->raise();
            break;
        default:
            break;
    }
}

void OneClickBingWallpaper::settingsValueChanged(const QString &key, const QVariant &value)
{
    qDebug() << "settingsValueChanged " << key << " " << value << endl;
    if(key == "base.update.type" && value.toString() == "random")
    {
        QDir dir(imagePath);
        qDebug() << "imagePath " << imagePath << endl;
        if (!dir.exists())
        {
            QMessageBox::information(nullptr, tr("Folder Not Found"),
                                     tr("BingWallpaper folder %1 in home is not exist, tool will create it by default").arg(imagePath),
                                     QMessageBox::Ok);
            bool res = dir.mkdir(imagePath);
            qDebug() << "create folder " << imagePath << ":" << res << endl;
        }
        dir.setFilter(QDir::Files);
        if (dir.entryInfoList().count() == 0)
        {
            QMessageBox::information(nullptr, tr("Attention! Empty Folder"),
                                     tr("Empty folder for %1 ,\n"
                                     "if you want to update wallpaper for random, please make sure to put some image file in this folder, or use lastest first").arg(imagePath),
                                     QMessageBox::Ok);
        }
    }
    if(key == "base.autoupdate.interval")
    {
        timer->stop();
        if(value.toInt() != -1 )
        {
            timer->setInterval(value.toInt() * 60 * 1000);
            timer->start();
        }
    }
}

void OneClickBingWallpaper::updateWallpaper(QString argument)
{
    bool pyFileVaild = true;
    auto pythonCheck = dsettings->option("base.security.python-check");
    auto isRandom = dsettings->option("base.update.type");
    auto isNotifyEnable = dsettings->option("base.notification.enable");
    auto subDomain = dsettings->option("base.domain.subdomain");
    QDir dir(imagePath);

    if (!dir.exists())
    {
        QMessageBox::information(nullptr, tr("Folder Not Found"),
                                 tr("BingWallpaper folder %1 in home is not exist, tool will create it by default").arg(imagePath),
                                 QMessageBox::Ok);
        bool res = dir.mkdir(imagePath);
        qDebug() << "create folder " << imagePath << ":" << res << endl;
    }

    dir.setFilter(QDir::Files);

    if(!QFile::exists(OneClickBingWallpaperConfig::pyFilePath))
    {
        QMessageBox::warning(nullptr, tr("Python File Not Found"), tr("Python file not found,please reinstall."), QMessageBox::Ok);
        return;
    }

    if(dir.entryInfoList().count() == 0 && isRandom->value().toString() == "random")
    {
        QMessageBox::StandardButton choice;
        choice = QMessageBox::information(nullptr, tr("Attention! Empty Folder"),
                                tr("Empty folder for %1.\n"
                                "If you want to update wallpaper for random, please make sure to put some image file in this folder, or use lastest first.\n"
                                "Setting Abort, click YES to open folder, NO to ignore.").arg(imagePath),
                                QMessageBox::Yes, QMessageBox::No);
        if(choice == QMessageBox::Yes)
        {
            bool ok = QDesktopServices::openUrl(QUrl(imagePath));
            qDebug() << "open folder :" << ok << endl;
        }
        return;
    }

    if(pythonCheck->value().toBool())
    {
        QFile pyFile(OneClickBingWallpaperConfig::pyFilePath);
        if (pyFile.open(QFile::ReadOnly))
        {
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(&pyFile);
            QString md5 = hash.result().toHex();
            qDebug() << OneClickBingWallpaperConfig::pyFilePath << md5;
            if (md5 != OneClickBingWallpaperConfig::pyFileMD5)
            {
                QMessageBox::StandardButton choice;
                choice = QMessageBox::information(nullptr, tr("Python File Have Modified"), 
                                                tr("Python File Have Modified\nClick YES to ignore it"), 
                                                QMessageBox::Yes, QMessageBox::No);
                if (choice == QMessageBox::No)
                    pyFileVaild = false;
            }
        }
    }

    if (pyFileVaild)
    {
        QProcess * process = new QProcess;
        QString command = QString("python3 %1 %2").arg(OneClickBingWallpaperConfig::pyFilePath,argument);
        if(isRandom->value().toString() == "random")
        {
            command += " --random";
        }
        if(!isNotifyEnable->value().toBool())
        {
            command += " --silent";
        }
        if(subDomain->value().toString() != "auto")
        {
            command += " -baseurl " + subDomain->value().toString();
        }

        qDebug() << command << endl;

        process->start(command);
        process->waitForFinished();

        QByteArray error_byte = process->readAllStandardError();
        QString error = error_byte;
        
        if(!error.isEmpty())
        {
            QTextBrowser * text = new QTextBrowser();
            text->resize(400,300);
            text->setWindowTitle(tr("Error"));
            text->append("------------------------------------------------------");
            text->append(tr("Something wrong "));
            text->append(tr("Please check/adjust your setting or network connection, "
                            "or just report this issue to maintainer with the following log."));
            text->append("------------------------------------------------------");
            text->append(error);
            text->show();
        }
        qDebug() << error << endl;
    }
}

void OneClickBingWallpaper::updateLanguage(QString value)
{
    DApplication * app;
    QSettings settings(app->organizationName(),app->applicationName());

    qDebug() << settings.value("lang") << endl;
    settings.setValue("lang",value);
    qDebug() << settings.value("lang") << endl;

    app->quit();
    QProcess::startDetached(app->arguments()[0]);
}

void OneClickBingWallpaper::showSettingWidget()
{
    DSettingsDialog * dialog = new DSettingsDialog(this);
    dialog->updateSettings(dsettings);
    dialog->move(( DApplication::desktop()->width()-dialog->width() )/2,( DApplication::desktop()->height()-dialog->height() )/2 );
    dialog->show();
}

void OneClickBingWallpaper::showAboutWidget()
{
    DAboutDialog * dialog = new DAboutDialog(this);

    QIcon icon;
    icon.addFile(":/OneClickBingWallpaper.png",QSize(96,96));
    QPixmap emptyPixmap;
    DApplication * app;

    dialog->setAttribute(Qt::WA_DeleteOnClose) ;
    dialog->setProductIcon(icon);
    dialog->setProductName(app->applicationDisplayName());
    dialog->setVersion("");
    dialog->setAcknowledgementLink("https://github.com/ypingcn/BingWallpaper/wiki/Acknowledgement");
    dialog->setWebsiteName("https://github.com/ypingcn/BingWallpaper");
    dialog->setWebsiteLink("https://github.com/ypingcn/BingWallpaper");
    dialog->setCompanyLogo(emptyPixmap);
    dialog->setDescription(tr("A tool to set lastest Bingwallpaper in Linux desktop."));
    dialog->show();
    dialog->move(( DApplication::desktop()->width()-dialog->width() )/2,( DApplication::desktop()->height()-dialog->height() )/2 );
}