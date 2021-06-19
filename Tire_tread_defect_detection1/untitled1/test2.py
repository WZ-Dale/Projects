"""泛洪填充"""

#泛洪填充(彩色图像填充)
import cv2 as cv
import numpy as np
def fill_color_demo(image):
    copyImg = image.copy()
    h, w = image.shape[:2]
    mask = np.zeros([h+2, w+2],np.uint8)   #mask必须行和列都加2，且必须为uint8单通道阵列
    #为什么要加2可以这么理解：当从0行0列开始泛洪填充扫描时，mask多出来的2可以保证扫描的边界上的像素都会被处理
    cv.floodFill(copyImg, mask, (400, 300), (255, 255, 255), (100, 100, 100), (50, 50 ,50), cv.FLOODFILL_FIXED_RANGE)
    cv.imshow("fill_color_demo", copyImg)

src = cv.imread('4c.jpg')
# cv.namedWindow('input_image', cv.WINDOW_AUTOSIZE)
# cv.imshow('input_image', src)
fill_color_demo(src)
cv.waitKey(0)
cv.destroyAllWindows()