# BingWallpaper

将当天的必应美图设置为桌面背景（支持 xfce ）

## 安装

```bash
sudo bash install
```

## 卸载

```bash
sudo bash uninstall
```

## 使用

```bash
## 安装后
bingwallpaper -d [desktop-environment-name] //暂时支持 xfce,欢迎 PR 添加其他桌面环境
bingwallpaper -c [set-desktop-wallpaper-command] // 尚未支持
## 免安装直接使用(需提前安装 Python 的 argparse)
python3 BingWallpaper.py -d [desktop-environment-name]
python3 BingWallpaper.py -c [set-desktop-wallpaper-command]
```
