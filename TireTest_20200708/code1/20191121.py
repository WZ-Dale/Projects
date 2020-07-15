from cv2 import cv2
import numpy as np


image11 = cv2.imread("11.jpg", cv2.IMREAD_GRAYSCALE)
#cv2.imshow("image11", image11) 

# image11_1 = cv2.resize(image11,None,fx=0.2,fy=0.2,interpolation=cv2.INTER_AREA)

# 直接规定缩放大小，这个时候就不需要缩放因子
height, width = image11.shape[:2]
size = (int(width*0.2), int(height*0.2))
image11_1 = cv2.resize(image11, size, interpolation = cv2.INTER_AREA)
# cv2.imwrite("11_1.jpg", image11_1)
# cv2.imshow("image11_1", image11_1) 

# cv2.equalizeHist(image11_1, image11_1)
# cv2.imshow("image11_1", image11_1) 

edge_output = cv2.Canny(image11_1, 100, 200)
cv2.imshow("canny edge", edge_output)
dst = cv2.bitwise_and(image11_1, image11_1, mask=edge_output)
cv2.imshow("color edge", dst)

# element5 = np.array(5, 5, cv2.CV_8U, cv2.Scalar(1))
# cv2.morphologyEx(edge_output, image11_1, cv2.MORPH_OPEN, element5)

# rows, cols = image11_1.shape
# sobel_horizontal = cv2.Sobel(image11_1, cv2.CV_64F, 1, 0, ksize=5) #利用sobel算子进行边缘检测，x方向
# sobel_vertical = cv2.Sobel(image11_1, cv2.CV_64F, 0, 1, ksize=5) #利用sobel算子进行边缘检测，y方向
# kernel_1 = np.array([[-1,0,1], [-2,0,2], [-1,0,1]])   #利用图像卷积的方法进行边缘放大
# kernel_2 = np.array([[-1,-2,-1], [0,0,0], [1,2,1]])    #利用图像卷积的方法进行边缘放大
# output=cv2.filter2D(image11_1, -1,kernel=kernel_1)
# cv2.imshow('ker1',output)
# output2=cv2.filter2D(image11_1, -1,kernel=kernel_2)
# cv2.imshow('ker2',output2)
# cv2.imshow('Sobel horizontal', sobel_horizontal)
# cv2.imshow('Sobel vertical', sobel_vertical)




# kernel = np.ones((5,5), np.uint8)
# img_erosion = cv2.erode(edge_output, kernel, iterations=1)
# img_dilation = cv2.dilate(edge_output, kernel, iterations=1)
# cv2.imshow('Erosion', img_erosion)
# cv2.imshow('Dilation', img_dilation)



cv2.waitKey(0)
cv2.destroyAllWindows() 