// 20200106
// 学习并注释 by wangze

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

int mserFeatures_Test() {

	// Read input image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\building.jpg",0);
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Image");
	cv::imshow("Image",image);
	
	// 基本的MSER检测器
	// basic MSER detector
	cv::Ptr<cv::MSER> ptrMSER= cv::MSER::create(5,     // delta value for local minima detection（局部检测是使用的增量值）
		                                        200,   // min acceptable area（允许的最小面积）
				                                2000); // max acceptable area（允许的最大面积）
	// 点集的容器
    // vector of point sets
	std::vector<std::vector<cv::Point> > points;
	// 矩形的容器
	// vector of rectangles
	std::vector<cv::Rect> rects;
	// 检测MSER特征
	// detect MSER features
	ptrMSER->detectRegions(image, points, rects);

	std::cout << points.size() << " MSERs detected" << std::endl;

	// 创建白色图像
	// create white image
	cv::Mat output(image.size(),CV_8UC3);
	output= cv::Scalar(255,255,255);
	
	// OpenCV随机数生成器
	// OpenCV random number generator
	cv::RNG rng;

	// 针对每个检测到的特征区域，在彩色区域显示MSER
	// 反向排序，先显示较大的MSER
	// Display the MSERs in color areas
	// for each detected feature
	// reverse order to display the larger MSER first
    for (std::vector<std::vector<cv::Point> >::reverse_iterator it= points.rbegin();
			   it!= points.rend(); ++it) {
		// 生成随机颜色
		// generate a random color
		cv::Vec3b c(rng.uniform(0,254),
			        rng.uniform(0,254),
					rng.uniform(0,254));

		std::cout << "MSER size= " << it->size() << std::endl;
		// 针对MSER集合中的每个点
		// for each point in MSER set
		for (std::vector<cv::Point>::iterator itPts= it->begin();
			                                  itPts!= it->end(); ++itPts) {
			// 不重新写MSER的像素
			//do not overwrite MSER pixels
			if (output.at<cv::Vec3b>(*itPts)[0]==255) {

				output.at<cv::Vec3b>(*itPts)= c;
			}
		}
	}
	
	cv::namedWindow("MSER point sets");
	cv::imshow("MSER point sets",output);
	cv::imwrite("E:\\Projects\\Opencv3_Demo\\5_morphology_segment_mserFeatures\\mser.bmp", output);

	// 提取并显示矩形的MSER
	// Extract and display the rectangular MSERs
	std::vector<cv::Rect>::iterator itr = rects.begin();
	std::vector<std::vector<cv::Point> >::iterator itp = points.begin();
	for (; itr != rects.end(); ++itr, ++itp) {
		// 检查两者比例
		// ratio test
		if (static_cast<double>(itp->size())/itr->area() > 0.6)
			cv::rectangle(image, *itr, cv::Scalar(255), 2);
	}

	// Display the resulting image
	cv::namedWindow("Rectangular MSERs");
	cv::imshow("Rectangular MSERs", image);
	
	// Reload the input image
	image = cv::imread("E:\\Projects\\Opencv3_Demo\\images\\building.jpg", 0);
	if (!image.data)
		return 0;
	// 提取并显示椭圆形的MSER
	// Extract and display the elliptic MSERs
	for (std::vector<std::vector<cv::Point> >::iterator it = points.begin();
	                                                    it != points.end(); ++it) {
		// 遍历MSER集合中的每个点
		// for each point in MSER set
		for (std::vector<cv::Point>::iterator itPts = it->begin();
		                                      itPts != it->end(); ++itPts) {
			// 提取封闭的矩形
			// Extract bouding rectangles
			cv::RotatedRect rr = cv::minAreaRect(*it);
			// 检测椭圆的长宽比
            // check ellipse elongation
			if (rr.size.height / rr.size.height > 0.6 || rr.size.height / rr.size.height < 1.6)
				cv::ellipse(image, rr, cv::Scalar(255), 2);
		} 
		// 这里没有死循环，只是多层循环导致运行慢
	}

	// Display the image
	cv::namedWindow("MSER ellipses");
	cv::imshow("MSER ellipses", image);

	/*
	// detection using mserFeatures class

	// create MSER feature detector instance
	MSERFeatures mserF(200,  // min area 
		               1500, // max area
					   0.5); // ratio area threshold
	                         // default delta is used

	// the vector of bounding rotated rectangles
	std::vector<cv::RotatedRect> rects;

	// detect and get the image
	cv::Mat result= mserF.getImageOfEllipses(image,rects);

	// display detected MSER
	cv::namedWindow("MSER regions");
	cv::imshow("MSER regions",result);
	*/
	cv::waitKey();
}
