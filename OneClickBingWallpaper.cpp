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

#include <DAboutDialog>

OneClickBingWallpaper::OneClickBingWallpaper(QWidget *parent)
    : QWidget(parent)
{
    DApplication * app;
    configPath = QString("%1/%2/%3.conf").arg(
            QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first(), 
            app->organizationName(), 
            app->applicationName()
        );
    backend = new QSettingBackend(configPath);
    dsettings = DSettings::fromJsonFile(":/config/settings.json");
    dsettings->setBackend(backend);

    initAction();
    initMenu();
    initConnect();
    initSettingOptions();
}

OneClickBingWallpaper::~OneClickBingWallpaper()
{

}

void OneClickBingWallpaper::initAction()
{
    autoAction = new QAction(tr("Auto Setting"),this);
    
    cinnamonAction = new QAction(tr("Cinnamon"),this);
    deepinAction = new QAction(tr("Deepin"),this);
    gnomeAction = new QAction(tr("Gnome"),this);
    kdeAction = new QAction(tr("KDE"),this);
    mateAction = new QAction(tr("Mate"),this);
    wmAction = new QAction(tr("WM"),this);
    xfceAction = new QAction(tr("Xfce"),this);

    zhAction = new QAction(tr("中文"),this);
    enAction = new QAction(tr("English"),this);

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
    actionList << cinnamonAction << deepinAction << gnomeAction \
                << kdeAction << mateAction << wmAction << xfceAction;
    moreMenu->addActions(actionList);

    actionList.clear();
    actionList << zhAction << enAction ;
    langMenu->addActions(actionList);

    trayMenu->addAction(autoAction);
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

    connect(autoAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));

    connect(cinnamonAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    connect(deepinAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    connect(gnomeAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    connect(kdeAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    connect(mateAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    connect(wmAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    connect(xfceAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));

    connect(zhAction,SIGNAL(triggered()),this,SLOT(updateLanguage()));
    connect(enAction,SIGNAL(triggered()),this,SLOT(updateLanguage()));

    connect(settingAction,SIGNAL(triggered()),this,SLOT(showSettingWidget()));

    connect(aboutAction,SIGNAL(triggered()),this,SLOT(showAboutWidget()));
    
    connect(quitAction,&QAction::triggered,[](){
        DApplication * app;
        app->exit(0);
    });
}

void OneClickBingWallpaper::initSettingOptions()
{
    auto duration = dsettings->option("base.autoupdate.duration");
    QMap<QString, QVariant> durationOptions;
    durationOptions.insert("keys", QStringList() << "-1" << "5" << "30");
    durationOptions.insert("values", QStringList() << tr("Disable") << tr("Every 5 minute") << tr("Every 30 minute"));
    duration->setData("items",durationOptions);
}

void OneClickBingWallpaper::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    DApplication * app;
    QSettings settings(app->organizationName(),app->applicationName());

    OneClickBingWallpaperConfig::updateLanguagesSetting(settings);

    QTranslator translator;
    const QString i18nFilePath = OneClickBingWallpaperConfig::geti18nFilePath(settings);
    if(QFile::exists(i18nFilePath))
    {
        translator.load(i18nFilePath);
    }

    app->installTranslator(&translator);


    switch(reason)
    {
        case QSystemTrayIcon::Context:
            this->raise();
            break;
        default:
            break;
    }
}

void OneClickBingWallpaper::updateWallpaper()
{
    bool pyFileVaild = true;

    if(!QFile::exists(OneClickBingWallpaperConfig::pyFilePath))
    {
        QMessageBox::warning(nullptr, tr("Python File Not Found"), tr("Python file not found,please reinstall."), QMessageBox::Yes);
        pyFileVaild = false;
    }
    else
    {
#ifdef PYFILE_MD5_CHECK
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
                choice = QMessageBox::information(nullptr, tr("Python File Have Modified"), tr("Python File Have Modified\nClick YES to ignore it"), QMessageBox::Yes, QMessageBox::No);
                if (choice == QMessageBox::No)
                    pyFileVaild = false;
            }
        }
#endif // PYFILE_MD5_CHECK
    }

    QProcess p(0);
    if (pyFileVaild)
    {
        if (QObject::sender() == autoAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" --auto");
            p.waitForFinished();
        }
        else if (QObject::sender() == cinnamonAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" -d cinnamon");
            p.waitForFinished();
        }
        else if (QObject::sender() == xfceAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" -d xfce");
            p.waitForFinished();
        }
        else if (QObject::sender() == deepinAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" -d deepin");
            p.waitForFinished();
        }
        else if (QObject::sender() == kdeAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" -d kde");
            p.waitForFinished();
        }
        else if (QObject::sender() == gnomeAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" -d gnome");
            p.waitForFinished();
        }
        else if (QObject::sender() == wmAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" -d wm");
            p.waitForFinished();
        }
        else if(QObject::sender() == mateAction)
        {
            p.start("python3 "+OneClickBingWallpaperConfig::pyFilePath+" -d mate");
            p.waitForFinished();
        }
    }
}

void OneClickBingWallpaper::updateLanguage()
{
    DApplication * app;
    QSettings settings(app->organizationName(),app->applicationName());

    qDebug() << settings.value("lang") << endl;
    if(QObject::sender() == zhAction)
    {
        settings.setValue("lang","zh-CN");
    }
    else if(QObject::sender() == enAction)
    {
        settings.setValue("lang","en-US");
    }
    qDebug() << settings.value("lang") << endl;
}

void OneClickBingWallpaper::showSettingWidget()
{
    DSettingsDialog * dialog = new DSettingsDialog;
    dialog->updateSettings(dsettings);
    dialog->move(( DApplication::desktop()->width()-dialog->width() )/2,( DApplication::desktop()->height()-dialog->height() )/2 );
    dialog->show();
}

void OneClickBingWallpaper::showAboutWidget()
{
    DAboutDialog * dialog = new DAboutDialog;

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