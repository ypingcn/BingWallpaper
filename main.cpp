#include "OneClickBingWallpaper.h"

#include <DApplication>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);

    OneClickBingWallpaper w;

    return a.exec();
}
