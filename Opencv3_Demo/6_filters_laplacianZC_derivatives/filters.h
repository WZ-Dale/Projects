// 20200107
// 学习并注释 by wangze

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int filters_Test() {

	// Read input image
	cv::Mat image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg",0);
	if (!image.data)
		return 0; 

    // Display the image
	cv::namedWindow("Original Image");
	cv::imshow("Original Image",image);

	// 块滤波器
	// Blur the image with a mean filter
	cv::Mat result;
	cv::blur(image, result, cv::Size(5, 5));

	// Display the blurred image
	cv::namedWindow("Mean filtered Image");
	cv::imshow("Mean filtered Image", result);

	// Blur the image with a mean filter 9x9
	cv::blur(image, result, cv::Size(9, 9));

	// Display the blurred image
	cv::namedWindow("Mean filtered Image (9x9)");
	cv::imshow("Mean filtered Image (9x9)", result);
	
	// 高斯函数（钟形曲线）
	// Gaussian Blur the image
	cv::GaussianBlur(image,result,
		  cv::Size(5,5), // size of the filter（滤波器尺寸）
		  1.5);			 // parameter controlling the （控制高斯曲线形状的参数）
						 // shape of the Gaussian

    // Display the blurred image
	cv::namedWindow("Gaussian filtered Image");
	cv::imshow("Gaussian filtered Image",result);

	cv::GaussianBlur(image,result,cv::Size(9,9),1.7);

    // Display the blurred image
	cv::namedWindow("Gaussian filtered Image (9x9)");
	cv::imshow("Gaussian filtered Image (9x9)",result);

	// Get the gaussian kernel (1.5)
	cv::Mat gauss= cv::getGaussianKernel(9,1.5,CV_32F);
		  
	// Display kernel values
	cv::Mat_<float>::const_iterator it= gauss.begin<float>();  
	cv::Mat_<float>::const_iterator itend= gauss.end<float>();  
	std::cout << "1.5 = [";
	for ( ; it!= itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the gaussian kernel (0.5)
	gauss= cv::getGaussianKernel(9,0.5,CV_32F);
		  
	// Display kernel values
	it= gauss.begin<float>();  
	itend= gauss.end<float>();  
	std::cout << "0.5 = [";
	for ( ; it!= itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the gaussian kernel (2.5)
	gauss= cv::getGaussianKernel(9,2.5,CV_32F);
		  
	// Display kernel values
	it= gauss.begin<float>();  
	itend= gauss.end<float>();  
	std::cout << "2.5 = [";
	for ( ; it!= itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the gaussian kernel (9 elements)
	gauss= cv::getGaussianKernel(9,-1,CV_32F);
		  
	// Display kernel values
	it= gauss.begin<float>();  
	itend= gauss.end<float>();  
	std::cout << "9 = [";
	for ( ; it!= itend; ++it) {
		std::cout << *it << " ";
	}
	std::cout << "]" << std::endl;

	// Get the Deriv kernel (2.5)
	cv::Mat kx, ky;
    cv::getDerivKernels(kx,ky,2,2,7,true);
		  
	// Display kernel values
	cv::Mat_<float>::const_iterator kit= kx.begin<float>();  
	cv::Mat_<float>::const_iterator kitend= kx.end<float>();  
	std::cout << "[";
	for ( ; kit!= kitend; ++kit) {
		std::cout << *kit << " ";
	}
	std::cout << "]" << std::endl;

	// Read input image with salt&pepper noise
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\salted.bmp",0);
	if (!image.data)
		return 0; 

    // Display the S&P image
	cv::namedWindow("S&P Image");
	cv::imshow("S&P Image",image);

	// Blur the image with a mean filter
	cv::blur(image,result,cv::Size(5,5));
		
    // Display the blurred image
	cv::namedWindow("Mean filtered S&P Image");
	cv::imshow("Mean filtered S&P Image",result);

	// 中值滤波
	// Applying a median filter
	cv::medianBlur(image,result,5);
		
    // Display the blurred image
	cv::namedWindow("Median filtered Image");
	cv::imshow("Median filtered Image",result);

	// 只保留四个像素中的一个
	// Reduce by 4 the size of the image (the wrong way)
	image= cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg",0);
	cv::Mat reduced(image.rows / 4, image.cols / 4, CV_8U);

	for (int i=0; i<reduced.rows; i++)
		for (int j=0; j<reduced.cols; j++)
			reduced.at<uchar>(i,j)= image.at<uchar>(i*4,j*4);

    // Display the reduced image
	cv::namedWindow("Badly reduced Image");
	cv::imshow("Badly reduced Image",reduced);

	cv::resize(reduced,reduced, cv::Size(), 4, 4, cv::INTER_NEAREST);

    // Display the (resized) reduced image
	cv::namedWindow("Badly reduced");
	cv::imshow("Badly reduced",reduced);

	cv::imwrite("E:\\Projects\\Opencv3_Demo\\6_filters_laplacianZC_derivatives\\badlyreducedimage.bmp",reduced);

	// 首先去除高频成分
	// first remove high frequency component
	cv::GaussianBlur(image, image, cv::Size(11,11), 1.75);
	// keep only 1 of every 4 pixels
	cv::Mat reduced2(image.rows/4, image.cols/4, CV_8U);
	for (int i=0; i<reduced2.rows; i++)
		for (int j=0; j<reduced2.cols; j++)
			reduced2.at<uchar>(i,j)= image.at<uchar>(i*4,j*4);

    // Display the reduced image
	cv::namedWindow("Reduced Image, original size");
	cv::imshow("Reduced Image, original size",reduced2);

	cv::imwrite("E:\\Projects\\Opencv3_Demo\\6_filters_laplacianZC_derivatives\\reducedimage.bmp",reduced2);

	// 最近邻域插值法
	// resizing with NN
	cv::Mat newImage;
	cv::resize(reduced2, newImage, cv::Size(), 4, 4, cv::INTER_NEAREST);

    // Display the (resized) reduced image
	cv::namedWindow("Reduced Image");
	cv::imshow("Reduced Image",newImage);

	// 双线性插值法
	// resizing with bilinear
	cv::resize(reduced2, newImage, cv::Size(), 4, 4, cv::INTER_LINEAR);

	// Display the (resized) reduced image
	cv::namedWindow("Bilinear resizing");
	cv::imshow("Bilinear resizing", newImage);

	// 图像金字塔
	// Creating an image pyramid
	cv::Mat pyramid(image.rows, image.cols + image.cols/2 + image.cols/4 + image.cols/8, CV_8U, cv::Scalar(255));
	image.copyTo(pyramid(cv::Rect(0, 0, image.cols, image.rows)));

	// cv::pyDown——图像尺寸缩小（2倍）
	// cv::pyUp——图像尺寸放大（2倍）
	// cv:: resize——通用的图像尺寸缩放（任意倍数）

	cv::pyrDown(image, reduced); // reduce image size by half
	reduced.copyTo(pyramid(cv::Rect(image.cols, image.rows/2, image.cols/2, image.rows/2)));
	cv::pyrDown(reduced, reduced2); // reduce image size by another half
	reduced2.copyTo(pyramid(cv::Rect(image.cols + image.cols/2, image.rows - image.rows/4, image.cols/4, image.rows/4)));
	cv::pyrDown(reduced2, reduced); // reduce image size by another half
	reduced.copyTo(pyramid(cv::Rect(image.cols + image.cols/2 + image.cols/4, image.rows - image.rows/8, image.cols/8, image.rows/8)));

    // Display the pyramid
	cv::namedWindow("Pyramid of images");
	cv::imshow("Pyramid of images", pyramid);

	cv::waitKey();
	return 0;
}
