import requests
import json
import os
import sys
import datetime
 
def Down_data():
    url = 'https://view.inews.qq.com/g2/getOnsInfo?name=disease_h5'
    headers = {
        'user-agent': 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Mobile Safari/537.36'
    }
    r = requests.get(url, headers)
    res = json.loads(r.text)
    data_res = json.loads(res['data'])
    return data_res
 
def Parse_data1():
    data = Down_data()
    list = [ datetime.datetime.now().strftime('截止%m月%d日%H时') + ','
             '全国累计确诊：' + str(data['chinaTotal']['confirm']) + '例,'
             '新增确诊：' + str(data['chinaAdd']['confirm']) + '例,'
             '累计境外输入：' + str(data['chinaTotal']['importedCase']) + '例,'
             '新增境外输入：' + str(data['chinaAdd']['importedCase']) + '例,'
             '累计治愈：' + str(data['chinaTotal']['heal']) + '例,'
             '新增治愈：' + str(data['chinaAdd']['heal']) + '例,'
             '累计死亡：' + str(data['chinaTotal']['dead']) + '例,'
             '新增死亡：' + str(data['chinaAdd']['dead']) + '例.']
    result = ''.join(list)
    with open('全国实时数据'+str(data['lastUpdateTime'])+'.txt', 'a+', encoding="utf-8") as f:
        f.write(result + '\n')
    f = open('全国实时数据'+str(data['lastUpdateTime'])+'.txt',"r")
    text = f.read()
    cmd = "ilang " + text
    os.system(cmd)
    
 
def Parse_data2():
    data = Down_data()['areaTree'][0]['children']
    path = str(input('请输入你要查询的省份：'))
    for i in data:
        if path in i['name']:
            for item in i['children']:
                list_city = [
                    '地区: ' + str(item['name']),
                    '\t累计确诊：' + str(item['total']['confirm']),
                    '\t新增确诊：' + str(item['today']['confirm']) + '\n'

                ]
                res_city = ''.join(list_city)
                with open(path+'省数据'+str(datetime.datetime.now().strftime('%m月%d日'))+'.txt', 'a+', encoding="utf-8") as f:
                    f.write(res_city)

Down_data()
Parse_data1()
Parse_data2()

