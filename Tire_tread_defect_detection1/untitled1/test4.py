"""最大连通域"""

import cv2
import numpy as np

def find_max_region(mask_sel):
    _, contours, hierarchy = cv2.findContours(mask_sel, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    # 找到最大区域并填充
    area = []
    for j in range(len(contours)):
        area.append(cv2.contourArea(contours[j]))

    max_idx = np.argmax(area)
    max_area = cv2.contourArea(contours[max_idx])
    # min_idx = np.argmin(area)
    # min_area = cv2.contourArea(contours[min_idx])
    # print(max_area)
    # print(min_area)
    for k in range(len(contours)):
        if k != max_idx:
            cv2.fillPoly(mask_sel, [contours[k]], 0)
    return mask_sel

img = cv2.imread('4e.jpg', 0)
img = find_max_region(img)

cv2.imshow('image', img)
cv2.waitKey(0)