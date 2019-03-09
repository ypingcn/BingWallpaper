# BingWallpaper

[English Version](/README-en.md)

将当天的必应美图设置为桌面背景（支持图形化和命令行设置）

## 使用

### 命令行

本程序的主体部分是```BingWallpaper.py``` 文件，如无需用户界面或者其他情况（例如结合 crontab 做定时更新），可以直接在 Python3 环境中运行

```bash
## (需提前安装 Python 的 argparse requests)
python3 BingWallpaper.py [参数]
```

参数

-d 指定桌面环境，暂时支持 cinnamon,deepin,gnome,kde,mate,xfce，欢迎 PR 添加其他桌面环境

-c 自定义更改壁纸的命令，必须包含```{{}}```，此符号将会被替换成图片路径

--auto 自动探测桌面环境，自动选择合适的命令进行设置，如果与 -d 选项同时使用，-d 选项将被忽略

--random 随机选择一张壁纸进行设置而非使用当天的壁纸（需自行保证 ~/BingWallpaper 目录不为空且包含图片文件）

--silent 成功设置后不发送通知

-baseurl 指定 bing.com 的子域名，以防无法获取图片信息。如果与 --random 选项同时使用，--random 选项将被忽略。不使用该选项程序将会在以下子域名自动选择 https://www.bing.com、https://www2.bing.com、https://www4.bing.com、https://cn.bing.com 。自行指定的选项也要在以上四个选项中选择以免出现其他不可预料的问题。


### 用户界面

用户界面使用了 DTK (Deepin Tool Kit)，无需使用请在源代码中移除相应部分。

建议 Ubuntu 18.04+、Linuxmint 19+、Deepin 15.9+ 用户下载 release 里的安装包，安装后使用图形化界面一键设置

## 参与贡献

所有代码修改请在 dev 分支修改后再发 PR，所有的 commit 记录应简洁高效地记录修改原因，使用英文书写 commit，并在内容前加上修改类型，类型如下

feat：新功能（feature）

fix：修补bug

docs：文档（documentation）

style： 格式（不影响代码运行的变动）

refactor：重构（即不是新增功能，也不是修改bug的代码变动）

test：增加测试

chore：构建过程或辅助工具的变动

例如 ```fix: case insensitive for -d option```
