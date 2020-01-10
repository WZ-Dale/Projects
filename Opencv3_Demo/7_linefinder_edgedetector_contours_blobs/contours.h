// 20200108
// 学习并注释 by wangze

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "E:\Projects\Opencv3_Demo\7_linefinder_edgedetector_contours_blobs\linefinder.h"
#include "E:\Projects\Opencv3_Demo\7_linefinder_edgedetector_contours_blobs\edgedetector.h"

#define PI 3.1415926

int contours_Test() {

	// Read input image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\road.jpg",0);
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);

	// Compute Sobel
	EdgeDetector ed;
	ed.computeSobel(image);

    // Display the Sobel orientation（方向，定向）
	cv::namedWindow("Sobel (orientation)");
	cv::imshow("Sobel (orientation)",ed.getSobelOrientationImage());
	cv::imwrite("E:\\Projects\\Opencv3_Demo\\7_linefinder_edgedetector_contours_blobs\\ori.bmp",ed.getSobelOrientationImage());

    // Display the Sobel low threshold（低阈值）
	cv::namedWindow("Sobel (low threshold)");
	cv::imshow("Sobel (low threshold)",ed.getBinaryMap(125));

    // Display the Sobel high threshold（高阈值）
	cv::namedWindow("Sobel (high threshold)");
	cv::imshow("Sobel (high threshold)",ed.getBinaryMap(350));

	// Apply Canny algorithm（应用Canny算子）
	cv::Mat contours;
	cv::Canny(image,contours,125,350);

    // Display the image of contours
	cv::namedWindow("Canny Contours");
	cv::imshow("Canny Contours",255-contours);

	// Create a test image
	cv::Mat test(200,200,CV_8U,cv::Scalar(0));
	cv::line(test,cv::Point(100,0),cv::Point(200,200),cv::Scalar(255));
	cv::line(test,cv::Point(0,50),cv::Point(200,200),cv::Scalar(255));
	cv::line(test,cv::Point(0,200),cv::Point(200,0),cv::Scalar(255));
	cv::line(test,cv::Point(200,0),cv::Point(0,200),cv::Scalar(255));
	cv::line(test,cv::Point(100,0),cv::Point(100,200),cv::Scalar(255));
	cv::line(test,cv::Point(0,100),cv::Point(200,100),cv::Scalar(255));

    // Display the test image
	cv::namedWindow("Test Image");
	cv::imshow("Test Image",test);
	cv::imwrite("E:\\Projects\\Opencv3_Demo\\7_linefinder_edgedetector_contours_blobs\\test.bmp",test);

	// Hough tranform for line detection（用霍夫变换检测直线）
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(contours,lines,1,PI/180,50);

	// Draw the lines
	cv::Mat result(contours.rows,contours.cols,CV_8U,cv::Scalar(255));
	image.copyTo(result);

	std::cout << "Lines detected: " << lines.size() << std::endl;

	std::vector<cv::Vec2f>::const_iterator it= lines.begin();
	while (it!=lines.end()) {

		float rho= (*it)[0];   // first element is distance rho（第一个元素是距离rho）
		float theta= (*it)[1]; // second element is angle theta（第二个元素是角度theta）
		
		if (theta < PI/4. || theta > 3.*PI/4.) { // ~vertical line（垂直线（大致））
		
			// point of intersection of the line with first row（直线与第一行的交叉点）
			cv::Point pt1(rho/cos(theta),0);        
			// point of intersection of the line with last row（直线与最后一行的交叉点）
			cv::Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows);
			// draw a white line（画白色线）
			cv::line( result, pt1, pt2, cv::Scalar(255), 1); 

		} else { // ~horizontal line（水平线（大致））

			// point of intersection of the line with first column（直线与第一列的交叉点）
			cv::Point pt1(0,rho/sin(theta));        
			// point of intersection of the line with last column（直线与最后一列的交叉点）
			cv::Point pt2(result.cols,(rho-result.cols*cos(theta))/sin(theta));
			// draw a white line（画白色线）
			cv::line( result, pt1, pt2, cv::Scalar(255), 1); 
		}

		std::cout << "line: (" << rho << "," << theta << ")\n"; 

		++it;
	}

    // Display the detected line image
	cv::namedWindow("Lines with Hough");
	cv::imshow("Lines with Hough",result);

	// Create LineFinder instance（创建LineFinder类的实例）
	LineFinder ld;

	// Set probabilistic Hough parameters（设置概率霍夫变换的参数）
	ld.setLineLengthAndGap(100,20);
	ld.setMinVote(60);

	// Detect lines（检测直线并画线）
	std::vector<cv::Vec4i> li= ld.findLines(contours);
	ld.drawDetectedLines(image);
	cv::namedWindow("Lines with HoughP");
	cv::imshow("Lines with HoughP",image);

	std::vector<cv::Vec4i>::const_iterator it2= li.begin();
	while (it2!=li.end()) {

		std::cout << "(" << (*it2)[0] << ","<< (*it2)[1]<< ")-(" 
			     << (*it2)[2]<< "," << (*it2)[3] << ")" <<std::endl;

		++it2;
	}

	// Display one line
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\road.jpg",0);

	// 选用直线0
	int n = 0;
	cv::line(image, cv::Point(li[n][0],li[n][1]),cv::Point(li[n][2],li[n][3]),cv::Scalar(255),5);
	cv::namedWindow("One line of the Image");
	cv::imshow("One line of the Image",image);

	// Extract the contour pixels of the first detected line（黑色图像，白色直线）
	cv::Mat oneline(image.size(),CV_8U,cv::Scalar(0));
	cv::line(oneline, cv::Point(li[n][0],li[n][1]),cv::Point(li[n][2],li[n][3]),cv::Scalar(255),3);// 直线宽度
	// 轮廓与白色直线进行与运算
	cv::bitwise_and(contours,oneline,oneline);
	cv::namedWindow("One line");
	cv::imshow("One line",255-oneline);

	std::vector<cv::Point> points;

	// Iterate over the pixels to obtain all point positions（遍历像素，得到所有点的位置）
	for( int y = 0; y < oneline.rows; y++ ) {
    
		uchar* rowPtr = oneline.ptr<uchar>(y);
    
		for( int x = 0; x < oneline.cols; x++ ) {

		    // if on a contour（列x，如果在轮廓上）
			if (rowPtr[x]) {

				points.push_back(cv::Point(x,y));
			}
		}
    }
	
	// find the best fitting line
	cv::Vec4f line;
	cv::fitLine(points, line, cv::DIST_L2, 0, 0.01, 0.01);// 距离类型，L2距离不用这个参数，精度

	std::cout << "line: (" << line[0] << "," << line[1] << ")(" << line[2] << "," << line[3] << ")\n"; 

	int x0= line[2]; // a point on the line（直线上的一点）
	int y0= line[3];
	int x1= x0+100*line[0]; // add a vector of length 100（加上长度为100的向量（用单位向量生成））
	int y1= y0+100*line[1];
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\road.jpg",0);

	// draw the line（绘制这条直线）
	cv::line(image,cv::Point(x0,y0),cv::Point(x1,y1),0,2);// 颜色和宽度
	cv::namedWindow("Fitted line");
	cv::imshow("Fitted line",image);

	// eliminate inconsistent lines
	ld.removeLinesOfInconsistentOrientations(ed.getOrientation(),0.4,0.1);

   // Display the detected line image
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\road.jpg",0);

	ld.drawDetectedLines(image);
	cv::namedWindow("Detected Lines (2)");
	cv::imshow("Detected Lines (2)",image);

	// Create a Hough accumulator（创建霍夫累加器，这里的图像类型为uchar，实际使用时应该用int）
	cv::Mat acc(200,180,CV_8U,cv::Scalar(0));

	// Choose a point（选取一个像素点）
	int x=50, y=30;

	// loop over all angles（循环遍历所用角度）
	for (int i=0; i<180; i++) {

		double theta= i*PI/180.;

		// find corresponding rho value （找到对应的rho值，j对应-100~100的rho）
		double rho= x*std::cos(theta)+y*std::sin(theta);
		int j= static_cast<int>(rho+100.5);

		std::cout << i << "," << j << std::endl;

		// increment accumulator（增值累加器）
		acc.at<uchar>(j,i)++;
	}

	// draw the axes
	cv::line(acc, cv::Point(0,0), cv::Point(0,acc.rows-1), 255);	
	cv::line(acc, cv::Point(acc.cols-1,acc.rows-1), cv::Point(0,acc.rows-1), 255);	
	
	cv::imwrite("E:\\Projects\\Opencv3_Demo\\7_linefinder_edgedetector_contours_blobs\\hough1.bmp",255-(acc*100));

	// Choose a second point
	x=30, y=10;

	// loop over all angles
	for (int i=0; i<180; i++) {

		double theta= i*PI/180.;
		double rho= x*cos(theta)+y*sin(theta);
		int j= static_cast<int>(rho+100.5);

		acc.at<uchar>(j,i)++;
	}

	cv::namedWindow("Hough Accumulator");
	cv::imshow("Hough Accumulator",acc*100);
	cv::imwrite("E:\\Projects\\Opencv3_Demo\\7_linefinder_edgedetector_contours_blobs\\hough2.bmp",255-(acc*100));

	// Detect circles
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\chariot.jpg",0);

	cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, 
		2,   // accumulator resolution (size of the image / 2) （累加器分辨率（图像尺寸/2））
		20,  // minimum distance between two circles（两个圆之间的最小距离）
		200, // Canny high threshold （Canny算子的高阈值）
		60, // minimum number of votes （最小投票数）
		15, 50); // min and max radius（最小和最大半径）

	std::cout << "Circles: " << circles.size() << std::endl;
	
	// Draw the circles
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\chariot.jpg",0);

	std::vector<cv::Vec3f>::const_iterator itc = circles.begin();
	
	while (itc!=circles.end()) {
		
	  cv::circle(image, 
		  cv::Point((*itc)[0], (*itc)[1]), // circle centre（圆心）
		  (*itc)[2], // circle radius（半径）
		  cv::Scalar(255), // color （颜色）
		  2); // thickness（厚度）
		
	  ++itc;	
	}

	cv::namedWindow("Detected Circles");
	cv::imshow("Detected Circles",image);

	cv::waitKey();
	return 0;
}