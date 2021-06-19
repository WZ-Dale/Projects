import os
import sys
import cv2
import shutil
import numpy as np


#ImgPath = input("Please enter the path where the JPG is located: ") + '/'
#Savepath = 'D:/Tire_Test/code6/'


fx = 577.78055
cx = 372.23922
fy = 579.39692
cy = 299.19369
k1, k2, p1, p2, k3 = 0.00728, -0.02320, 0.00009, -0.00130, 0.00000

#相机坐标系到像素坐标系的转换矩阵
k = np.array([
    [fx, 0, cx],
    [0, fy, cy],
    [0, 0, 1]
])
#畸变系数
d = np.array([
    k1, k2, p1, p2, k3
])

#璜老板的三行去畸变
def undistort(img):
    h, w = img.shape[:2]
    mapx, mapy = cv2.initUndistortRectifyMap(k, d, None, k, (w, h), 5)
    return cv2.remap(img, mapx, mapy, cv2.INTER_LINEAR)

#os.mkdir(Savepath)
#print('\n' + 'Dictory D:/Tire_Test/code6/1/ has been created!' + '\n')
#imagelist = os.listdir(ImgPath)
#for image in imagelist:
image = "D:/Tire_Test/code6/11.jpg"
print(image)
img = cv2.imread(image)
cv2.imshow("原图", img)
img = undistort(img)
cv2.imshow("校正图", img)
cv2.imwrite("D:/Tire_Test/code6/12.jpg", img)
cv2.waitKey(0)