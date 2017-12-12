#!/usr/bin/python3
# -*- coding:utf-8 -*-

from datetime import datetime
import requests,time,os,argparse


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

        self.download()
        self.setWallpaper()

    def download(self):
        if os.path.exists(self.imgPath):
            self.log("file exist "+self.imgPath)
            return

        try:
            imgResponse = requests.get(self.imgUrl)
        except:
            self.log("exception throw when getting "+self.imgName)

        if imgResponse.status_code != 200:
            self.log("network error: "+str(imgResponse.status_code))
            return

        if not os.path.exists(os.path.expanduser('~') + "/BingWallpaper/"):
            os.mkdir(os.path.expanduser('~') + "/BingWallpaper/")
        with open(self.imgPath,"wb") as file:
            file.write(imgResponse.content)

    def setWallpaper(self):
        if self.de == "xfce":
            self.command = "xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/workspace0/last-image -s "\
                           +self.imgPath

            self.log("xfce command status:"+str(os.system(self.command)))
            self.notify()
            self.log("xfce setting finish")
            
        elif self.de == "cinnamon":
            self.command = "gsettings set org.cinnamon.desktop.background picture-uri  \"file:///"\
                           +self.imgPath+"\""

            self.log("cinnamon command status:" + str(os.system(self.command)))
            self.notify()
            self.log("cinnamon setting finish")

        elif self.de == "deepin":
            self.command = "gsettings set com.deepin.wrap.gnome.desktop.background picture-uri \"file:///"\
                           +self.imgPath+"\""

            self.log("deepin command status:" + str(os.system(self.command)))
            self.notify()
            self.log("deepin setting finish")

        else:
            self.log("not support desktop environment:"+str(self.de))
    
    def notify(self):
        content = self.json['images'][0]['copyright']
        lastIndex = content.rfind("(")
        os.system("notify-send "+self.imgName+":"+content[:lastIndex])

    def log(self,content):
        with open(self.logPath,"a+") as file:
            file.write(datetime.now().strftime("[%Y-%m-%d %H:%M:%S.%f] "));
            file.write(content+"\n")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-d",help="desktop environment: xfce etc")
    parser.add_argument("-c",help="command in your device to set desktop background")
    args = parser.parse_args()

    BingWallpaper(args.d,args.c)
