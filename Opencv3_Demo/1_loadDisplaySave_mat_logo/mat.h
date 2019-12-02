// 20191129
// 学习并注释 by wangze
#pragma once

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// test function that creates an image
cv::Mat matFunction() {

	// create image
	cv::Mat ima(500, 500, CV_8U, 50);
	// return it
	return ima;
}

int mat_Test() {

	// create a new image made of 240 rows and 320 columns
	cv::Mat image1(240, 320, CV_8U, 100);
	// or:
	// cv::Mat image1(240,320,CV_8U,cv::Scalar(100));
	cv::imshow("Image", image1); // show the image
	cv::waitKey(0); // wait for a key pressed

	// re-allocate a new image
	// (only if size or type are different)
	image1.create(200, 200, CV_8U);
	image1 = 200;
	cv::imshow("Image", image1); // show the image
	cv::waitKey(0); // wait for a key pressed

	// 创建三通道图，初始颜色为红色
	// create a red color image
	// channel order is BGR
	cv::Mat image2(240, 320, CV_8UC3, cv::Scalar(0, 0, 255));
	// or:
	// cv::Mat image2(cv::Size(320,240),CV_8UC3);
	// image2= cv::Scalar(0,0,255);
	cv::imshow("Image", image2); // show the image
	cv::waitKey(0); // wait for a key pressed

	// read an image
	cv::Mat image3 = cv::imread("E:\\Projects\\Opencv3_Demo\\images\\puppy.bmp");

	// 浅拷贝
	// all these images point to the same data block
	cv::Mat image4(image3);
	image1 = image3;

	// 深拷贝
	// these images are new copies of the source image
	image3.copyTo(image2);
	cv::Mat image5 = image3.clone();

	// 转换图像，测试哪个图像是浅拷贝
	// transform the image for testing
	cv::flip(image3, image3, 1);
	// image1和image4会随着image3的改变而改变
	// check which images have been affected by the processing
	cv::imshow("Image 3", image3);
	cv::imshow("Image 1", image1);
	cv::imshow("Image 2", image2);
	cv::imshow("Image 4", image4);
	cv::imshow("Image 5", image5);
	cv::waitKey(0); // wait for a key pressed

	// 从函数中获取一个灰度图像
	// get a gray-level image from a function
	cv::Mat gray = matFunction();
	cv::imshow("Image", gray); // show the image
	cv::waitKey(0); // wait for a key pressed

	// 只读一个通道（灰度图）
	// read the image in gray scale
	image1 = cv::imread("E:\\Projects\\Opencv3_Demo\\images\\puppy.bmp", cv::IMREAD_GRAYSCALE);
	
	// 把一幅图像复制到另一幅图像中（两幅图像的数据类型不同时），使用该函数：
	// convert the image into a floating point image [0,1]
	image1.convertTo(image2, CV_32F, 1 / 255.0, 0.0);	// 将image1复制到浮点型图像中（两个参数为：缩放比例和偏移量），需要注意：这两幅图像的通道数必须一致
	cv::imshow("Image", image2); // show the image

	// 3x3双精度型矩阵
	// Test cv::Matx
	// a 3x3 matrix of double-precision
	cv::Matx33d matrix(3.0, 2.0, 1.0,
		2.0, 1.0, 3.0,
		1.0, 2.0, 3.0);
	// 3x1矩阵（即向量）
	// a 3x1 matrix (a vector)
	cv::Matx31d vector(5.0, 1.0, 3.0);
	// 矩阵相乘
	// multiplication
	cv::Matx31d result = matrix * vector;
	// 输出相乘后的结果
	std::cout << result;

	cv::waitKey(0); // wait for a key pressed
	return 0;
}
