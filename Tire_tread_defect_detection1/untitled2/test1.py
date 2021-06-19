# @License : 西安工业大学
# @Author  : 王泽 WZ-Dale
# @Time    : 2020/12/1 21:21

"""根据极值点截取图像"""

from scipy import ndimage
import numpy as np
import matplotlib.pyplot as plt
import imageio
from cv2 import cv2

def DiscreteRadonTransform(image):
    res = np.zeros((800), dtype='float64')
    for i in range(600):
        for j in range(800):
            res[j] += image[i][j]
    return res

image = cv2.imread("00.jpg")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
radon = DiscreteRadonTransform(gray)

x = np.linspace(0, 800, 800, endpoint = False)
print(x)
plt.plot(x, radon)
plt.title('Radon 0')
plt.xlabel('x')
plt.ylabel('radon')
plt.savefig("Radon0.jpg")
plt.show()

line = 30000
lst = []
l, r, m = 0, 0, 0
for i in range(800):
    # 初始位置
    if radon[i - 1] == 0 and radon[i] > 0:
        lst.append(i)
    # 结束位置
    elif radon[i] > 0 and radon[i + 1] == 0:
        lst.append(i)
        break
    # 中间位置
    elif (radon[i] >= line and radon[i + 1] <= line):
        l = i
    elif l != 0 and (radon[i - 1] <= line and radon[i] >= line):
        r = i
        m = (l + r) // 2
        lst.append(m)
        l, r = 0, 0
print(lst)

img1 = image[0:, lst[0]:lst[1] + 1]
img2 = image[0:, lst[1]:lst[2] + 1]
img3 = image[0:, lst[2]:lst[3] + 1]
img4 = image[0:, lst[3]:lst[4] + 1]
img5 = image[0:, lst[4]:lst[5] + 1]
img6 = image[0:, lst[5]:lst[6] + 1]


cv2.imshow("img1", img1)
cv2.imshow("img2", img2)
cv2.imshow("img3", img3)
cv2.imshow("img4", img4)
cv2.imshow("img5", img5)
cv2.imshow("img6", img6)

cv2.imwrite("img1.jpg", img1)

cv2.waitKey(0)