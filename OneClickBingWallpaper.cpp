#include "OneClickBingWallpaper.h"

#include <QDir>
#include <QProcess>
#include <QFile>
#include <QMessageBox>

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
    wmAction = new QAction(tr("WM"),this);
    connect(wmAction,SIGNAL(triggered()),this,SLOT(updateWallpaper()));
    quitAction = new QAction(tr("Quit"),this);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));
    trayMenu->addAction(cinnamonAction);
    trayMenu->addAction(xfceAction);
    trayMenu->addAction(deepinAction);
    trayMenu->addAction(wmAction);
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

    if(!QFile::exists(pyFilePath))
    {
        QMessageBox::warning(nullptr, tr("Python File Not Found"), tr("Python file not found,please reinstall.\nClick YES to download page"), QMessageBox::Yes, QMessageBox::No);
        return;
    }
    
    if(QObject::sender() == cinnamonAction)
    {
        p.start("python3 /usr/bin/OneClickBingWallpaper/BingWallpaper.py -d cinnamon");
        p.waitForFinished();
    }
    else if(QObject::sender() == xfceAction)
    {
        p.start("python3 /usr/bin/OneClickBingWallpaper/BingWallpaper.py -d xfce");
        p.waitForFinished();
    }
    else if(QObject::sender() == deepinAction)
    {
        p.start("python3 /usr/bin/OneClickBingWallpaper/BingWallpaper.py -d deepin");
        p.waitForFinished();
    }
    else if(QObject::sender() == wmAction)
    {
        p.start("python3 /usr/bin/OneClickBingWallpaper/BingWallpaper.py -d wm");
        p.waitForFinished();
    }
}
