// 20191226
// 学习并注释 by wangze

#pragma once

#if !defined COLHISTOGRAM
#define COLHISTOGRAM

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <opencv2/imgproc/types_c.h>

class ColorHistogram {

  private:

    int histSize[3];        // size of each dimension（每个纬度的大小）
	float hranges[2];       // range of values (same for the 3 dimensions)（值的范围（三个维度用同一个值））
    const float* ranges[3]; // array of ranges for each dimension（每个维度的范围）
    int channels[3];        // channel to be considered（需要处理的通道）

  public:

	ColorHistogram() {

		// Prepare default arguments for a color histogram（准备用于彩色图像的默认参数）
		// each dimension has equal size and range（每个维度的大小和范围是相等的）
		histSize[0]= histSize[1]= histSize[2]= 256;
		hranges[0]= 0.0;    // BRG range from 0 to 256
		hranges[1]= 256.0;
		ranges[0]= hranges; // in this class,  （这个类中，所有通道的范围都相等）
		ranges[1]= hranges; // all channels have the same range
		ranges[2]= hranges; 
		channels[0]= 0;		// the three channels: B（三个通道BGR）
		channels[1]= 1;     // G
		channels[2]= 2;     // R
	}

	// set histogram size for each dimension
	void setSize(int size) {

		// each dimension has equal size 
		histSize[0]= histSize[1]= histSize[2]= size;
	}

	// Computes the histogram.（计算直方图）
	cv::Mat getHistogram(const cv::Mat &image) {

		cv::Mat hist;

		// BGR color histogram
		hranges[0]= 0.0;    // BRG range
		hranges[1]= 256.0;
		channels[0]= 0;		// the three channels 
		channels[1]= 1; 
		channels[2]= 2; 

		// Compute histogram（计算直方图）
		cv::calcHist(&image, 
			1,			// histogram of 1 image only（单幅图像的直方图）
			channels,	// the channel used（用到的通道）
			cv::Mat(),	// no mask is used（不使用掩码）
			hist,		// the resulting histogram（得到直方图）
			3,			// it is a 3D histogram（这是一个三维直方图）
			histSize,	// number of bins（箱子数量）
			ranges		// pixel value range（像素值的范围）
		);

		return hist;
	}

	// Computes the histogram.（计算直方图）
	cv::SparseMat getSparseHistogram(const cv::Mat &image) {

		cv::SparseMat hist(3,        // number of dimensions（维数）
			               histSize, // size of each dimension（每个纬度的大小）
						   CV_32F);

		// BGR color histogram
		hranges[0]= 0.0;    // BRG range
		hranges[1]= 256.0;
		channels[0]= 0;		// the three channels 
		channels[1]= 1; 
		channels[2]= 2; 

		// Compute histogram（计算直方图）
		cv::calcHist(&image, 
			1,			// histogram of 1 image only（单幅图像的直方图）
			channels,	// the channel used（用到的通道）
			cv::Mat(),	// no mask is used（不使用掩码）
			hist,		// the resulting histogram（得到直方图）
			3,			// it is a 3D histogram（这是一个三维直方图）
			histSize,	// number of bins（箱子数量）
			ranges		// pixel value range（像素值的范围）
		);

		return hist;
	}

	// 获取色调直方图
	// 计算一维色调直方图
	// BGR的原图转换成HSV
	// 忽略低饱和度的像素
	// Computes the 1D Hue histogram.
	// BGR source image is converted to HSV
	// Pixels with low saturation are ignored
	cv::Mat getHueHistogram(const cv::Mat &image, 
                             int minSaturation=0) {

		cv::Mat hist;
		// 转换成HSV
		// Convert to HSV colour space
		cv::Mat hsv;
		cv::cvtColor(image, hsv, CV_BGR2HSV);
		// 掩码
		// Mask to be used (or not)
		cv::Mat mask;
		// 根据需要创建掩码
		// creating the mask if required
		if (minSaturation>0) {
			// 将3个通道分割进3幅图像
			// Spliting the 3 channels into 3 images
			std::vector<cv::Mat> v;
			cv::split(hsv,v);
			// 屏蔽低饱和度的像素
			// Mask out the low saturated pixels
			cv::threshold(v[1],mask,minSaturation,255,
                                 cv::THRESH_BINARY);
		}
		// 准备一维色调直方图的参数
		// Prepare arguments for a 1D hue histogram
		hranges[0]= 0.0;    // range is from 0 to 180
		hranges[1]= 180.0;
		channels[0]= 0;    // the hue channel 
		// 计算直方图
		// Compute histogram
		cv::calcHist(&hsv, 
			1,			// histogram of 1 image only
			channels,	// the channel used
			mask,		// binary mask
			hist,		// the resulting histogram
			1,			// it is a 1D histogram
			histSize,	// number of bins
			ranges		// pixel value range
		);

		return hist;
	}

	// Computes the 2D ab histogram.
	// BGR source image is converted to Lab
	cv::Mat getabHistogram(const cv::Mat &image) {

		cv::Mat hist;

		// Convert to Lab color space
		cv::Mat lab;
		cv::cvtColor(image, lab, CV_BGR2Lab);

		// Prepare arguments for a 2D color histogram
		hranges[0]= 0;
		hranges[1]= 256.0;
		channels[0]= 1; // the two channels used are ab 
		channels[1]= 2; 

		// Compute histogram
		cv::calcHist(&lab, 
			1,			// histogram of 1 image only
			channels,	// the channel used
			cv::Mat(),	// no mask is used
			hist,		// the resulting histogram
			2,			// it is a 2D histogram
			histSize,	// number of bins
			ranges		// pixel value range
		);

		return hist;
	}
};


#endif
