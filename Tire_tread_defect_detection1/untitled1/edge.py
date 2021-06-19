# @License : 西安工业大学
# @Author  : 王泽 WZ-Dale
# @Time    : 2020/11/27 21:20

# coding=utf-8
import cv2
import numpy as np

img = cv2.imread("1b.jpg", 0)

# Sobel算子
# x = cv2.Sobel(img, cv2.CV_16S, 1, 0)
# y = cv2.Sobel(img, cv2.CV_16S, 0, 1)
# absX = cv2.convertScaleAbs(x)    # 转回uint8
# absY = cv2.convertScaleAbs(y)
# dst = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)
# cv2.imshow("absX", absX)
# cv2.imshow("absY", absY)
# cv2.imshow("Result", dst)

# Roberts算子
kernelx = np.array([[-1, 0], [0, 1]], dtype=int)
kernely = np.array([[0, -1], [1, 0]], dtype=int)
x = cv2.filter2D(img, cv2.CV_16S, kernelx)
y = cv2.filter2D(img, cv2.CV_16S, kernely)
absX = cv2.convertScaleAbs(x)    # 转回uint8
absY = cv2.convertScaleAbs(y)
rob = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)
cv2.imshow('Roberts', rob)

# Laplacian算子
# gray_lap = cv2.Laplacian(img, cv2.CV_16S, ksize=3)
# dst = cv2.convertScaleAbs(gray_lap)
# cv2.imshow('laplacian', dst)

# Canny算子
# img = cv2.GaussianBlur(img, (3, 3), 0)  # 用高斯平滑处理原图像降噪。
# canny = cv2.Canny(img, 50, 150)  # 最大最小阈值
# cv2.imshow('Canny', canny)


cv2.waitKey(0)
cv2.destroyAllWindows()
