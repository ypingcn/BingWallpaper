#include "OneClickBingWallpaper.h"

#include <QDir>
#include <QProcess>
#include <QFile>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>

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
    bool pyFileVaild = true;

    if(!QFile::exists(pyFilePath))
    {
        QMessageBox::warning(nullptr, tr("Python File Not Found"), tr("Python file not found,please reinstall.\nClick YES to download page"), QMessageBox::Yes, QMessageBox::No);
        pyFileVaild = false;
    }
    else
    {
        QFile pyFile(pyFilePath);
        if (pyFile.open(QFile::ReadOnly))
        {
            QCryptographicHash hash(QCryptographicHash::Md5);
            hash.addData(&pyFile);
            QString md5 = hash.result().toHex();
            qDebug() << pyFilePath << md5;
            if (md5 != pyFileMD5)
            {
                QMessageBox::StandardButton choice;
                choice = QMessageBox::information(nullptr, tr("Python File Have Modified"), tr("Python File Have Modified\nClick YES to ignore it"), QMessageBox::Yes, QMessageBox::No);
                if (choice == QMessageBox::No)
                    pyFileVaild = false;
            }
        }
    }

    QProcess p(0);
    if (pyFileVaild)
    {
        if (QObject::sender() == cinnamonAction)
        {
            p.start("python3 "+pyFilePath+" -d cinnamon");
            p.waitForFinished();
        }
        else if (QObject::sender() == xfceAction)
        {
            p.start("python3 "+pyFilePath+" -d xfce");
            p.waitForFinished();
        }
        else if (QObject::sender() == deepinAction)
        {
            p.start("python3 "+pyFilePath+" -d deepin");
            p.waitForFinished();
        }
        else if (QObject::sender() == wmAction)
        {
            p.start("python3 "+pyFilePath+" -d wm");
            p.waitForFinished();
        }
    }
}
