// 20191210
// 学习并注释 by wangze

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "E:\Projects\Opencv3_Demo\3_colorDetection_colordetector_extractObject_huesaturation\colordetector.h"

int colorDetection_Test() {
    // 1. Create image processor object						// 创建图像处理器
	ColorDetector cdetect;

    // 2. Read input image									// 读图并显示
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg");
	if (image.empty())
		return 0; 
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

    // 3. Set input parameters								// 设置输入参数
	cdetect.setTargetColor(230,190,130); // here blue sky	// 这里表示蓝天

    // 4. Process the image and display the result			// 处理图像并显示结果
	cv::namedWindow("result");
	cv::Mat result = cdetect.process(image);
	cv::imshow("result",result);


	// or using functor
	// here distance is measured with the Lab color space
	ColorDetector colordetector(230, 190, 130,  // color
		                             45, true); // Lab threshold
	cv::namedWindow("result (functor)");
	result = colordetector(image);
	cv::imshow("result (functor)",result);

	// 使用floodFill函数，实现相同功能
	// testing floodfill
	cv::floodFill(image,            // input/ouput image						// 输入/输出图像
		cv::Point(100, 50),         // seed point								// 起始点
		cv::Scalar(255, 255, 255),  // repainted color							// 填充颜色
		(cv::Rect*)0,  // bounding rectangle of the repainted pixel set			// 填充区域的边界矩形
		cv::Scalar(35, 35, 35),     // low and high difference threshold		// 偏差的最小/最大阈值
		cv::Scalar(35, 35, 35),     // most of the time will be identical		// 正差阈值，两个阈值通常相等
		cv::FLOODFILL_FIXED_RANGE); // pixels are compared to seed color		// 与起始点像素比较

	cv::namedWindow("Flood Fill result");
	result = colordetector(image);
	cv::imshow("Flood Fill result", image);

	// Creating artificial images to demonstrate color space properties
	cv::Mat colors(100, 300, CV_8UC3, cv::Scalar(100, 200, 150));
	cv::Mat range= colors.colRange(0, 100);
	range = range + cv::Scalar(10, 10, 10);
	range = colors.colRange(200, 300);
	range = range + cv::Scalar(-10, -10, 10);

	cv::namedWindow("3 colors");
	cv::imshow("3 colors", colors);

	cv::Mat labImage(100, 300, CV_8UC3, cv::Scalar(100, 200, 150));
	cv::cvtColor(labImage, labImage, CV_BGR2Lab);
	range = colors.colRange(0, 100);
	range = range + cv::Scalar(10, 10, 10);
	range = colors.colRange(200, 300);
	range = range + cv::Scalar(-10, -10, 10);
	cv::cvtColor(labImage, labImage, CV_Lab2BGR);

	cv::namedWindow("3 colors (Lab)");
	cv::imshow("3 colors (Lab)", colors);

	// brightness versus luminance
	cv::Mat grayLevels(100, 256, CV_8UC3);
	for (int i = 0; i < 256; i++) {
		grayLevels.col(i) = cv::Scalar(i, i, i);
	}

	range = grayLevels.rowRange(50, 100);
	cv::Mat channels[3];
	cv::split(range, channels);
	channels[1] = 128;
	channels[2] = 128;
	cv::merge(channels, 3, range);
	cv::cvtColor(range, range, CV_Lab2BGR);


	cv::namedWindow("Luminance vs Brightness");
	cv::imshow("Luminance vs Brightness", grayLevels);

	cv::waitKey();

	return 0;
}

