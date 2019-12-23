// 20191221
// 学习并注释 by wangze

#pragma once

#if !defined ICOMPARATOR
#define ICOMPARATOR

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "E:\Projects\Opencv3_Demo\4_histogram_finder__imageComparator_integral\colorhistogram.h"

// 比较直方图搜索相似图像
class ImageComparator {

  private:

	cv::Mat refH;       // reference histogram// 基准图像
	cv::Mat inputH;     // histogram of input image// 输入图像的直方图

	ColorHistogram hist; // 生成直方图
	int nBins; // number of bins used in each color channel// 每个颜色通道使用的箱子数量

  public:

	ImageComparator() :nBins(8) {

	}

	// Set number of bins used when comparing the histograms
	void setNumberOfBins( int bins) {

		nBins= bins;
	}

	int getNumberOfBins() {

		return nBins;
	}
	// 设置并计算基准图像的直方图
	// set and compute histogram of reference image
	void setReferenceImage(const cv::Mat& image) {

		hist.setSize(nBins);
		refH= hist.getHistogram(image);
	}
	// 用BGR直方图比较图像
	// compare the image using their BGR histograms
	double compare(const cv::Mat& image) {

		inputH= hist.getHistogram(image);
		// 用交叉法比较直方图
		// histogram comparison using intersection
		return cv::compareHist(refH,inputH, cv::HISTCMP_INTERSECT);
	}
};


#endif
