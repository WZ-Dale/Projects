// 20191207
// 学习并注释 by wangze

#pragma once

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// 锐化函数
void sharpen(const cv::Mat &image, cv::Mat &result) {
	
	// 判断是否需要分配图像数据，若需要就分配
	result.create(image.size(), image.type()); // allocate if necessary
	int nchannels= image.channels();

	// 处理所有行，除了第一行和最后一行
	for (int j= 1; j<image.rows-1; j++) { // for all rows (except first and last)

		const uchar* previous= image.ptr<const uchar>(j-1); // previous row		// 上一行
		const uchar* current= image.ptr<const uchar>(j);	// current row		// 当前行
		const uchar* next= image.ptr<const uchar>(j+1);		// next row			// 下一行

		uchar* output= result.ptr<uchar>(j);	// output row			// 输出行

		// 处理当前行到输出图像
		for (int i=nchannels; i<(image.cols-1)*nchannels; i++) {

			// 应用锐化算子
			// apply sharpening operator
			*output++= cv::saturate_cast<uchar>(5*current[i]-current[i-nchannels]-current[i+nchannels]-previous[i]-next[i]); 
			// saturate_cast<uchar>模板函数的作用是将结果调整到0-255范围内
		}
	}

	// 将未处理的像素设置为0，即将最外圈的像素设为黑色
	// Set the unprocess pixels to 0
	result.row(0).setTo(cv::Scalar(0));
	result.row(result.rows-1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols-1).setTo(cv::Scalar(0));
	// row和col会返回一个单行ROI和一个单列ROI
	// setTo()方法可以对矩阵中的所有元素赋值
	// 如果矩阵中是三通道的彩图，需要使用cv::Scalar(a,b,c)指定三个数值，分别对每个通道进行赋值
}

// 锐化函数，迭代器版本
// same function but using iterator
// this one works only for gray-level image
void sharpenIterator(const cv::Mat &image, cv::Mat &result) {

	// 断言
	// must be a gray-level image
	CV_Assert(image.type() == CV_8UC1);

	// initialize iterators at row 1
	cv::Mat_<uchar>::const_iterator it= image.begin<uchar>()+image.cols;
	cv::Mat_<uchar>::const_iterator itend= image.end<uchar>()-image.cols;
	cv::Mat_<uchar>::const_iterator itup= image.begin<uchar>();
	cv::Mat_<uchar>::const_iterator itdown= image.begin<uchar>()+2*image.cols;

	// setup output image and iterator
	result.create(image.size(), image.type()); // allocate if necessary
	cv::Mat_<uchar>::iterator itout= result.begin<uchar>()+result.cols;

	for ( ; it!= itend; ++it, ++itout, ++itup, ++itdown) {

			*itout= cv::saturate_cast<uchar>(*it *5 - *(it-1)- *(it+1)- *itup - *itdown); 
	}

	// Set the unprocessed pixels to 0
	result.row(0).setTo(cv::Scalar(0));
	result.row(result.rows-1).setTo(cv::Scalar(0));
	result.col(0).setTo(cv::Scalar(0));
	result.col(result.cols-1).setTo(cv::Scalar(0));
}

// 使用卷积核心，滤波函数
// using kernel
void sharpen2D(const cv::Mat &image, cv::Mat &result) {

	// Construct kernel (all entries initialized to 0)
	cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
	// assigns kernel values
	kernel.at<float>(1,1)= 5.0;
	kernel.at<float>(0,1)= -1.0;
	kernel.at<float>(2,1)= -1.0;
	kernel.at<float>(1,0)= -1.0;
	kernel.at<float>(1,2)= -1.0;

	//filter the image
	cv::filter2D(image,result,image.depth(),kernel);
}

int contrast_Test() {
	// test sharpen function

	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg");
	if (!image.data)
		return 0; 

	cv::Mat result;

	double time= static_cast<double>(cv::getTickCount());
	sharpen(image, result);
	time= (static_cast<double>(cv::getTickCount())-time)/cv::getTickFrequency();
	std::cout << "time= " << time << std::endl;

	cv::namedWindow("Image");
	cv::imshow("Image",result);

	// test sharpenIterator

    // open the image in gray-level
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg",0);

	time = static_cast<double>(cv::getTickCount());
	sharpenIterator(image, result);
	time= (static_cast<double>(cv::getTickCount())-time)/cv::getTickFrequency();
	std::cout << "time 3= " << time << std::endl;

	cv::namedWindow("Sharpened Image");
	cv::imshow("Sharpened Image",result);

	// test sharpen2D

	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg");

	time = static_cast<double>(cv::getTickCount());
	sharpen2D(image, result);
	time= (static_cast<double>(cv::getTickCount())-time)/cv::getTickFrequency();
	std::cout << "time 2D= " << time << std::endl;

	cv::namedWindow("Image 2D");
	cv::imshow("Image 2D",result);

	cv::waitKey();

	return 0;
}


