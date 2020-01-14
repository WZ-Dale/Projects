// 20200108
// 学习并注释 by wangze

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int blobs_Test() {

	// Read input binary image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\binaryGroup.bmp",0);
	if (!image.data)
		return 0; 

	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image",image);

	// findContours用于提取图像中连续区域的轮廓
	// Get the contours of the connected components（用于存储轮廓的向量）
    std::vector<std::vector<cv::Point> > contours;
	cv::findContours(image, 
		contours, // a vector of contours （存储轮廓的向量）
		cv::RETR_EXTERNAL,      // retrieve the external contours（检索外部轮廓）
		cv::CHAIN_APPROX_NONE); // retrieve all pixels of each contours（每个轮廓的全部像素）

	// Print contours' length
	std::cout << "Contours: " << contours.size() << std::endl;
    std::vector<std::vector<cv::Point> >::const_iterator itContours= contours.begin();
	for ( ; itContours!=contours.end(); ++itContours) {

		std::cout << "Size: " << itContours->size() << std::endl;
	}

	// drawContours可以在白色图像上画出那些区域的轮廓
	// draw black contours on white image（在白色图像上画黑色轮廓）
	cv::Mat result(image.size(),CV_8U,cv::Scalar(255));
	cv::drawContours(result,contours,
		-1, // draw all contours（画全部轮廓）
		cv::Scalar(0), // in black（用黑色画）
		2); // with a thickness of 2（宽度为2）

	cv::namedWindow("Contours");
	cv::imshow("Contours",result);

	// Eliminate too short or too long contours（删除太短或太长的轮廓）
	int cmin= 50;  // minimum contour length（最小轮廓长度）
	int cmax= 500; // maximum contour length（最长轮廓长度）
    std::vector<std::vector<cv::Point> >::iterator itc= contours.begin();
	// 针对所有轮廓
	while (itc!=contours.end()) {
		// 验证轮廓大小
		if (itc->size() < cmin || itc->size() > cmax)
			itc= contours.erase(itc);
		else 
			++itc;
	}

	// draw contours on the original image
	cv::Mat original= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\group.jpg");

	cv::drawContours(original,contours,
		-1, // draw all contours
		cv::Scalar(255,255,255), // in white
		2); // with a thickness of 2

	cv::namedWindow("Contours on Animals");
	cv::imshow("Contours on Animals",original);

	// Let's now draw black contours on white image
	result.setTo(cv::Scalar(255));
	cv::drawContours(result,contours,
		-1, // draw all contours
		 0, // in black
		 1); // with a thickness of 1

	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\binaryGroup.bmp",0);

	// testing the bounding box （测试边界框）
	cv::Rect r0= cv::boundingRect(contours[0]);
	// draw the rectangle（画矩形）
	cv::rectangle(result,r0, 0, 2);

	// testing the enclosing circle （测试覆盖圆）
	float radius;
	cv::Point2f center;
	cv::minEnclosingCircle(contours[1],center,radius);
	// draw the cricle（画圆形）
    cv::circle(result,center,static_cast<int>(radius), 0, 2);

	// testing the approximate polygon（测试多边形逼近）
	std::vector<cv::Point> poly;
	cv::approxPolyDP(contours[2],poly,5,true);
	// draw the polygon（画多边形）
	cv::polylines(result, poly, true, 0, 2);

	std::cout << "Polygon size: " << poly.size() << std::endl;

	// testing the convex hull（测试凸包）
	std::vector<cv::Point> hull;
	cv::convexHull(contours[3],hull);
	// draw the polygon（画多边形）
	cv::polylines(result, hull, true, 0, 2);
	
	std::vector<cv::Vec4i> defects;
	// cv::convexityDefects(contours[3], hull, defects);

	// testing the moments

	// iterate over all contours（测试轮廓矩，迭代遍历所有轮廓）
	itc= contours.begin();
	while (itc!=contours.end()) {

		// compute all moments（计算所有轮廓矩）
		cv::Moments mom= cv::moments(*itc++);

		// draw mass center（画重心）
		cv::circle(result,
			// position of mass center converted to integer（将重心位置转换成整数）
			cv::Point(mom.m10/mom.m00,mom.m01/mom.m00),
			2,cv::Scalar(0),2); // draw black dot（画黑点）
	}

	cv::namedWindow("Some Shape descriptors");
	cv::imshow("Some Shape descriptors",result);

	// New call to findContours but with RETR_LIST flag
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\binaryGroup.bmp",0);

	// Get the contours of the connected components
	cv::findContours(image, 
		contours, // a vector of contours （存放轮廓的向量）
		cv::RETR_LIST, // retrieve the external and internal contours（检索全部轮廓）
		cv::CHAIN_APPROX_NONE); // retrieve all pixels of each contours（全部像素）

	// draw black contours on white image
	result.setTo(255);
	cv::drawContours(result,contours,
	   -1,  // draw all contours
		0,  // in black
		2); // with a thickness of 2
	cv::namedWindow("All Contours");
	cv::imshow("All Contours",result);

	// get a MSER image
	cv::Mat components;
	components= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\mser.bmp",0);

	// create a binary version（创建二值图像）
	components= components==255;
	// open the image (white background)（打开图像，包含背景）
	cv::morphologyEx(components,components,cv::MORPH_OPEN,cv::Mat(),cv::Point(-1,-1),3);

	cv::namedWindow("MSER image");
	cv::imshow("MSER image",components);

	contours.clear();
	//invert image (background must be black)（翻转图像（背景必须是黑色的））
	cv::Mat componentsInv= 255-components;
	// Get the contours of the connected components（得到连续区域的轮廓）
	cv::findContours(componentsInv, 
		contours, // a vector of contours （轮廓的向量）
		cv::RETR_EXTERNAL, // retrieve the external contours（检索外部轮廓）
		cv::CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	// white image（白色图像）
	cv::Mat quadri(components.size(),CV_8U,255);

	// for all contours（针对全部轮廓）
    std::vector<std::vector<cv::Point> >::iterator it= contours.begin();
	while (it!= contours.end()) {
		poly.clear();
		// approximate contour by polygon（用多边形逼近轮廓）
		cv::approxPolyDP(*it,poly,5,true);
	
		// do we have a quadrilateral?（是否为四边形）
		if (poly.size()==4) {
	        // draw it（画出来）
			cv::polylines(quadri, poly, true, 0, 2);
		}
	
		++it;
	}

	cv::namedWindow("MSER quadrilateral");
	cv::imshow("MSER quadrilateral",quadri);

	cv::waitKey();
	return 0;
}
