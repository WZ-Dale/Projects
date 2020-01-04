// 20191226
// ѧϰ��ע�� by wangze

#pragma once

#if !defined COLHISTOGRAM
#define COLHISTOGRAM

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <opencv2/imgproc/types_c.h>

class ColorHistogram {

  private:

    int histSize[3];        // size of each dimension��ÿ��γ�ȵĴ�С��
	float hranges[2];       // range of values (same for the 3 dimensions)��ֵ�ķ�Χ������ά����ͬһ��ֵ����
    const float* ranges[3]; // array of ranges for each dimension��ÿ��ά�ȵķ�Χ��
    int channels[3];        // channel to be considered����Ҫ������ͨ����

  public:

	ColorHistogram() {

		// Prepare default arguments for a color histogram��׼�����ڲ�ɫͼ���Ĭ�ϲ�����
		// each dimension has equal size and range��ÿ��ά�ȵĴ�С�ͷ�Χ����ȵģ�
		histSize[0]= histSize[1]= histSize[2]= 256;
		hranges[0]= 0.0;    // BRG range from 0 to 256
		hranges[1]= 256.0;
		ranges[0]= hranges; // in this class,  ��������У�����ͨ���ķ�Χ����ȣ�
		ranges[1]= hranges; // all channels have the same range
		ranges[2]= hranges; 
		channels[0]= 0;		// the three channels: B������ͨ��BGR��
		channels[1]= 1;     // G
		channels[2]= 2;     // R
	}

	// set histogram size for each dimension
	void setSize(int size) {

		// each dimension has equal size 
		histSize[0]= histSize[1]= histSize[2]= size;
	}

	// Computes the histogram.������ֱ��ͼ��
	cv::Mat getHistogram(const cv::Mat &image) {

		cv::Mat hist;

		// BGR color histogram
		hranges[0]= 0.0;    // BRG range
		hranges[1]= 256.0;
		channels[0]= 0;		// the three channels 
		channels[1]= 1; 
		channels[2]= 2; 

		// Compute histogram������ֱ��ͼ��
		cv::calcHist(&image, 
			1,			// histogram of 1 image only������ͼ���ֱ��ͼ��
			channels,	// the channel used���õ���ͨ����
			cv::Mat(),	// no mask is used����ʹ�����룩
			hist,		// the resulting histogram���õ�ֱ��ͼ��
			3,			// it is a 3D histogram������һ����άֱ��ͼ��
			histSize,	// number of bins������������
			ranges		// pixel value range������ֵ�ķ�Χ��
		);

		return hist;
	}

	// Computes the histogram.������ֱ��ͼ��
	cv::SparseMat getSparseHistogram(const cv::Mat &image) {

		cv::SparseMat hist(3,        // number of dimensions��ά����
			               histSize, // size of each dimension��ÿ��γ�ȵĴ�С��
						   CV_32F);

		// BGR color histogram
		hranges[0]= 0.0;    // BRG range
		hranges[1]= 256.0;
		channels[0]= 0;		// the three channels 
		channels[1]= 1; 
		channels[2]= 2; 

		// Compute histogram������ֱ��ͼ��
		cv::calcHist(&image, 
			1,			// histogram of 1 image only������ͼ���ֱ��ͼ��
			channels,	// the channel used���õ���ͨ����
			cv::Mat(),	// no mask is used����ʹ�����룩
			hist,		// the resulting histogram���õ�ֱ��ͼ��
			3,			// it is a 3D histogram������һ����άֱ��ͼ��
			histSize,	// number of bins������������
			ranges		// pixel value range������ֵ�ķ�Χ��
		);

		return hist;
	}

	// ��ȡɫ��ֱ��ͼ
	// ����һάɫ��ֱ��ͼ
	// BGR��ԭͼת����HSV
	// ���Եͱ��Ͷȵ�����
	// Computes the 1D Hue histogram.
	// BGR source image is converted to HSV
	// Pixels with low saturation are ignored
	cv::Mat getHueHistogram(const cv::Mat &image, 
                             int minSaturation=0) {

		cv::Mat hist;
		// ת����HSV
		// Convert to HSV colour space
		cv::Mat hsv;
		cv::cvtColor(image, hsv, CV_BGR2HSV);
		// ����
		// Mask to be used (or not)
		cv::Mat mask;
		// ������Ҫ��������
		// creating the mask if required
		if (minSaturation>0) {
			// ��3��ͨ���ָ��3��ͼ��
			// Spliting the 3 channels into 3 images
			std::vector<cv::Mat> v;
			cv::split(hsv,v);
			// ���εͱ��Ͷȵ�����
			// Mask out the low saturated pixels
			cv::threshold(v[1],mask,minSaturation,255,
                                 cv::THRESH_BINARY);
		}
		// ׼��һάɫ��ֱ��ͼ�Ĳ���
		// Prepare arguments for a 1D hue histogram
		hranges[0]= 0.0;    // range is from 0 to 180
		hranges[1]= 180.0;
		channels[0]= 0;    // the hue channel 
		// ����ֱ��ͼ
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