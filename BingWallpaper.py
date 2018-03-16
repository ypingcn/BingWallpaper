#!/usr/bin/python3
# -*- coding:utf-8 -*-

from datetime import datetime
import requests,time,os,argparse

class Logger(object):

    @staticmethod
    def error(content,**kwargs):
        time = datetime.now().strftime("[%Y-%m-%d %H:%M:%S.%f]")
        path = os.path.expanduser('~') + "/BingWallpaper/.oneclickbingwallpaper.log"

        if "path" in kwargs:
            path = kwargs["path"]

        with open(path,"a+") as file:
            file.write(time + "#error :");
            file.write(content+"\n")

    @staticmethod
    def info(content,**kwargs):
        time = datetime.now().strftime("[%Y-%m-%d %H:%M:%S.%f]")
        path = os.path.expanduser('~') + "/BingWallpaper/.oneclickbingwallpaper.log"
        if "path" in kwargs:
            path = kwargs["path"]

        with open(path,"a+") as file:
            file.write(time + "#info :");
            file.write(content+"\n")

class Downloader(object):

    @staticmethod
    def get(url,fileName):
        try:
            downloadReponse = requests.get(url)
        except:
            Logger.error("exception throw when getting "+fileName)

        if downloadReponse.status_code != 200:
            Logger.error("network error: "+str(downloadReponse.status_code))

        lastIndex = fileName.rfind("/")
        if lastIndex:
            if not os.path.exists(fileName[:lastIndex]):
                Logger.info(fileName[:lastIndex] + " not exist,repairing ...")
                os.mkdir(fileName[:lastIndex])

        with open(fileName,"wb") as file:
            file.write(downloadReponse.content)

class BingWallpaper(object):

    def __init__(self,de="",command=""):
        self.baseUrl = "http://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1"
        self.json = requests.get(self.baseUrl).json()
        self.imgUrl = "http://cn.bing.com"+self.json['images'][0]['url']
        self.imgName = "BingWallpaper-" + time.strftime("%Y-%m-%d", time.localtime()) + ".jpg"
        self.imgPath = os.path.expanduser('~') + "/BingWallpaper/" + self.imgName
        self.logPath = os.path.expanduser('~') + "/BingWallpaper/.oneclickbingwallpaper.log"

        self.de = de
        self.command = command

        Downloader.get(self.imgUrl,self.imgPath)

        self.setWallpaper()

    def setWallpaper(self):
        if self.de == "xfce":
            self.command = "xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/workspace0/last-image -s "\
                           +self.imgPath

            Logger.info("xfce command status:"+str(os.system(self.command)))
            self.notify()
            Logger.info("xfce setting finish")

        elif self.de == "cinnamon":
            self.command = "gsettings set org.cinnamon.desktop.background picture-uri  \"file:///"\
                           +self.imgPath+"\""

            Logger.info("cinnamon command status:" + str(os.system(self.command)))
            self.notify()
            Logger.info("cinnamon setting finish")

        elif self.de == "deepin":
            self.command = "gsettings set com.deepin.wrap.gnome.desktop.background picture-uri \"file:///"\
                           +self.imgPath+"\""

            Logger.info("deepin command status:" + str(os.system(self.command)))
            self.notify()
            Logger.info("deepin setting finish")

        elif self.de == "wm":
            self.command = "feh --bg-fill "\
                           +self.imgPath
            Logger.info("feh command status:" + str(os.system(self.command)))
            self.notify()
            Logger.info("feh setting finish")

        elif self.command:
            command = self.command.replace("{{}}",self.imgPath)
            Logger.info(self.command + " command status:" + str(os.system(command)))

        else:
            Logger.info("not support desktop environment:"+str(self.de))

    def notify(self):
        content = self.json['images'][0]['copyright']
        lastIndex = content.rfind("(")
        os.system("notify-send "+self.imgName+":"+content[:lastIndex])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-d",help="desktop environment: xfce etc")
    parser.add_argument("-c",help="command in your device to set desktop background,{{}} will be replaced with the true images path(not end with \)")
    args = parser.parse_args()

    BingWallpaper(args.d,args.c)
