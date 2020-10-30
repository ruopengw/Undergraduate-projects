from matplotlib import pyplot as plt  # 展示图片
import numpy as np  # 数值处理
import cv2  # opencv库
from sklearn.linear_model import LinearRegression  # 回归分析
#from sklearn.preprocessing import PolynomialFeatures
from skimage.measure import compare_ssim as ssim
from scipy import spatial
import random
import time

def read_image(img_path):
    """
    读取图片，图片是以 np.array 类型存储
    :param img_path: 图片的路径以及名称
    :return: img np.array 类型存储
    """
    # 读取图片
    img = cv2.imread(img_path) 
    # 如果图片是三通道，采用 matplotlib 展示图像时需要先转换通道
    if len(img.shape) == 3:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        
    return img

def plot_image(image, image_title, is_axis=False):
    """
    展示图像
    :param image: 展示的图像，一般是 np.array 类型
    :param image_title: 展示图像的名称
    :param is_axis: 是否需要关闭坐标轴，默认展示坐标轴
    :return:
    """
    # 展示图片
    plt.imshow(image)
    # 关闭坐标轴,默认关闭
    if not is_axis:
        plt.axis('off')
    # 展示受损图片的名称
    plt.title(image_title)
    # 展示图片
    plt.show()

def save_image(filename, image):
    """
    将np.ndarray 图像矩阵保存为一张 png 或 jpg 等格式的图片
    :param filename: 图片保存路径及图片名称和格式
    :param image: 图像矩阵，一般为np.array
    :return:
    """
    # np.copy() 函数创建一个副本。
    # 对副本数据进行修改，不会影响到原始数据，它们物理内存不在同一位置。
    img = np.copy(image)
    # 从给定数组的形状中删除一维的条目
    img = img.squeeze()
    # 将图片数据存储类型改为 np.uint8
    if img.dtype == np.double:  
        # 若img数据存储类型是 np.double ,则转化为 np.uint8 形式
        img = img * np.iinfo(np.uint8).max
        # 转换图片数组数据类型
        img = img.astype(np.uint8)
    # 将 RGB 方式转换为 BGR 方式
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    # 生成图片
    cv2.imwrite(filename, img)

def normalization(image):
    """
    将数据线性归一化
    :param image: 图片矩阵，一般是np.array 类型 
    :return: 将归一化后的数据，在（0,1）之间
    """
    # 获取图片数据类型对象的最大值和最小值
    info = np.iinfo(image.dtype)
    # 图像数组数据放缩在 0-1 之间
    return image.astype(np.double) / info.max

def compute_error(res_img, img):
    """
    计算恢复图像 res_img 与原始图像 img 的 2-范数
    :param res_img:恢复图像 
    :param img:原始图像 
    :return: 恢复图像 res_img 与原始图像 img 的2-范数
    """
    # 初始化
    error = 0.0
    # 将图像矩阵转换成为np.narray
    res_img = np.array(res_img)
    img = np.array(img)
    # 如果2个图像的形状不一致，则打印出错误结果，返回值为 None
    if res_img.shape != img.shape:
        print("shape error res_img.shape and img.shape %s != %s" % (res_img.shape, img.shape))
        return None
    # 计算图像矩阵之间的评估误差
    error = np.sqrt(np.sum(np.power(res_img - img, 2)))
    
    return round(error,3)

def calc_ssim(img, img_noise):
    """
    计算图片的结构相似度
    :param img: 原始图片， 数据类型为 ndarray, shape 为[长, 宽, 3]
    :param img_noise: 噪声图片或恢复后的图片，
                      数据类型为 ndarray, shape 为[长, 宽, 3]
    :return:
    """
    return ssim(img, img_noise,
                multichannel=True,
                data_range=img_noise.max() - img_noise.min())

def calc_csim(img, img_noise):
    """
    计算图片的 cos 相似度
    :param img: 原始图片， 数据类型为 ndarray, shape 为[长, 宽, 3]
    :param img_noise: 噪声图片或恢复后的图片，
                      数据类型为 ndarray, shape 为[长, 宽, 3]
    :return:
    """
    img = img.reshape(-1)
    img_noise = img_noise.reshape(-1)
    return 1 - spatial.distance.cosine(img, img_noise)

