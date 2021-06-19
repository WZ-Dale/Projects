"""伽玛变换(图像增强)"""

import cv2
import numpy as np
import sys

# 伽玛变换  power函数实现幂函数

if __name__ == "__main__":
    img = cv2.imread("4a.jpg", cv2.IMREAD_GRAYSCALE)
    # cv2.imwrite('4a0.jpg', img)
    # 归1
    Cimg = img / 255
    # 伽玛变换
    gamma = 0.5
    O = np.power(Cimg,gamma)
    #效果
    cv2.imshow('img',img)
    cv2.imshow('O',O)
    cv2.waitKey(0)
    cv2.destroyAllWindows()