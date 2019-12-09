// 20191209
// 学习并注释 by wangze

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <math.h>

// 重映射图像，创建波浪效果
// remapping an image by creating wave effects
void wave(const cv::Mat &image, cv::Mat &result) {
	// 映射参数
	// the map functions
	cv::Mat srcX(image.rows,image.cols,CV_32F); // x-map
	cv::Mat srcY(image.rows,image.cols,CV_32F); // y-map
	// 创建映射参数
	// creating the mapping
	for (int i=0; i<image.rows; i++) {
		for (int j=0; j<image.cols; j++) {
			// （i，j）像素的新位置
			srcX.at<float>(i,j)= j;					// 保持在同一列
			srcY.at<float>(i,j)= i+3*sin(j/6.0);	// 原来在第i行的像素，现在根据一个正弦曲线移动
			
			// 水平翻转
			// horizontal flipping
			// srcX.at<float>(i,j)= image.cols-j-1;
			// srcY.at<float>(i,j)= i;
		}
	}
	// 应用映射参数
	// applying the mapping
	cv::remap(image,  // source image
		      result, // destination image
			  srcX,   // x map
			  srcY,   // y map
			  cv::INTER_LINEAR); // interpolation method	// 填补方法，插值法
}

int remapping_Test() {
	// open image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg",0);

	cv::namedWindow("Image");
	cv::imshow("Image",image);

	// remap image
	cv::Mat result, result1, result2, result3, result4;
	wave(image,result);
	wave(result, result1);
	wave(result1, result2);
	wave(result2, result3);
	wave(result3, result4);

	cv::namedWindow("Remapped image");
	cv::imshow("Remapped image",result4);

	cv::waitKey();
	return 0;
}


