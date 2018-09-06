#-------------------------------------------------
#
# Project created by QtCreator 2017-12-05T13:18:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OneClickBingWallpaper
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    OneClickBingWallpaper.cpp \
    OneClickBingWallpaperConfig.cpp

HEADERS += \
    OneClickBingWallpaper.h \
    OneClickBingWallpaperConfig.h

RESOURCES += \
    img.qrc


icons.files = OneClickBingWallpaper.png
desktop.files = OneClickBingWallpaper.desktop

isEmpty(INSTALL_PREFIX) {
    unix: INSTALL_PREFIX = /usr
    else: INSTALL_PREFIX = ..
}

unix: {
    desktop.path = $$INSTALL_PREFIX/share/applications
    icons.path = $$INSTALL_PREFIX/share/icons/hicolor/128x128/apps
    INSTALLS += desktop icons
}

target.files = OneClickBingWallpaper
target.files += BingWallpaper.py
target.path = $$INSTALL_PREFIX/bin/OneClickBingWallpaper

INSTALLS += target
