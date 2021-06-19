# import the necessary packages
from scipy.spatial import distance as dist
from imutils import perspective
from imutils import contours
import numpy as np
import argparse
import imutils
from cv2 import cv2


# 计算中点
def midpoint(ptA, ptB):
    return ((ptA[0] + ptB[0]) * 0.5, (ptA[1] + ptB[1]) * 0.5)

# construct the argument parse and parse the arguments（构造参数解析并解析参数）
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, help="path to the input image")
ap.add_argument("-w", "--width", type=float, required=True, help="width of the left-most object in the image (in inches)")
args = vars(ap.parse_args())

####################################################################################################################################

# load the image, convert it to grayscale, and blur it slightly（加载图像，将其转换为灰度，并略微模糊它）
image = cv2.imread(args["image"])
BGR = cv2.imread("D:\\Tire_Test\\image4\\p1.jpg")
gray = cv2.cvtColor(BGR, cv2.COLOR_BGR2GRAY)
# gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
# gray = cv2.GaussianBlur(gray, (7, 7), 0)
cv2.imshow("gray", gray)
cv2.imwrite("D:\\Tire_Test\\image4\\gray.jpg", gray)

# perform edge detection, then perform a dilation + erosion to（执行边缘检测，然后执行膨胀+侵蚀到）
# close gaps in between object edges（关闭对象边缘之间的间隙）
edged = cv2.Canny(gray, 50, 100)
cv2.imshow("edge", edged)
edged = cv2.dilate(edged, None, iterations=1)
edged = cv2.erode(edged, None, iterations=1)
cv2.imshow("edged", edged)

# find contours in the edge map（在边缘图中找到轮廓）
cnts = cv2.findContours(edged.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
# print(cnts,type(cnts))    # list 所有轮廓像素的集合

# sort the contours from left-to-right and initialize the（对轮廓从左到右排序，并初始化）
# 'pixels per metric' calibration variable（“单位像素”校正变量）
(cnts, _) = contours.sort_contours(cnts)
pixelsPerMetric = None
# print(cnts,type(cnts))    # tuple 排序后的元组

####################################################################################################################################
# while True:
# loop over the contours individually（分别绕着轮廓线）
for c in cnts:
# if the contour is not sufficiently large, ignore it（如果轮廓不够大，忽略它）
    if cv2.contourArea(c) < 100:
        continue

    # compute the rotated bounding box of the contour（计算轮廓的旋转包围盒）
    orig = image.copy()
    box = cv2.minAreaRect(c)    # 求出在上述点集下的最小面积矩形
    box = cv2.cv.BoxPoints(box) if imutils.is_cv2() else cv2.boxPoints(box)    # 画最小外接矩形
    box = np.array(box, dtype="int")

    # order the points in the contour such that they appear
    # in top-left, top-right, bottom-right, and bottom-left
    # order, then draw the outline of the rotated bounding
    # box（对轮廓中的点进行排序，使它们按左上、右上、右下和左下的顺序出现，然后绘制旋转后的包围框的轮廓）
    box = perspective.order_points(box)
    cv2.drawContours(orig, [box.astype("int")], -1, (0, 255, 0), 2)

    # loop over the original points and draw them（在原来的点上循环并画出它们）
    for (x, y) in box:
        cv2.circle(orig, (int(x), int(y)), 5, (0, 0, 255), -1)

        ####################################################################################################################################

        # unpack the ordered bounding box, then compute the midpoint
        # between the top-left and top-right coordinates, followed by
        # the midpoint between bottom-left and bottom-right coordinates
        #（解压缩有序边界框，然后计算左上角和右上角坐标之间的中点，以及左下角和右下角坐标之间的中点）
        (tl, tr, br, bl) = box
        (tltrX, tltrY) = midpoint(tl, tr)
        (blbrX, blbrY) = midpoint(bl, br)

        # compute the midpoint between the top-left and top-right points,
        # followed by the midpoint between the top-righ and bottom-right
        #（计算左上点和右上点之间的中点，以及右上点和右下点之间的中点）
        (tlblX, tlblY) = midpoint(tl, bl)
        (trbrX, trbrY) = midpoint(tr, br)

        # draw the midpoints on the image（在图像上画中点）
        cv2.circle(orig, (int(tltrX), int(tltrY)), 5, (255, 0, 0), -1)
        cv2.circle(orig, (int(blbrX), int(blbrY)), 5, (255, 0, 0), -1)
        cv2.circle(orig, (int(tlblX), int(tlblY)), 5, (255, 0, 0), -1)
        cv2.circle(orig, (int(trbrX), int(trbrY)), 5, (255, 0, 0), -1)

        # draw lines between the midpoints（在中点之间画线）
        cv2.line(orig, (int(tltrX), int(tltrY)), (int(blbrX), int(blbrY)), (255, 0, 255), 2)
        cv2.line(orig, (int(tlblX), int(tlblY)), (int(trbrX), int(trbrY)), (255, 0, 255), 2)

        ####################################################################################################################################

        # compute the Euclidean distance between the midpoints（计算中点之间的欧氏距离）
        dA = dist.euclidean((tltrX, tltrY), (blbrX, blbrY))
        dB = dist.euclidean((tlblX, tlblY), (trbrX, trbrY))

        # if the pixels per metric has not been initialized, then
        # compute it as the ratio of pixels to supplied metric
        # (in this case, inches)
        #（如果每个度量的像素没有初始化，那么将其计算为像素与提供的度量的比率(在本例中为英寸)）
        if pixelsPerMetric is None:
            # pixelsPerMetric = dB / args["width"]
            pixelsPerMetric = args["width"]

        ####################################################################################################################################

        # compute the size of the object（计算对象的大小）
        dimA = dA / pixelsPerMetric
        dimB = dB / pixelsPerMetric

        # draw the object sizes on the image（在图像上绘制对象大小）
        cv2.putText(orig, "{:.1f}mm".format(dimA),
        (int(tltrX - 15), int(tltrY - 10)), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (255, 255, 0), 2)
        cv2.putText(orig, "{:.1f}mm".format(dimB), (int(trbrX + 10), int(trbrY)), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (255, 255, 0), 2)

        # show the output image（显示输出图像）
    cv2.imshow("Image", orig)
    cv2.waitKey(1000)
