"""逻辑与操作"""

import cv2

image = cv2.imread('4a.jpg', 0)
max = cv2.imread('4f.jpg', 0)

and_demo = cv2.bitwise_and(image, max)

cv2.imshow('and_demo', and_demo)
cv2.waitKey(0)