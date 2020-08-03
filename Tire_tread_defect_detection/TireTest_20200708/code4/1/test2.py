from cv2 import cv2
import numpy as np

"""DCT"""

img = cv2.imread('D:/Tire_Test/code4/1/frame1013.jpg')
img = img[:, :, 0] # 获取rgb通道中的一个
print(img.shape)
cv2.imshow("1", img)
img = np.float32(img) # 将数值精度调整为32位浮点型
img_dct = cv2.dct(img)  # 使用dct获得img的频域图像
cv2.imshow("2", img_dct)    
print(img_dct.shape)
img_recor2 = cv2.idct(img_dct)  # 使用反dct从频域图像恢复出原图像(有损) 
cv2.imshow("3", img_recor2)  

cv2.waitKey(0)
cv2.destroyAllWindows()