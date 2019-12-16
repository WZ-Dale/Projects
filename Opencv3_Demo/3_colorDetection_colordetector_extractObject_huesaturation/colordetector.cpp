// 20191210
// 学习并注释 by wangze

#pragma once

#include "E:\Projects\Opencv3_Demo\3_colorDetection_colordetector_extractObject_huesaturation\colordetector.h"
#include <vector>
	
cv::Mat ColorDetector::process(const cv::Mat &image) {
	  // 必要时重新分配二值映像
	  // 与输入图像的尺寸相同，不过是单通道
	  // re-allocate binary map if necessary
	  // same size as input image, but 1-channel
	  result.create(image.size(),CV_8U);

	  // 转换到Lab颜色空间
	  // Converting to Lab color space 
	  if (useLab)
		  cv::cvtColor(image, converted, CV_BGR2Lab);

	  // 取得迭代器
	  // get the iterators
	  cv::Mat_<cv::Vec3b>::const_iterator it= image.begin<cv::Vec3b>();
	  cv::Mat_<cv::Vec3b>::const_iterator itend= image.end<cv::Vec3b>();
	  cv::Mat_<uchar>::iterator itout= result.begin<uchar>();

	  // 获取转换后图像的迭代器
	  // get the iterators of the converted image 
	  if (useLab) {
		  it = converted.begin<cv::Vec3b>();
		  itend = converted.end<cv::Vec3b>();
	  }

	  // 对比每个像素
	  // for each pixel
	  for ( ; it!= itend; ++it, ++itout) {
	  // process each pixel --------------------- 处理每一个像素
		  // 比较与目标颜色的差距
		  // compute distance from target color
		  if (getDistanceToTargetColor(*it)<maxDist) {
			  *itout= 255;
		  } else {
			  *itout= 0;
		  }
      // end of pixel processing ---------------- 像素处理结束
	  }

	  return result;
}

