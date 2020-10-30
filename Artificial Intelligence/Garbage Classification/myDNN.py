from keras.preprocessing.image import ImageDataGenerator
from keras.layers import Input, Dense, Flatten, Dropout, Activation
from keras.layers.normalization import BatchNormalization
from keras.models import Model
from keras.callbacks import TensorBoard
import time

from keras.models import Sequential
from keras.layers import SeparableConv2D
from keras.layers.convolutional import Conv2D
from keras.layers.convolutional import MaxPooling2D
from keras import optimizers
from keras.models import load_model
from keras.preprocessing import image
import numpy as np
import os

def preprocessing(data_path):
    """
    数据处理
    :param data_path: 数据集路径
    :return: train, test:处理后的训练集数据、测试集数据
    """
    # -------------------------- 实现数据处理部分代码 ----------------------------
    batch_size = 64
    width, height = 384, 512
    train_datagen = ImageDataGenerator(
            rescale=1./255,
            shear_range=0.2, zoom_range=0.2,
            width_shift_range=0.2, height_shift_range=0.2,
            horizontal_flip=True, vertical_flip=True,
            validation_split=0.1)
 
    test_datagen = ImageDataGenerator(rescale=1. / 255, validation_split=0.1)
    
    train_generator = train_datagen.flow_from_directory(
        data_path, target_size=(width, height), batch_size=batch_size,
        class_mode='categorical', subset='training', seed=0)
    
    validation_generator = test_datagen.flow_from_directory(
        data_path, target_size=(width, height), batch_size=batch_size,
        class_mode='categorical', subset='validation', seed=0)
    # ------------------------------------------------------------------------
    return train_generator, validation_generator


