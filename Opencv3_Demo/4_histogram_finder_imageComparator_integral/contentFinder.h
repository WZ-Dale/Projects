// 20191219
// 学习并注释 by wangze

#pragma once

#if !defined OFINDER
#define OFINDER

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class ContentFinder {

  private:

	// histogram parameters
	float hranges[2];
    const float* ranges[3];
    int channels[3];

	float threshold;           // decision threshold
	cv::Mat histogram;         // histogram can be sparse 
	cv::SparseMat shistogram;  // or not
	bool isSparse;

  public:

	ContentFinder() : threshold(0.1f), isSparse(false) {

		// in this class,
		// all channels have the same range
		ranges[0]= hranges;  
		ranges[1]= hranges; 
		ranges[2]= hranges; 
	}
   
	// Sets the threshold on histogram values [0,1]
	void setThreshold(float t) {

		threshold= t;
	}

	// Gets the threshold
	float getThreshold() {

		return threshold;
	}

	// 归一化
	// Sets the reference histogram
	void setHistogram(const cv::Mat& h) {

		isSparse= false;
		cv::normalize(h,histogram,1.0);
	}

	// Sets the reference histogram
	void setHistogram(const cv::SparseMat& h) {

		isSparse= true;
		cv::normalize(h,shistogram,1.0,cv::NORM_L2);
	}

	// Simplified version in which
	// all channels used, with range [0,256[
	cv::Mat find(const cv::Mat& image) {

		cv::Mat result;

		hranges[0]= 0.0;	// default range [0,256[
		hranges[1]= 256.0;
		channels[0]= 0;		// the three channels 
		channels[1]= 1; 
		channels[2]= 2; 

		return find(image, hranges[0], hranges[1], channels);
	}

	// Finds the pixels belonging to the histogram
	cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int *channels) {

		cv::Mat result;

		hranges[0]= minValue;
		hranges[1]= maxValue;

		if (isSparse) { // call the right function based on histogram type

		   for (int i=0; i<shistogram.dims(); i++)
			  this->channels[i]= channels[i];

		   cv::calcBackProject(&image,
                      1,            // we only use one image at a time
                      channels,     // vector specifying what histogram dimensions belong to what image channels
                      shistogram,   // the histogram we are using
                      result,       // the resulting back projection image
                      ranges,       // the range of values, for each dimension
                      255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255
		   );

		} else {

		   for (int i=0; i<histogram.dims; i++)
			  this->channels[i]= channels[i];

		   cv::calcBackProject(&image,
                      1,            // we only use one image at a time/*一幅图像*/
                      channels,     // vector specifying what histogram dimensions belong to what image channels/*用到的通道，取决于直方图的维度*/
                      histogram,    // the histogram we are using/*需要反向投影的直方图*/
                      result,       // the resulting back projection image/*反向投影得到的结果*/
                      ranges,       // the range of values, for each dimension/*值的范围*/
                      255.0         // the scaling factor is chosen such that a histogram value of 1 maps to 255/*选用的换算系数，把概率值从1映射到255*/
		   );
		}

        // Threshold back projection to obtain a binary image
		if (threshold>0.0)
			cv::threshold(result, result, 255.0*threshold, 255.0, cv::THRESH_BINARY);

		return result;
	}

};


#endif
