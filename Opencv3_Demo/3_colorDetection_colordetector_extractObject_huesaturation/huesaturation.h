// 20191216
// 学习并注释 by wangze

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

#include <opencv2/imgproc/types_c.h>

// 一个基于数值区间（最小和最大色调、最小和最大饱和度）的函数
void detectHScolor(const cv::Mat& image,		// input image 
	double minHue, double maxHue,	// Hue interval 
	double minSat, double maxSat,	// saturation interval
	cv::Mat& mask) {				// output mask

	// 转换到HSV色彩空间
	// convert into HSV space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// 将3个通道分割进三幅图像中
	// split the 3 channels into 3 images
	std::vector<cv::Mat> channels;
	cv::split(hsv, channels);
	// channels[0] is the Hue				色调（主色，不同的颜色对应不同的色调值）
	// channels[1] is the Saturation		饱和度（颜色的鲜艳程度，柔和的颜色饱和度低）
	// channels[2] is the Value				亮度（主观属性，某种颜色的亮度程度）

	// Hue masking
	cv::Mat mask1; // below maxHue
	cv::threshold(channels[0], mask1, maxHue, 255, cv::THRESH_BINARY_INV);
	cv::Mat mask2; // over minHue
	cv::threshold(channels[0], mask2, minHue, 255, cv::THRESH_BINARY);

	cv::Mat hueMask; // hue mask
	if (minHue < maxHue)
		hueMask = mask1 & mask2;
	else // if interval crosses the zero-degree axis
		hueMask = mask1 | mask2;

	// Saturation masking
	// below maxSat
	cv::threshold(channels[1], mask1, maxSat, 255, cv::THRESH_BINARY_INV);
	// over minSat
	cv::threshold(channels[1], mask2, minSat, 255, cv::THRESH_BINARY);

	cv::Mat satMask; // saturation mask
	satMask = mask1 & mask2;

	// combined mask
	mask = hueMask&satMask;
}

int huesaturation_Test () {
	
	// read the image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg");
	if (!image.data)
		return 0; 

	// show original image
	cv::namedWindow("Original image");
	cv::imshow("Original image",image);

	// convert into HSV space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// split the 3 channels into 3 images
	std::vector<cv::Mat> channels;
	cv::split(hsv,channels);
	// channels[0] is the Hue				色调（主色，不同的颜色对应不同的色调值）
	// channels[1] is the Saturation		饱和度（颜色的鲜艳程度，柔和的颜色饱和度低）
	// channels[2] is the Value				亮度（主观属性，某种颜色的亮度程度）

	// display value
	cv::namedWindow("Value");
	cv::imshow("Value",channels[2]);

	// display saturation
	cv::namedWindow("Saturation");
	cv::imshow("Saturation",channels[1]);

	// display hue
	cv::namedWindow("Hue");
	cv::imshow("Hue",channels[0]);

	// image with fixed value
	cv::Mat newImage;		
	cv::Mat tmp(channels[2].clone());
	// 所有像素的颜色亮度通道将变为255
	// Value channel will be 255 for all pixels
	channels[2]= 255;
	// 重新合并通道
	// merge back the channels
	cv::merge(channels,hsv);
	// 转换回BGR
	// re-convert to BGR
	cv::cvtColor(hsv,newImage,CV_HSV2BGR);

	cv::namedWindow("Fixed Value Image");
	cv::imshow("Fixed Value Image",newImage);

	// image with fixed saturation
	channels[1]= 255;
	channels[2]= tmp;
	cv::merge(channels,hsv);
	cv::cvtColor(hsv,newImage,CV_HSV2BGR);

	cv::namedWindow("Fixed saturation");
	cv::imshow("Fixed saturation",newImage);

	// image with fixed value and fixed saturation
	channels[1]= 255;
	channels[2]= 255;
	cv::merge(channels,hsv);
	cv::cvtColor(hsv,newImage,CV_HSV2BGR);

	cv::namedWindow("Fixed saturation/value");
	cv::imshow("Fixed saturation/value",newImage);

	// 人工图像显示所有可能的HS颜色
	// artificial image shown the all possible HS colors
	cv::Mat hs(128, 360, CV_8UC3);  
	for (int h = 0; h < 360; h++) {
		for (int s = 0; s < 128; s++) {
			hs.at<cv::Vec3b>(s, h)[0] = h/2;     // all hue angles					所有色调角度
			hs.at<cv::Vec3b>(s, h)[1] = 255-s*2; // from high saturation to low		饱和度从高到低
			hs.at<cv::Vec3b>(s, h)[2] = 255;     // constant value					常数
		}
	}

	cv::cvtColor(hs, newImage, CV_HSV2BGR);

	cv::namedWindow("Hue/Saturation");
	cv::imshow("Hue/Saturation", newImage);


	// 测试皮肤检测
	// Testing skin detection

	// read the image
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\girl.jpg");
	if (!image.data)
		return 0; 

	// show original image
	cv::namedWindow("Original image");
	cv::imshow("Original image",image);

	// detect skin tone
	cv::Mat mask;
	detectHScolor(image, 
		160, 10, // hue from 320 degrees to 20 degrees 
		25, 166, // saturation from ~0.1 to 0.65
		mask);

	// show masked image
	cv::Mat detected(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	image.copyTo(detected, mask);
	cv::imshow("Detection result",detected);

	// A test comparing luminance and brightness

	// create linear intensity image
	cv::Mat linear(100,256,CV_8U);
	for (int i=0; i<256; i++) {

		linear.col(i)= i;
	}

	// create a Lab image
	linear.copyTo(channels[0]);
	cv::Mat constante(100,256,CV_8U,cv::Scalar(128));
	constante.copyTo(channels[1]);
	constante.copyTo(channels[2]);
	cv::merge(channels,image);

	// convert back to BGR
	cv::Mat brightness;
	cv::cvtColor(image,brightness, CV_Lab2BGR);
	cv::split(brightness, channels);

	// create combined image
	cv::Mat combined(200,256, CV_8U);
	cv::Mat half1(combined,cv::Rect(0,0,256,100));
	linear.copyTo(half1);
	cv::Mat half2(combined,cv::Rect(0,100,256,100));
	channels[0].copyTo(half2);

	cv::namedWindow("Luminance vs Brightness");
	cv::imshow("Luminance vs Brightness",combined);

	cv::waitKey();
}
