#include "OneClickBingWallpaper.h"

#include <DApplication>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    OneClickBingWallpaper w;

    return a.exec();
}
