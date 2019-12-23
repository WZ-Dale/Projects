// 20191221
// Ñ§Ï°²¢×¢ÊÍ by wangze

#pragma once

#include <iostream>
using namespace std;

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "E:\Projects\Opencv3_Demo\4_histogram_finder__imageComparator_integral\imageComparator.h"

int retrieve_Test() {
	// Read reference image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\waves.jpg");
	if (!image.data)
		return 0; 

	// Display image
	cv::namedWindow("Query Image");
	cv::imshow("Query Image",image);

	ImageComparator c;
	c.setReferenceImage(image);

	// Read an image and compare it with reference
	cv::Mat input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\dog.jpg");
	cout << "waves vs dog: " << c.compare(input) << endl;

	// Read an image and compare it with reference
	input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\marais.jpg");
	cout << "waves vs marais: " << c.compare(input) << endl;

	// Read an image and compare it with reference
	input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\bear.jpg");
	cout << "waves vs bear: " << c.compare(input) << endl;

	// Read an image and compare it with reference
	input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\beach.jpg");
	cout << "waves vs beach: " << c.compare(input) << endl;

	// Read an image and compare it with reference
	input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\polar.jpg");
	cout << "waves vs polar: " << c.compare(input) << endl;

	// Read an image and compare it with reference
	input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\moose.jpg");
	cout << "waves vs moose: " << c.compare(input) << endl;

	// Read an image and compare it with reference
	input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\lake.jpg");
	cout << "waves vs lake: " << c.compare(input) << endl;

	// Read an image and compare it with reference
	input= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\fundy.jpg");
	cout << "waves vs fundy: " << c.compare(input) << endl;

	cv::waitKey();
	return 0;
}