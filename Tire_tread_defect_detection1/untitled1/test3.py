"""闭运算"""

import cv2

image = cv2.imread('4d.jpg')

image = cv2.dilate(image, None, iterations=3)
image = cv2.erode(image, None, iterations=3)

cv2.imshow('image', image)
cv2.waitKey(0)