def noise_mask_image(img, noise_ratio):
    """
    根据题目要求生成受损图片
    :param img: 图像矩阵，一般为 np.ndarray
    :param noise_ratio: 噪声比率，可能值是0.4/0.6/0.8
    :return: noise_img 受损图片, 图像矩阵值 0-1 之间，数据类型为 np.array, 
             数据类型对象 (dtype): np.double, 图像形状:(height,width,channel),通道(channel) 顺序为RGB
    """

    # -------------实现受损图像答题区域-----------------
    h,w,c = img.shape
    noise_img = np.zeros((h,w,c))
    
    for i in range(h):
        for j in range(w):
            for k in range(c):
                if random.random() < noise_ratio:
                    noise = 0
                else:
                    noise = 1
                noise_img[i][j][k] = float(img[i][j][k] * noise)
    #plt.imshow(noise_img)
    print("Noise image generated successfully!")
    #plt.imsave('./pictures/'+'A_n'+str(noise_ratio)+'.png', noise_img)

    # -----------------------------------------------

    return noise_img

def get_noise_mask(noise_img):
    """
    获取噪声图像，一般为 np.array
    :param noise_img: 带有噪声的图片
    :return: 噪声图像矩阵
    """
    # 将图片数据矩阵只包含 0和1,如果不能等于 0 则就是 1。
    return np.array(noise_img != 0, dtype='double')

def sample_mask(row, rows, col, cols, radius):
    # 确定处理区域的位置
    if row-radius < 0: #上边越界
        row_up = 0
        row_down = row + radius
    elif row+radius >= rows: #下边越界
        row_down = rows-1
        row_up = row - radius
    else:
        row_up = row - radius
        row_down = row + radius
    if col-radius < 0: #左边越界
        col_left = 0
        col_right = col + radius
    elif col+radius >= cols: #右边越界
        col_right = cols-1
        col_left = col - radius
    else:
        col_left = col - radius
        col_right = col + radius
    return row_up, row_down, col_left, col_right

def restore_image(noise_img, size=4):
    """
    使用 你最擅长的算法模型 进行图像恢复。
    :param noise_img: 一个受损的图像
    :param size: 输入区域半径，长宽是以 size*size 方形区域获取区域, 默认是 4
    :return: res_img 恢复后的图片，图像矩阵值 0-1 之间，数据类型为 np.array,
            数据类型对象 (dtype): np.double, 图像形状:(height,width,channel), 通道(channel) 顺序为RGB
    """
    # 恢复图片初始化，首先 copy 受损图片，然后预测噪声点的坐标后作为返回值。
    res_img = np.copy(noise_img)
   
    # 获取噪声图像
    noise_mask = get_noise_mask(noise_img)

    # -------------实现图像恢复代码答题区域----------------------------
    start_time = time.time()
    rows, cols, channel = noise_img.shape

    # 第一步：计算噪声率
    noise = 0
    for row in range(rows):
        for col in range(cols):
            for chan in range(channel):
                if noise_mask[row,col,chan] == 0.: #未被污染
                    noise += 1
    nr = 1.0*noise/rows/cols/channel
    print("计算得噪声率为: "+str(format(100.0*nr,'.2f'))+"%")
    high = 0
    if nr > 0.7:
        high = 1

    # 第二步：二元线性回归
    count = 0
    for row in range(rows):
        for col in range(cols):
            # 确定处理区域的位置
            row_up, row_down, col_left, col_right = sample_mask(row, rows, col, cols, 2)
            
            for chan in range(channel):
                if noise_mask[row,col,chan] != 0.: #未被污染
                    continue
                # 针对噪声点，采样回归样本
                x_sample=[]
                y_sample=[]
                for i in range(row_up,row_down+high):
                    for j in range(col_left,col_right+high):
                        if noise_mask[i,j,chan] == 0. or (i==row and j==col):
                            continue
                        x_sample.append([i,j])
                        y_sample.append([noise_img[i,j,chan]])
                if len(x_sample)==0:
                    continue
                # 二元线性回归
                model = LinearRegression()
                model.fit(x_sample, y_sample)
                res_img[row,col,chan] = model.predict([[row,col]])

            count += 1
            if count % 50000 == 0:
                print("正在二元线性回归修复: "+str(format(100.0*count/rows/cols,'.2f'))+"%")

           
    # 第三步：中值滤波（仅针对高污染图片）
    res_img2 = np.copy(res_img)
    if nr > 0.5:
        radius = 1
        if nr > 0.7:
            radius = 2
        count = 0
        for row in range(rows):
            for col in range(cols):
                # 确定处理区域的位置
                row_up, row_down, col_left, col_right = sample_mask(row, rows, col, cols, radius)
                # 处理各个通道
                for chan in range(channel):
                    if noise_mask[row,col,chan]!=0.: #未被污染
                        continue
                    # 采样未被完全修复的像素
                    mask=[]
                    for i in range(row_up,row_down+high):
                        for j in range(col_left,col_right+high):
                            mask.append(res_img[i,j,chan])
                    # 中值滤波，消除胡椒噪声
                    res_img2[row,col,chan] = np.median(mask)
                count += 1
                if count % 100000 == 0:
                    print("正在中值滤波修复: "+str(format(100.0*count/rows/cols,'.2f'))+"%")
    
    # 第四步：把估值限定在0-1内
    for row in range(rows):
        for col in range(cols):
            for chan in range(channel):
                if res_img2[row,col,chan] >= 1.0:
                    res_img2[row,col,chan] = 1.0
                elif res_img2[row,col,chan] <= 0.0:
                    res_img2[row,col,chan] = 0.0
    
    end_time = time.time()
    dtime = end_time - start_time
    print("Restore finished!")
    print("程序运行时间: %.7s s" % dtime)
    
    # ---------------------------------------------------------------

    return res_img2


