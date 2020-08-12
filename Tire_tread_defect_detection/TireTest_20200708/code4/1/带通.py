# Writer : wojianxinygcl@163.com

# Data  : 2020.3.20

import cv2

import numpy as np

import math

from matplotlib import pyplot as plt

#读取图像

img = cv2.imread('D:/Tire_Test/code4/1/frame1013.jpg', 0)

#傅里叶变换

dft = cv2.dft(np.float32(img), flags = cv2.DFT_COMPLEX_OUTPUT)

fshift = np.fft.fftshift(dft)

#设置带通滤波器

# w 带宽

# radius: 带中心到频率平面原点的距离

rows, cols = img.shape

crow,ccol = int(rows/2), int(cols/2) #中心位置

w = 30

radius = 30

mask = np.ones((rows, cols, 2), np.uint8)

for i in range(0, rows):

    for j in range(0, cols):

        # 计算(i, j)到中心点的距离

        d = math.sqrt(pow(i - crow, 2) + pow(j - ccol, 2))

        if radius - w / 2 < d < radius + w / 2:

            mask[i, j, 0] = mask[i, j, 1] = 0

        else:

            mask[i, j, 0] = mask[i, j, 1] = 1

#掩膜图像和频谱图像乘积

f = fshift * mask

#傅里叶逆变换

ishift = np.fft.ifftshift(f)

iimg = cv2.idft(ishift)

res = cv2.magnitude(iimg[:,:,0], iimg[:,:,1])

#显示原始图像和带通滤波处理图像

plt.subplot(121), plt.imshow(img, 'gray'), plt.title('Original Image')

plt.axis('off')

plt.subplot(122), plt.imshow(res, 'gray'), plt.title('Band Pass Filter Image')

plt.axis('off')

plt.show()