// 20191219
// 学习并注释 by wangze

#pragma once

#include <iostream>
using namespace std;

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\histogram.h"

int histograms_Test() {

	// Read input image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\group.jpg",0);			// 以黑白方式打开
	if (!image.data)
		return 0; 

	// save grayscale image
	cv::imwrite("E:\\Projects\\Opencv3_Demo\\4_histogram_finder_imageComparator_integral\\groupBW.jpg", image);

    // Display the image
	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// 直方图对象
	// The histogram object
	Histogram1D h;

	// 计算直方图
    // Compute the histogram
	cv::Mat histo= h.getHistogram(image);

	// 循环遍历每个箱子
	// Loop over each bin
	for (int i=0; i<256; i++) 
		cout << "Value " << i << " = " << histo.at<float>(i) << endl;  

	// 以图像形式显示直方图
	// Display a histogram as an image
	cv::namedWindow("Histogram");
	cv::imshow("Histogram",h.getHistogramImage(image));


	// re-display the histagram with chosen threshold indicated（重新显示已选择阈值的直方图）
	cv::Mat hi = h.getHistogramImage(image);
	cv::line(hi, cv::Point(70, 0), cv::Point(70, 255), cv::Scalar(128));
	cv::namedWindow("Histogram with threshold value");
	cv::imshow("Histogram with threshold value", hi);

	// creating a binary image by thresholding at the valley
	cv::Mat thresholded; // output binary image（输出二值图像）
	cv::threshold(image,thresholded,
		          70,    // threshold value(阈值)
				  255,   // value assigned to pixels over threshold value（超出阈值的像素赋值）
				  cv::THRESH_BINARY); // thresholding type（阈值化类型）
 
	// 显示经过阈值处理的图像
	// Display the thresholded image
	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image",thresholded);
	thresholded = 255 - thresholded;
	cv::imwrite("E:\\Projects\\Opencv3_Demo\\4_histogram_finder_imageComparator_integral\\binary.bmp",thresholded);

	// 直方图均衡化
	// Equalize the image
	cv::Mat eq= h.equalize(image);

	// 直方图均衡化的图像
	// Show the result
	cv::namedWindow("Equalized Image");
	cv::imshow("Equalized Image",eq);

	// 均衡化后的直方图
	// Show the new histogram
	cv::namedWindow("Equalized H");
	cv::imshow("Equalized H",h.getHistogramImage(eq));

	// 拉伸图像，设置1%的像素为黑色和1%的像素为白色
	// Stretch the image, setting the 1% of pixels at black and 1% at white
	cv::Mat str= h.stretch(image,0.01f);

	// Show the result（拉伸后的图像）
	cv::namedWindow("Stretched Image");
	cv::imshow("Stretched Image",str);

	// 拉伸后的直方图
	// Show the new histogram
	cv::namedWindow("Stretched H");
	cv::imshow("Stretched H",h.getHistogramImage(str));

	// 创建一个图像反转的查找表
	// Create an image inversion table
	cv::Mat lut(1,256,CV_8U); // 1x256 matrix（256x1矩阵）

	// Or:
	// int dim(256);
	// cv::Mat lut(1,  // 1 dimension
	// 	&dim,          // 256 entries
	//	CV_8U);        // uchar

	for (int i=0; i<256; i++) {
		
		// 0 becomes 255, 1 becomes 254, etc.（0变成255,1变成254，以此类推）
		lut.at<uchar>(i)= 255-i;
	}

	// 反向图像
	// Apply lookup and display negative image
	cv::namedWindow("Negative image");
	cv::imshow("Negative image",h.applyLookUp(image,lut));

	cv::waitKey();
	return 0;
}

