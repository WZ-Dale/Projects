from cv2 import cv2
import os 

"""简化版，读到文件名之后不保存，立即进行操作"""
for root,dirs,files in os.walk("D:/Tire_Test/code3/image/7/1", True):           # 遍历检索根、目录、文件
	for file in files:                                     # 文件遍历
		line = "D:/Tire_Test/code3/image/7/1/" + file
		print(line)
		image = cv2.imread(line)           # 读取文件名对应的图片
		# print(image.shape)
		# cropImg = image[0:1080:2, 240:1680:2]           # 先y范围，后x范围，每个的第三个为采样步长
		cropImg = image[50:600, 0:1280]                 # 裁剪
		cv2.imwrite("D:/Tire_Test/code3/image/7/2/" + line[29:], cropImg)       # 保存
