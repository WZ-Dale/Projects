// 20191216
// 学习并注释 by wangze

#pragma once

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


int extractObject_Test() {

	// Read input image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg");
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);

	// 定义一个带边框的矩形
	// 矩形外部的像素会被标记为背景
	// define bounding rectangle 
	cv::Rect rectangle(50,25,210,180);

	// 模型（内部使用）
	// the models (internally used)
	cv::Mat bgModel,fgModel; 
	// 分割结果（四种可能的值）
	// segmentation result
	cv::Mat result; // segmentation (4 possible values)

	// GrabCut分割算法，计算量大，但结果很精确
	// GrabCut segmentation
	cv::grabCut(image,    // input image					// 输入图像
		result,   // segmentation result					// 分割结果
		rectangle,// rectangle containing foreground		// 包含前景的矩形
		bgModel, fgModel, // models							// 模型
		5,        // number of iterations					// 迭代次数
		cv::GC_INIT_WITH_RECT); // use rectangle			// 使用矩形，表示将使用带边框的矩形模型

	// 取得标记为“可能属于前景”的像素
	// Get the pixels marked as likely foreground
	cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
	// or: 使用按位与的运算检查第一位，如果是前景色结果为1
    //	result= result&1;
	
	// 生成输出图像
	// create a white image
	cv::Mat foreground(image.size(), CV_8UC3,
	          	       cv::Scalar(255, 255, 255));
	// 不复制背景像素
	image.copyTo(foreground,result); // bg pixels not copied

	// draw rectangle on original image
	cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);
	cv::namedWindow("Image with rectangle");
	cv::imshow("Image with rectangle",image);

	// display result
	cv::namedWindow("Foreground object");
	cv::imshow("Foreground object",foreground);

	cv::waitKey();
	return 0;
}
