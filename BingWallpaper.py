#!/usr/bin/python3
# -*- coding:utf-8 -*-


import requests,time,os,argparse


class BingWallpaper(object):

    def __init__(self,de="",command=""):
        self.baseUrl = "http://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1"
        self.json = requests.get(self.baseUrl).json()
        self.imgUrl = "http://cn.bing.com"+self.json['images'][0]['url']
        self.imgName = "BingWallpaper-" + time.strftime("%Y-%m-%d", time.localtime()) + ".jpg"
        self.imgPath = os.path.expanduser('~') + "/BingWallpaper/" + self.imgName

        self.de = de
        self.command = command

        self.download()
        self.setWallpaper()

    def download(self):
        imgResponse = requests.get(self.imgUrl)
        if imgResponse.status_code != 200:
            print("network error: "+str(imgResponse.status_code))
            return
        if not os.path.exists(os.path.expanduser('~') + "/BingWallpaper/"):
            os.mkdir(os.path.expanduser('~') + "/BingWallpaper/")
        with open(self.imgPath,"wb") as file:
            file.write(imgResponse.content)

    def setWallpaper(self):
        if self.de == "xfce":
            self.command = "xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/workspace0/last-image -s "\
                           +self.imgPath

            os.system(self.command)
            self.notify()
            
        elif self.de == "cinnamon":
            self.command = "gsettings set org.cinnamon.desktop.background picture-uri  \"file:///"\
                           +self.imgPath+"\""
            print(self.command)
            os.system(self.command)
            self.notify()

        elif self.de == "deepin":
            self.command = "gsettings set com.deepin.wrap.gnome.desktop.background picture-uri \"file:///"\
                           +self.imgPath+"\""
            print(self.command)
            os.system(self.command)
            self.notify()

        else:
            print("not support desktop environment:",self.de)
    
    def notify(self):
        content = self.json['images'][0]['copyright']
        lastIndex = content.rfind("(")
        os.system("notify-send "+self.imgName+":"+content[:lastIndex])

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-d",help="desktop environment: xfce etc")
    parser.add_argument("-c",help="command in your device to set desktop background")
    args = parser.parse_args()

    BingWallpaper(args.d,args.c)
