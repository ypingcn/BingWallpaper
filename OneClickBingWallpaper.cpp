#include "OneClickBingWallpaper.h"

#include <QDir>
#include <QProcess>

OneClickBingWallpaper::OneClickBingWallpaper(QWidget *parent)
    : QWidget(parent)
{
    QIcon icon = QIcon(":/OneClickBingWallpaper.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->show();
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    trayMenu = new QMenu(this);
    cinnamonAction = new QAction(tr("Cinnamon"),this);
    connect(cinnamonAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    xfceAction = new QAction(tr("Xfce"),this);
    connect(xfceAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    deepinAction = new QAction(tr("Deepin"),this);
    connect(deepinAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    quitAction = new QAction(tr("Quit"),this);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
    trayMenu->addAction(cinnamonAction);
    trayMenu->addAction(xfceAction);
    trayMenu->addAction(deepinAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
}

OneClickBingWallpaper::~OneClickBingWallpaper()
{

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

void OneClickBingWallpaper::updateWallpaper()
{
    QProcess p(0);

    if(QObject::sender() == cinnamonAction)
    {
        p.start("python3 /usr/bin/OneClickGo/BingWallpaper.py -d cinnamon");
        p.waitForFinished();
    }
    else if(QObject::sender() == xfceAction)
    {
        p.start("python3 /usr/bin/OneClickGo/BingWallpaper.py -d xfce");
        p.waitForFinished();
    }
    else if(QObject::sender() == deepinAction)
    {
        p.start("python3 /usr/bin/OneClickGo/BingWallpaper.py -d deepin");
        p.waitForFinished();
    }
}
