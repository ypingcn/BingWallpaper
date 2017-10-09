#!/usr/bin/python3
# -*- coding:utf-8 -*-


import requests,time,os,argparse


class BingWallpaper(object):

    def __init__(self,de="",command=""):
        self.baseUrl = "http://cn.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1"
        self.json = requests.get(self.baseUrl).json()
        self.imgUrl = "http://cn.bing.com"+self.json['images'][0]['url']
        self.imgName = "BingWallpaper-" + time.strftime("%Y-%m-%d", time.localtime()) + ".jpg"

        self.de = de
        self.command = command

        self.download()
        self.setWallpaper()

    def download(self):
        imgResponse = requests.get(self.imgUrl)
        if imgResponse.status_code != 200:
            print("network error: "+str(imgResponse.status_code))
            return
        if not os.path.exists("imgs"):
            os.mkdir("imgs")
        with open("imgs/"+self.imgName,"wb") as file:
            file.write(imgResponse.content)

    def setWallpaper(self):
        if self.de == "xfce":
            self.command = "xfconf-query -c xfce4-desktop -p /backdrop/screen0/monitor0/workspace0/last-image -s "\
                           +os.getcwd()+"/imgs/"+self.imgName

            os.system(self.command)

            content = self.json['images'][0]['copyright']
            lastIndex = content.rfind("(")

            os.system("notify-send "+self.imgName+":"+content[:lastIndex])

        else:
            print("not support desktop environment:",self.de)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-d",help="desktop environment: xfce etc")
    parser.add_argument("-c",help="command in your device to set desktop background")
    args = parser.parse_args()

    BingWallpaper(args.d,args.c)
