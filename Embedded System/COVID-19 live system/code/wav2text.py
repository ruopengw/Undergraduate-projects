#!/usr/bin/python
# -*- coding: UTF-8 -*-
from aip import AipSpeech

#从百度AI开放平台创建应用处获取
APP_ID = '19163157'
API_KEY = 'EOh02I4yMBDs3Yjuz5feRY4n'
SECRET_KEY = 'q29Ql47tYwWZG5xn5YFndLTRNImBRrzn'

client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)

# 读取文件
def get_file_content(filePath):
    with open(filePath, 'rb') as fp:
        return fp.read()

# 识别本地文件
def get_text():
    result = client.asr(get_file_content('test.wav'), 'wav', 8000, {
    'dev_pid': 1537,})
    #print(result)
    text = result['result'][0]
    return text

print(get_text())