def main():
    # 原始图片
    # 加载图片的路径和名称
    img_path = './pictures/A.png'
    #noise_img_path = './samples/forest_noise.png'

    # 读取原始图片
    img = read_image(img_path)
    #noise_img = read_image(noise_img_path)

    # 展示原始图片
    #plot_image(image=img, image_title="original image")

    # 图像数据归一化
    nor_img = normalization(img)
    #print("原像素：\n", img[0,0,:],img[100,100,:],img[200,200,:],img[300,300,:])
    #print("归一化原像素：\n", nor_img[0,0,:],nor_img[100,100,:],nor_img[200,200,:],nor_img[300,300,:])

    # 噪声比率
    noise_ratio = 0.4

    # 生成受损图片
    noise_img = noise_mask_image(nor_img, noise_ratio)
    #noise_img = normalization(noise_img)
    #print("受损像素：\n", noise_img[0,0,:],noise_img[100,100,:],noise_img[200,200,:],noise_img[300,300,:])

    if noise_img is not None:
        # 展示受损图片
        #plot_image(image=noise_img, image_title="the noise_ratio = %s of original image"%noise_ratio)

        # 恢复图片
        res_img = restore_image(noise_img)
        print("修复后像素：\n", res_img[0,0,:],res_img[100,100,:],res_img[200,200,:],res_img[300,300,:])

        # 计算受损图片与原始图片的误差
        print("受损图片与原始图片的 2-范数: ", compute_error(noise_img, nor_img))
        print("受损图片与原始图片的 SSIM 相似度: ", calc_ssim(noise_img, nor_img))
        print("受损图片与原始图片的 Cosine 相似度: ", calc_csim(noise_img, nor_img))

        # 计算恢复图片与原始图片的误差
        print("恢复图片与原始图片的 2-范数: ", compute_error(res_img, nor_img))
        print("恢复图片与原始图片的 SSIM 相似度: ", calc_ssim(res_img, nor_img))
        print("恢复图片与原始图片的 Cosine 相似度: ", calc_csim(res_img, nor_img))

        # 展示恢复图片
        plot_image(image=res_img, image_title="restore image")
        # 保存恢复图片
        save_image("./pictures/A_r"+str(noise_ratio)+'.png', res_img)

    else:
        # 未生成受损图片
        print("返回值是 None, 请生成受损图片并返回!")


if __name__ == '__main__':
    main()