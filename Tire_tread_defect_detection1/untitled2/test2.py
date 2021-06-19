# @License : 西安工业大学
# @Author  : 王泽 WZ-Dale
# @Time    : 2020/12/1 22:00

"""找缺块，画框"""

from scipy import ndimage
import numpy as np
import matplotlib.pyplot as plt
import imageio
from cv2 import cv2

def DiscreteRadonTransform1(image):
    res = np.zeros((600), dtype='float64')
    for i in range(600):
        res[i] = sum(image[i])
    return res

image = cv2.imread("01.jpg")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
radon = DiscreteRadonTransform1(gray)

x = np.linspace(0, 600, 600, endpoint = False)
print(x)
plt.plot(x, radon)
plt.title('Radon 90')
plt.xlabel('x')
plt.ylabel('radon')
plt.savefig("Radon90.jpg")
plt.show()

line = 9000
count = 0
for i in range(600):
    if radon[i] > line and count < 60:
        count = 0
    elif i == 599 or (radon[i] < line and radon[i + 1] >= line):
        if count > 60:
            print(count)
            cv2.rectangle(image, (0, i - count), (len(gray[0]), i), (0, 0, 255), 2)
            cv2.putText(image, "Tread defect", (0, i + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0), 2)
            count = 0
        else:
            count = 0
    elif radon[i] < line:
        count += 1
cv2.imshow("image", image)
cv2.waitKey(0)