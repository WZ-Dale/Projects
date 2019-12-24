// 20191223
// Ñ§Ï°²¢×¢ÊÍ by wangze

#pragma once

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\integral.h"

int integral_Test() {

	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\book.jpg",0);
	if (!image.data)
		return 0; 
	// rotate the image for easier display
	cv::transpose(image, image);
	cv::flip(image, image, 0);

	// display original image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);

	// using a fixed threshold 
	cv::Mat binaryFixed;
	cv::Mat binaryAdaptive;
	cv::threshold(image,binaryFixed,70,255,cv::THRESH_BINARY);

	// using as adaptive threshold
	int blockSize= 21; // size of the neighborhood
	int threshold=10;  // pixel will be compared to (mean-threshold)

	int64 time;
	time= cv::getTickCount();
	cv::adaptiveThreshold(image,           // input image
		                  binaryAdaptive,  // output binary image
						  255,             // max value for output
		                  cv::ADAPTIVE_THRESH_MEAN_C, // adaptive method
						  cv::THRESH_BINARY, // threshold type
						  blockSize,       // size of the block
						  threshold);      // threshold used
	time= cv::getTickCount()-time;
	std::cout << "time (adaptiveThreshold)= " << time << std::endl; 

	// compute integral image
	IntegralImage<int,1> integral(image);

	// test integral result
	std::cout << "sum=" << integral(18,45,30,50) << std::endl;
	cv::Mat test(image,cv::Rect(18,45,30,50));
	cv::Scalar t= cv::sum(test);
	std::cout << "sum test=" << t[0] << std::endl;

	cv::namedWindow("Fixed Threshold");
	cv::imshow("Fixed Threshold",binaryFixed);

	cv::namedWindow("Adaptive Threshold");
	cv::imshow("Adaptive Threshold",binaryAdaptive);

	cv::Mat binary= image.clone();

	time= cv::getTickCount();	  
	int nl= binary.rows; // number of lines
	int nc= binary.cols; // total number of elements per line
              
	// compute integral image
	cv::Mat iimage;
	cv::integral(image,iimage,CV_32S);

	// for each row
	int halfSize= blockSize/2;
	for (int j = halfSize; j<nl - halfSize - 1; j++) {

		// get the address of row j
		uchar* data = binary.ptr<uchar>(j);
		int* idata1 = iimage.ptr<int>(j - halfSize);
		int* idata2 = iimage.ptr<int>(j + halfSize + 1);

		// for pixel of a line
		for (int i = halfSize; i<nc - halfSize - 1; i++) {

			// compute sum
			int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize] -
				idata1[i + halfSize + 1] + idata1[i - halfSize]) / (blockSize*blockSize);

			// apply adaptive threshold
			if (data[i]<(sum - threshold))
				data[i] = 0;
			else
				data[i] = 255;
		}
	}

	// add white border
	for (int j = 0; j<halfSize; j++) {
		uchar* data = binary.ptr<uchar>(j);

		for (int i = 0; i<binary.cols; i++) {
			data[i] = 255;
		}
	}
	for (int j = binary.rows - halfSize-1; j<binary.rows; j++) {
		uchar* data = binary.ptr<uchar>(j);

		for (int i = 0; i<binary.cols; i++) {
			data[i] = 255;
		}
	}
	for (int j = halfSize; j<nl - halfSize - 1; j++) {
		uchar* data = binary.ptr<uchar>(j);

		for (int i = 0; i<halfSize; i++) {
			data[i] = 255;
		}
		for (int i = binary.cols-halfSize-1; i<binary.cols; i++) {
			data[i] = 255;
		}
	}

	time= cv::getTickCount()-time;
	std::cout << "time integral= " << time << std::endl; 

	cv::namedWindow("Adaptive Threshold (integral)");
	cv::imshow("Adaptive Threshold (integral)",binary);

	// adaptive threshold using image operators
	time= cv::getTickCount();
	cv::Mat filtered;
	cv::Mat binaryFiltered;
	// box filter compute avg of pixels over a rectangular region
	cv::boxFilter(image,filtered,CV_8U,cv::Size(blockSize,blockSize));
	// check if pixel greater than (mean + threshold)
	binaryFiltered= image>= (filtered-threshold);
	time= cv::getTickCount()-time;

	std::cout << "time filtered= " << time << std::endl; 

	cv::namedWindow("Adaptive Threshold (filtered)");
	cv::imshow("Adaptive Threshold (filtered)",binaryFiltered);

	cv::waitKey();
}
