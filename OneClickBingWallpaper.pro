#-------------------------------------------------
#
# Project created by QtCreator 2017-12-05T13:18:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OneClickBingWallpaper
TEMPLATE = app
TRANSLATIONS += i18n/zh-CN.ts i18n/en-US.ts
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
    resource/resource.qrc


icons.files = resource/OneClickBingWallpaper.png
icons_16x16.files = resource/icons/16x16/OneClickBingWallpaper.png
icons_22x22.files = resource/icons/22x22/OneClickBingWallpaper.png
icons_24x24.files = resource/icons/24x24/OneClickBingWallpaper.png
icons_28x28.files = resource/icons/28x28/OneClickBingWallpaper.png
icons_32x32.files = resource/icons/32x32/OneClickBingWallpaper.png
icons_36x36.files = resource/icons/36x36/OneClickBingWallpaper.png
icons_48x48.files = resource/icons/48x48/OneClickBingWallpaper.png
icons_64x64.files = resource/icons/64x64/OneClickBingWallpaper.png
icons_72x72.files = resource/icons/72x72/OneClickBingWallpaper.png
icons_96x96.files = resource/icons/96x96/OneClickBingWallpaper.png
desktop.files = OneClickBingWallpaper.desktop
local.files = i18n/*.qm

isEmpty(INSTALL_PREFIX) {
    unix: INSTALL_PREFIX = /usr
    else: INSTALL_PREFIX = ..
}

unix: {
    desktop.path = $$INSTALL_PREFIX/share/applications
    local.path = $$INSTALL_PREFIX/bin/OneClickBingWallpaper/i18n
    icons.path = $$INSTALL_PREFIX/share/icons/hicolor/128x128/apps
    icons_16x16.path = $$INSTALL_PREFIX/share/icons/hicolor/16x16/apps
    icons_22x22.path = $$INSTALL_PREFIX/share/icons/hicolor/22x22/apps
    icons_24x24.path = $$INSTALL_PREFIX/share/icons/hicolor/24x24/apps
    icons_28x28.path = $$INSTALL_PREFIX/share/icons/hicolor/28x28/apps
    icons_32x32.path = $$INSTALL_PREFIX/share/icons/hicolor/32x32/apps
    icons_36x36.path = $$INSTALL_PREFIX/share/icons/hicolor/36x36/apps
    icons_48x48.path = $$INSTALL_PREFIX/share/icons/hicolor/48x48/apps
    icons_64x64.path = $$INSTALL_PREFIX/share/icons/hicolor/64x64/apps
    icons_72x72.path = $$INSTALL_PREFIX/share/icons/hicolor/72x72/apps
    icons_96x96.path = $$INSTALL_PREFIX/share/icons/hicolor/96x96/apps
    INSTALLS += desktop local icons
    INSTALLS += icons_16x16 icons_22x22 icons_24x24 icons_28x28 icons_32x32 icons_36x36 icons_48x48 icons_64x64 icons_72x72 icons_96x96
}

target.files = OneClickBingWallpaper
target.files += BingWallpaper.py
target.path = $$INSTALL_PREFIX/bin/OneClickBingWallpaper

INSTALLS += target
