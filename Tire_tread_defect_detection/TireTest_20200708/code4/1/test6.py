import random

import cv2 as cv
import numpy as np

from matplotlib import pyplot as plt

"""各种滤波器"""


path = 'D:/Tire_Test/code4/1/'
image = 'D:/Tire_Test/code4/1/frame1013.jpg'


def fft(image):
    """
    傅里叶变化
    :return:
    """
    img = cv.imread(image, 0)
    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)
    magnitude_spectrum = 20 * np.log(np.abs(fshift))
    f = 20 * np.log(np.abs(f))
    cv.imwrite(path + "magnitude_spectrum.png", magnitude_spectrum)
    cv.imwrite(path + "f.png", f)

def xiangwei(image):
    """
    相位
    :return:
    """
    img = cv.imread(image, 0)
    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)
    magnitude_spectrum = 20 * np.angle(fshift)
    f = 20 * np.angle(f)
    cv.imwrite(path + "xiangwei1.png", magnitude_spectrum)
    cv.imwrite(path + "xiangwei2.png", f)

def ifft(image):
    """
    逆傅里叶
    :return:
    """
    img = cv.imread(image, 0)
    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)

    f1shift = np.fft.ifftshift(fshift)
    img_back = np.fft.ifft2(f1shift)
    # 出来的是复数，无法显示
    img_back = np.abs(img_back)
    cv.imwrite(path + "img3_1back.png", img_back)

def gaotonglvbo(image):
    """
    高通滤波
    :return:
    """

    img_man = cv.imread(image, 0)
    rows, cols = img_man.shape
    h = int(rows / 2)
    v = int(cols / 2)
    mask = np.ones(img_man.shape, np.uint8)
    mask[h - 30:h + 30,v - 30:v + 30] = 0

    f1 = np.fft.fft2(img_man)
    f1shift = np.fft.fftshift(f1)
    f1shift = f1shift * mask
    f2shift = np.fft.ifftshift(f1shift)  # 对新的进行逆变换
    img_new = np.fft.ifft2(f2shift)
    # 出来的是复数，无法显示
    img_new = np.abs(img_new)
    # 调整大小范围便于显示
    # img_new = (img_new - np.amin(img_new)) / (np.amax(img_new) - np.amin(img_new))

    cv.imwrite(path + "gaotonglvbo.png", img_new)


def ditonglvbo(image):
    """
    低通滤波
    :return:
    """

    img_man = cv.imread(image, 0)
    rows, cols = img_man.shape
    h = int(rows / 2)
    v = int(cols / 2)
    mask = np.zeros(img_man.shape, np.uint8)
    mask[h - 30:h + 30,v - 30:v + 30] = 1

    f1 = np.fft.fft2(img_man)
    f1shift = np.fft.fftshift(f1)
    f1shift = f1shift * mask
    f2shift = np.fft.ifftshift(f1shift)  # 对新的进行逆变换
    img_new = np.fft.ifft2(f2shift)
    # 出来的是复数，无法显示
    img_new = np.abs(img_new)
    # 调整大小范围便于显示
    # img_new = (img_new - np.amin(img_new)) / (np.amax(img_new) - np.amin(img_new))

    cv.imwrite(path + "ditonglvbo.png", img_new)


def index_low_pass_kernel(img,D0,n=1):
    """
        指数滤波
        :return:
        """
    r,c = img.shape[1],img.shape[0]
    u = np.arange(r)
    v = np.arange(c)
    u, v = np.meshgrid(u, v)
    low_pass = np.sqrt((u - r / 2) ** 2 + (v - c / 2) ** 2)
    low_pass = np.exp(-1 * (D0 / low_pass) ** n)

    return low_pass

def index_high_pass_filter(image, D0=50,n=1):
    """
    指数滤波
    :param D0:
    :param n:
    :return:
    """
    src = cv.imread(image, 0)
    kernel = 1 - index_low_pass_kernel(src, D0, n)
    gray = np.float64(src)
    gray_fft = np.fft.fft2(gray)
    gray_fftshift = np.fft.fftshift(gray_fft)
    dst_filtered = kernel * gray_fftshift
    dst_ifftshift = np.fft.ifftshift(dst_filtered)
    dst_ifft = np.fft.ifft2(dst_ifftshift)
    dst = np.abs(np.real(dst_ifft))
    dst = np.clip(dst, 0, 255)
    cv.imwrite(path + "kernel.png", kernel * 255)
    cv.imwrite(path + "index_high_pass_filter.png", dst)

def batewo_low_pass_kernel(img,D0,n=1):
    """
    巴特沃
    :param img:
    :param D0:
    :param n:
    :return:
    """
    r,c = img.shape[1],img.shape[0]
    u = np.arange(r)
    v = np.arange(c)
    u, v = np.meshgrid(u, v)
    low_pass = np.sqrt((u - r / 2) ** 2 + (v - c / 2) ** 2)
    low_pass = 1 / (1 + (D0 / low_pass) ** (2 * n))

    return low_pass

def btwindex_high_pass_filter(image, D0=50,n=1):
    """
    巴特沃
    :param D0:
    :param n:
    :return:
    """
    src = cv.imread(image, 0)
    kernel = 1 - batewo_low_pass_kernel(src, D0, n)
    gray = np.float64(src)
    gray_fft = np.fft.fft2(gray)
    gray_fftshift = np.fft.fftshift(gray_fft)
    dst_filtered = kernel * gray_fftshift
    dst_ifftshift = np.fft.ifftshift(dst_filtered)
    dst_ifft = np.fft.ifft2(dst_ifftshift)
    dst = np.abs(np.real(dst_ifft))
    dst = np.clip(dst, 0, 255)
    cv.imwrite(path + "btwkernel.png", kernel * 255)
    cv.imwrite(path + "btwindex_high_pass_filter.png", dst)


def dct(image):
    """
    离散余弦
    :return:
    """
    img = cv.imread(image)
    img = img[:, :, 0]
    img = np.float32(img)  # 将数值精度调整为32位浮点型
    img_dct = cv.dct(img)  # 使用dct获得img的频域图像
    img_recor2 = cv.idct(img_dct)  # 使用反dct从频域图像恢复出原图像(有损)
    cv.imwrite(path + "dct.png", img_recor2)


def test():
    fft(image)
    xiangwei(image)
    ifft(image)
    gaotonglvbo(image)
    ditonglvbo(image)
    # index_low_pass_kernel(image, 50)
    index_high_pass_filter(image)
    # batewo_low_pass_kernel(image, 50)
    btwindex_high_pass_filter(image)
    dct(image)

test()