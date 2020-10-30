# -*- coding: utf-8 -*-


import os
import numpy
from pandas import DataFrame
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import MultinomialNB
import warnings
warnings.filterwarnings("ignore")
def readdata():
    data = DataFrame({'message': [], 'class': []})
    record = []
    path = "D:/junior/OOP/proj/ConsoleApplication2/enron1/ham" #文件夹目录
    files= os.listdir(path) #得到文件夹下的所有文件名称
    for file in files: #遍历文件夹
        position = path+'\\'+ file #构造绝对路径，"\\"，其中一个'\'为转义符          
        with open(position, "r",encoding='gbk') as f:    #打开文件
            data0 = f.read()   #读取文件
            data0 = data0.replace('\n','')
            data0 = data0.replace('Subject:','')
            data0 = data0.replace(' -','') 
            record.append({'message': data0, 'class': 'ham'})
    data = data.append(DataFrame(record))
    record = []
    path = "D:/junior/OOP/proj/ConsoleApplication2/enron1/spam" #文件夹目录
    files= os.listdir(path) #得到文件夹下的所有文件名称
    for file in files: #遍历文件夹
        position = path+'\\'+ file #构造绝对路径，"\\"，其中一个'\'为转义符          
        with open(position, "r",encoding='gbk',errors='ignore') as f:    #打开文件
            data0 = f.read()   #读取文件
            data0 = data0.replace('\n','')
            data0 = data0.replace('Subject:','')
            data0 = data0.replace(' -','') 
            record.append({'message': data0, 'class': 'spam'})
    data = data.append(DataFrame(record))
    
    return data

def train():  #训练贝叶斯分类器
    data = readdata()
    vectorizer = CountVectorizer()
    counts = vectorizer.fit_transform(data['message'].values)
    classifier = MultinomialNB()
    targets = data['class'].values
    classifier.fit(counts, targets)
    return classifier,vectorizer

def evluate(example = "watch this"):   #垃圾邮件评估
    classifier, vectorizer = train()
    examples = [example]
    example_counts = vectorizer.transform(examples)
    predictions = classifier.predict(example_counts)
    #print(predictions)
    if 'spam' in predictions:
        return 1
    else:
        return 0


#evluate("watch this")