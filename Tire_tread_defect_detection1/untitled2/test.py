# @License : 西安工业大学
# @Author  : 王泽 WZ-Dale
# @Time    : 2020/12/1 20:26

import numpy as np
import matplotlib.pyplot as plt
import cv2

def DrawRadon(radon, deg, num):
    """画Radon变换结果图"""
    x = np.linspace(0, num, num, endpoint=False)
    plt.plot(x, radon)
    plt.title('Radon' + deg)
    plt.xlabel('x')
    plt.ylabel('y')
    # plt.savefig('Radon' + deg + '.jpg')
    plt.show()
def Radon_0(image):
    """计算0度Radon变换结果，用于图像切割"""
    res = np.zeros((800), dtype='float64')
    for i in range(600):
        for j in range(800):
            res[j] += image[i][j]
    # DrawRadon(res, '0', 800)
    return res
def Radon_90(image):
    """计算90度Radon变换结果，用于检测缺块"""
    res = np.zeros((600), dtype='float64')
    for i in range(600):
        res[i] = sum(image[i])
    # DrawRadon(res, '90', 600)
    return res
def Cut(image, radon):
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
    img_lst = []
    for i in range(len(lst) - 1):
        img = image[0:, lst[i]:lst[i + 1] + 1]
        img_lst.append(img)
        # cv2.imshow('img' + str(i + 1), img)
    return lst, img_lst
def MissBlockDetect(image, radon, start, end):
    """根据Radon90度结果，统计查找缺块位置，并将缺块标记出来"""
    # line = 9000
    average = np.mean(radon)
    line = average * 0.9
    count = 0
    for i in range(600):
        if radon[i] > line and count < 60:
            count = 0
        elif i == 599 or (radon[i] < line and radon[i + 1] >= line):
            if count > 60:
                print(count)
                cv2.rectangle(image, (start, i - count), (end, i), (0, 0, 255), 2)
                cv2.putText(image, "MissingBlock", (start, i + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0), 2)
                count = 0
            else:
                count = 0
        elif radon[i] < line:
            count += 1
    # cv2.imshow("image", image)

if __name__ == '__main__':
    image = cv2.imread('00.jpg')
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    radon = Radon_0(gray)
    lst, img_lst = Cut(gray, radon)
    for i in range(len(img_lst)):
        radon = Radon_90(img_lst[i])
        MissBlockDetect(image, radon, lst[i], lst[i + 1])
    cv2.imshow("image", image)
    cv2.waitKey(0)