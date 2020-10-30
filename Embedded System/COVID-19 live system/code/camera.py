import os
import sys
import datetime

name = datetime.datetime.now().strftime('%m月%d日%H:%M:%S.jpg')

cmd = "fswebcam --no-banner -r 640x480 "+name
os.system(cmd)
