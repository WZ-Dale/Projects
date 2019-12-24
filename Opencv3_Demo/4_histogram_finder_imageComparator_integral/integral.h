// 20191223
// Ñ§Ï°²¢×¢ÊÍ by wangze

#pragma once

#if !defined IINTEGRAL
#define IINTEGRAL

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <vector>

template <typename T, int N>
class IntegralImage {

	  cv::Mat integralImage;

  public:

	  IntegralImage(cv::Mat image) {

		// (costly) computation of the integral image
		cv::integral(image,integralImage,cv::DataType<T>::type);
	  }

	  // compute sum over sub-regions of any size from 4 pixel access
	  cv::Vec<T,N> operator()(int xo, int yo, int width, int height) {

		  // window at (xo,yo) of size width by height
          return (integralImage.at<cv::Vec<T,N> >(yo+height,xo+width)
                  -integralImage.at<cv::Vec<T,N> >(yo+height,xo)
                  -integralImage.at<cv::Vec<T,N> >(yo,xo+width)
                  +integralImage.at<cv::Vec<T,N> >(yo,xo));
	  }

	  // compute sum over sub-regions of any size from 4 pixel access
	  cv::Vec<T,N> operator()(int x, int y, int radius) {

		  // square window centered at (x,y) of size 2*radius+1
          return (integralImage.at<cv::Vec<T,N> >(y+radius+1,x+radius+1)
                  -integralImage.at<cv::Vec<T,N> >(y+radius+1,x-radius)
                  -integralImage.at<cv::Vec<T,N> >(y-radius,x+radius+1)
                  +integralImage.at<cv::Vec<T,N> >(y-radius,x-radius));
	  }
};

// convert to a multi-channel image made of binary planes
// nPlanes must be a power of 2
void convertToBinaryPlanes(const cv::Mat& input, cv::Mat& output, int nPlanes) {

	    // number of bits to mask out
	    int n= 8-static_cast<int>(log(static_cast<double>(nPlanes))/log(2.0));
	    // mask used to eliminate least significant bits
	    uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0

		// create a vector of 16 binary images
		std::vector<cv::Mat> planes;
		// reduce to nBins bins by eliminating least significant bits
		cv::Mat reduced= input&mask;

		// compute each binary image plane
		for (int i=0; i<nPlanes; i++) {

			// 1 for each pixel equals to i<<shift
			planes.push_back((reduced==(i<<n))&0x1);
		}

	    // create multi-channel image
		cv::merge(planes,output);
}

#endif
