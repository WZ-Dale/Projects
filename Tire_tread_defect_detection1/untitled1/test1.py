"""canny边缘检测"""

import cv2
import numpy as np
from matplotlib import pyplot as plt


def nothing(x):
    pass


cv2.namedWindow('res')
cv2.namedWindow('resbar')
cv2.createTrackbar('max', 'resbar', 170, 255, nothing)
cv2.createTrackbar('min', 'resbar', 80, 255, nothing)

img = cv2.imread('4a.jpg', 0)

maxVal = 200
minVal = 100

while (1):

    if cv2.waitKey(20) & 0xFF == 27:
        break
    minVal = cv2.getTrackbarPos('max', 'resbar')
    maxVal = cv2.getTrackbarPos('min', 'resbar')
    if minVal < maxVal:
        edge = cv2.Canny(img, 100, 200)
        cv2.imshow('res', edge)
    else:
        edge = cv2.Canny(img, minVal, maxVal)
        cv2.imshow('res', edge)
cv2.destoryAllWindows()
