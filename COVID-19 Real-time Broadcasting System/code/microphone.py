import os
import sys
import datetime

name = datetime.datetime.now().strftime(' %m月%d日%H:%M:%S.wav')
time = 10
cmd = "sudo arecord -D 'plughw:1,0' -d " + str(time) + name

os.system(cmd)
