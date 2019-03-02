#!/bin/bash

# update pyFileMD5 in OneClickBingWallpaperConfig.cpp for security check

MD5SUM=$(md5sum BingWallpaper.py | awk {'print $1'})
sed -i 's/{{MD5SUM}}/'$MD5SUM'/g' OneClickBingWallpaperConfig.cpp
sed -i 's/\/\/ QString OneClickBingWallpaperConfig::pyFileMD5/QString OneClickBingWallpaperConfig::pyFileMD5/g' OneClickBingWallpaperConfig.cpp