def mymodel(train_data, test_data, save_model_path, log_dir):
    """
    创建、训练和保存深度学习模型
    :param train_data: 训练集数据
    :param test_data: 测试集数据
    :param save_model_path: 保存模型的路径和名称
    :return:
    """
    # --------------------- 实现模型创建、训练和保存等部分的代码 ---------------------
    start = time.time()
    
    input_shape = (384, 512, 3)
    model = Sequential()
    model.add(SeparableConv2D(filters=32, kernel_size=3, activation='relu', padding='same', input_shape=input_shape))
    model.add(MaxPooling2D(pool_size=2, strides=2))

    model.add(SeparableConv2D(filters=64, kernel_size=3, activation='relu', padding='same'))
    model.add(BatchNormalization(axis=1))
    model.add(MaxPooling2D(pool_size=2, strides=2))

    model.add(SeparableConv2D(filters=128, kernel_size=3, activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=2, strides=2))

    model.add(SeparableConv2D(filters=64, kernel_size=3, activation='relu', padding='same'))
    model.add(BatchNormalization(axis=1))
    model.add(MaxPooling2D(pool_size=2, strides=2))

    model.add(SeparableConv2D(filters=32, kernel_size=3, activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=2, strides=2))

    model.add(Flatten())
    model.add(Dense(256, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(6, activation='softmax'))

    #myadam = optimizers.Adam(learning_rate=0.01)
    model.compile(loss='categorical_crossentropy', optimizer='Adam', metrics=['accuracy'])

    tensorboard = TensorBoard(log_dir)

    d = model.fit_generator(train_data, 
                    epochs=100, 
                    steps_per_epoch=len(train_data),
                    validation_data=test_data,
                    validation_steps=len(test_data),
                    callbacks=[tensorboard])

    model.save(save_model_path)
    # -------------------------------------------------------------------------

    return d, model

import matplotlib.pyplot as plt

def evaluate_mode(test_data, save_model_path, res):
    """
    加载模型和评估模型
    可以实现，比如: 模型训练过程中的学习曲线，测试集数据的loss值、准确率及混淆矩阵等评价指标！
    主要步骤:
        1.加载模型(请填写你训练好的最佳模型),
        2.对自己训练的模型进行评估

    :param test_data: 测试集数据
    :param save_model_path: 加载模型的路径和名称,请填写你认为最好的模型
    :return:
    """
    # ----------------------- 实现模型加载和评估等部分的代码 -----------------------
    # 绘制学习曲线
    # 绘制模型训练过程的损失和平均损失
    plt.figure(1)
    plt.plot(res.history['loss'], label='loss')
    plt.plot(res.history['val_loss'], label='val_loss')
    plt.legend(loc='upper right')
    plt.xlabel('epoch')
    plt.savefig('./results/evlpic/dnn27-loss.png')
    plt.show()
    plt.close(1)

    # 绘制模型训练过程中的的准确率和平均准确率
    plt.figure(2)
    plt.plot(res.history['accuracy'], label='acc')
    plt.plot(res.history['val_accuracy'], label='val_acc')
    plt.legend(loc='upper left')
    plt.xlabel('epoch')
    plt.ylim(0, 1)
    plt.savefig('./results/evlpic/dnn27-acc.png')
    plt.show()
    plt.close(2)
    
    model = load_model(save_model_path) # 加载模型
    loss, accuracy = model.evaluate_generator(test_data) # 获取验证集的 loss 和 accuracy
    print("\nLoss: %.2f, Accuracy: %.2f%%" % (loss, accuracy * 100))
    # ---------------------------------------------------------------------------


def main():
    """
    深度学习模型训练流程,包含数据处理、创建模型、训练模型、模型保存、评价模型等。
    如果对训练出来的模型不满意,你可以通过调整模型的参数等方法重新训练模型,直至训练出你满意的模型。
    如果你对自己训练出来的模型非常满意,则可以提交作业!
    :return:
    """
    data_path = "./datasets/la1ji1fe1nle4ishu4ju4ji22-momodel/dataset-resized"  # 数据集路径
    save_model_path = 'results/myDNN27.h5'  # 保存模型路径和名称
    log_path = './results/logs/dnn27' # 模型训练日志

    # 获取数据
    train_data, test_data = preprocessing(data_path)
    # 创建、训练和保存模型
    res, model = mymodel(train_data, test_data, save_model_path, log_path)
    # 评估模型
    evaluate_mode(test_data, save_model_path, res)


if __name__ == '__main__':
    main()


# -------------------------- 请加载您最满意的模型 ---------------------------

model1_path = 'results/myDNN27.h5'
model2_path = 'results/myDNN24.h5'
model3_path = 'results/myDNN26.h5'
model1 = load_model(model1_path)
model2 = load_model(model2_path)
model3 = load_model(model3_path)

# ---------------------------------------------------------------------------

def predict(img):
    """
    加载模型和模型预测
    主要步骤:
        1.图片处理
        2.用加载的模型预测图片的类别
    :param img: PIL.Image 对象
    :return: string, 模型识别图片的类别, 
            共 'cardboard','glass','metal','paper','plastic','trash' 6 个类别
    """
    # -------------------------- 实现模型预测部分的代码 ---------------------------

    # 获取图片的类别，共 'cardboard','glass','metal','paper','plastic','trash' 6 个类别
    img = image.img_to_array(img) # 把图片转换成为numpy数组
    img = 1.0/255 * img # 图片放缩
    # expand_dims的作用是把img.shape转换成(1, img.shape[0], img.shape[1], img.shape[2])
    x = np.expand_dims(img, axis=0) 

    # 模型预测
    y1 = model1.predict(x)
    y2 = model2.predict(x)
    y3 = model3.predict(x)
    y = (y1+y2+y3)/3

    labels = {0: 'cardboard', 1: 'glass', 2: 'metal', 3: 'paper', 4: 'plastic', 5: 'trash'}
    y_predict = labels[np.argmax(y)] # 获取输入图片的类别

    # -------------------------------------------------------------------------
    
    # 返回图片的类别
    return y_predict

# 输入图片路径和名称
img_path = 'test.jpg'

# 打印该张图片的类别
img = image.load_img(img_path)
print("照片里的垃圾为：" + predict(img))
