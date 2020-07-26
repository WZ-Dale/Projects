#垂直投影
import numpy as np
from cv2 import cv2 as cv
img=cv.imread("D:/Tire_Test/code4/2/frame4.jpg",0)
ret,img2=cv.threshold(img,80,255,cv.THRESH_BINARY)

#返回图像的高和宽
(h,w)=img2.shape

#初始化一个跟图像宽一样长度的数组，用于记录每一列的黑点个数
a =[0 for z in range(0,w)]

for i in range(0,w):           #遍历每一列
    for j in range(0,h):       #遍历每一行
        if img2[j,i]==255:       #判断该点是否为黑点，0代表是黑点
            a[i]+=1            #该列的计数器加1
            # img2[j,i]=255      #记录完后将其变为白色，即等于255
        else:
            img2[j,i]=255
for i in range(0,w):           #遍历每一列
    for j in range(h-a[i],h):  #从该列应该变黑的最顶部的开始向最底部设为黑点
        img2[j,i]=0            #设为黑点
cv.imshow("img",img2)
cv.imwrite('D:/Tire_Test/code4/2/img2.jpg', img2)
cv.waitKey(0)
cv.destroyAllWindows()