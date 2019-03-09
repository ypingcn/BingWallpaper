# BingWallpaper

[中文版](/README.md)

Setting everyday's Bing wallpaper as the desktop wallpaper ( GUI and command line supported)

## Usage

### CLI

The main part of this program is ```BingWallpaper.py```, if you don't need GUI or in other situation(like use crontab to update in regular time), you can use this file with Python3

```bash
## require python's module: argparse,requests
python3 BingWallpaper.py [options]
```

options

-d specific your desktop environment's name，cinnamon,deepin,gnome,kde,mate,xfce only, welcome your PR for more desktop environment support.

-c custom command to update wallpaper,```{{}}``` must be added, and it will be replaced by the path name of image.

--auto auto detect your desktop environment and use proper command to update.ignored if -d option is added.

--random random image to update wallpaper instead of using the newest one.(You should make sure ~/BingWallpaper is not empty and contains at least one image file)

--silent no notification after updating successfull.

-baseurl to specific subdomain for bing.com for accident,ignored if --random option is added. https://www.bing.com , https://www2.bing.com , https://www4.bing.com , https://cn.bing.com will be choosed automatically if you don't use this option. And this option's value must be in the above four url to avoid other unknown accident.


### GUI

The GUI of this program is built with DTK (Deepin Tool Kit), if you don't need it, remove relavant code manually.


Ubuntu 18.04+, Linuxmint 19+, Deepin 15.9+ users are recommanded to install deb package in release branch to set wallpaper with the help of GUI.

## Contribution

All change must be committed in dev branch before PR, write commit log in English with concise and specific words.Add corresponding type:

feat: new feature

fix: bug fix

docs: documentation

style: code format ( changes that do not affect code operation )

refactor: refactoring ( i.e. code changes that are not new features or bug fixs )

test: add test case

chore: changes of the building process or tools

i.e. ```fix: case insensitive for -d option```