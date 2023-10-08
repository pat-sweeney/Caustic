import threading
import requests
import os
from io import BytesIO
import time
import json, jsonpath_ng
import tkinter

from PIL import Image, ImageTk
main=tkinter.Tk()
main.geometry("1920x1080")
main.attributes('-fullscreen', True)
canvas = tkinter.Canvas(main, width=1920, height=1080)
canvas.pack()
srcimg = Image.open(r'c:\users\patricsw\Pictures\image1.PNG')
srcimg = srcimg.resize((1920, 1080))
canvasimg = canvas.create_image(960,540,image=ImageTk.PhotoImage(srcimg))


def ShowImage(path, sid):
    # Download image
    api_url = "http://192.168.1.136:5000/webapi/entry.cgi"
    params={
        'api':'SYNO.FileStation.Download',
        'version':'2',
        'method':'download',
        'path':path,
        'mode':"download",
        '_sid':sid,
    }
    response = requests.get(api_url,params=params)
    img = Image.open(BytesIO(response.content))
#    if img.width > img.height:
#        scale = img.width / float(img.height)
#        img = img.resize((1920, int(1080 / scale)))
#    else:
#        scale = img.height / float(img.width)
#        img = img.resize((int(1920 / scale), 1080))
    itk2 = ImageTk.PhotoImage(img)
    canvas.itemconfigure(canvasimg, image=itk2)
    time.sleep(10)

def WalkFolder(folderPath, sid):
    api_url = "http://192.168.1.136:5000/webapi/entry.cgi"
    params={
        'api':'SYNO.FileStation.List',
        'version':'1',
        'method':'list',
        'folder_path':folderPath,
        '_sid':sid,
    }
    response = requests.get(api_url,params=params)
    response_json = response.json()
    jsonpath_expr = jsonpath_ng.parse('$.data.files')
    matches = jsonpath_expr.find(response_json)
    for match in matches:
        for entry in match.value:
            print(entry)
            if entry['isdir']:
                if entry['name'] == '#recycle':
                    continue
                WalkFolder(entry['path'], sid)
            else:
                name = entry['name']
                if not '.png' in name and not '.jpg' in name and not '.jpeg' in name:
                    continue
                ShowImage(entry['path'], sid)

def StartThread():
    # First log into NAS
    userid = os.environ['SYNOLOGY_USERID']
    pwd = os.environ['SYNOLOGY_PASSWORD']
    api_url = "http://192.168.1.136:5000/webapi/query.cgi"
    params={
        'api':'SYNO.API.Auth',
        'version':'3',
        'method':'login',
        'account':userid,
        'passwd':pwd,
        'session':'FileStation',
        'format':'cookie',
    }
    response = requests.get(api_url,params=params)
    response_json = response.json()
    jsonpath_expr = jsonpath_ng.parse('$.data.sid')
    matches = jsonpath_expr.find(response_json)

    # Enumerate the top level files
    while True:
        WalkFolder("/Photos",matches[0].value)

t = threading.Thread(target=StartThread)
t.start()
main.mainloop()
