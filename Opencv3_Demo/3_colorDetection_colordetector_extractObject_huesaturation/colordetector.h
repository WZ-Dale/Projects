// 20191210
// 学习并注释 by wangze

#pragma once

#if !defined COLORDETECT
#define COLORDETECT

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/imgproc/types_c.h>

class ColorDetector {

  private:
	  // 允许的最小距离
	  // minimum acceptable distance
	  int maxDist; 
	  // 目标颜色
	  // target color
	  cv::Vec3b target; 
	  // 包含颜色转换图像的图像
	  // image containing color converted image
	  cv::Mat converted;
	  bool useLab;
	  // 包含生成的二进制映射的图像
	  // image containing resulting binary map
	  cv::Mat result;

  public:
	  // 空构造函数
	  // empty constructor
	  // 在此初始化默认参数
	  // default parameter initialization here
	  ColorDetector() : maxDist(100), target(0,0,0), useLab(false) {}

	  // extra constructor for Lab color space example
	  ColorDetector(bool useLab) : maxDist(100), target(0,0,0), useLab(useLab) {}
	  
	  // 另一种构造函数，使用用户输入的目标颜色和颜色距离作为参数
	  // full constructor
	  ColorDetector(uchar blue, uchar green, uchar red, int mxDist=100, bool useLab=false): maxDist(mxDist), useLab(useLab) { 

		  // target color
		  setTargetColor(blue, green, red);
	  }

	  // 计算与目标颜色的差距
	  // Computes the distance from target color.
	  int getDistanceToTargetColor(const cv::Vec3b& color) const {
		  return getColorDistance(color, target);
	  }
	  // 计算两个颜色之间的城区距离
	  // Computes the city-block distance between two colors.
	  int getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const {

		  return abs(color1[0]-color2[0])+
					abs(color1[1]-color2[1])+
					abs(color1[2]-color2[2]);

	 	  // Or:
		  // return static_cast<int>(cv::norm<int,3>(cv::Vec3i(color[0]-color2[0],color[1]-color2[1],color[2]-color2[2])));
		  
		  // Or:
		  // cv::Vec3b dist;
		  // cv::absdiff(color,color2,dist);
		  // return cv::sum(dist)[0];
	  }

	  // Processes the image. Returns a 1-channel binary image.
	  cv::Mat process(const cv::Mat &image) {
			  // 必要时重新分配二值映像
			  // 与输入图像的尺寸相同，不过是单通道
			  // re-allocate binary map if necessary
			  // same size as input image, but 1-channel
			  result.create(image.size(), CV_8U);

			  // 转换到Lab颜色空间
			  // Converting to Lab color space 
			  if (useLab)
				  cv::cvtColor(image, converted, CV_BGR2Lab);

			  // 取得迭代器
			  // get the iterators
			  cv::Mat_<cv::Vec3b>::const_iterator it = image.begin<cv::Vec3b>();
			  cv::Mat_<cv::Vec3b>::const_iterator itend = image.end<cv::Vec3b>();
			  cv::Mat_<uchar>::iterator itout = result.begin<uchar>();

			  // 获取转换后图像的迭代器
			  // get the iterators of the converted image 
			  if (useLab) {
				  it = converted.begin<cv::Vec3b>();
				  itend = converted.end<cv::Vec3b>();
			  }

			  // 对比每个像素
			  // for each pixel
			  for (; it != itend; ++it, ++itout) {
				  // process each pixel --------------------- 处理每一个像素
					  // 比较与目标颜色的差距
					  // compute distance from target color
				  if (getDistanceToTargetColor(*it) < maxDist) {
					  *itout = 255;
				  }
				  else {
					  *itout = 0;
				  }
				  // end of pixel processing ---------------- 像素处理结束
			  }

			  return result;
	  }

	  // 重载operator()目的->仿函数
	  cv::Mat operator()(const cv::Mat &image) {
	  
		  cv::Mat input;
		  
		  // 转换到Lab颜色空间
		  if (useLab) { // Lab conversion
			  cv::cvtColor(image, input, CV_BGR2Lab);
		  }
		  else {
			  input = image;
		  }

		  cv::Mat output;
		  // compute absolute difference with target color
		  cv::absdiff(input,cv::Scalar(target),output);
	      // split the channels into 3 images
	      std::vector<cv::Mat> images;
	      cv::split(output,images);
		  // add the 3 channels (saturation might occurs here)
	      output= images[0]+images[1]+images[2];
		  // apply threshold
          cv::threshold(output,  // input image
                      output,  // output image
                      maxDist, // threshold (must be < 256)
                      255,     // max value
                 cv::THRESH_BINARY_INV); // thresholding type
	
	      return output;
	  }


	  // 设置颜色差距的阈值
	  // 阈值必须是正数，否则就设置为0
	  // Getters and setters
	  // Sets the color distance threshold.
	  // Threshold must be positive, otherwise distance threshold
	  // is set to 0.
	  void setColorDistanceThreshold(int distance) {

		  if (distance<0)
			  distance=0;
		  maxDist= distance;
	  }
	  // 取得颜色差距的阈值
	  // Gets the color distance threshold
	  int getColorDistanceThreshold() const {

		  return maxDist;
	  }

	  // 设置需要检测的颜色
	  // Sets the color to be detected
	  // given in BGR color space
	  void setTargetColor(uchar blue, uchar green, uchar red) {

		  // 次序为BGR
		  // BGR order
		  target = cv::Vec3b(blue, green, red);

		  if (useLab) {
			  // 临时的单像素图像
			  // Temporary 1-pixel image
			  cv::Mat tmp(1, 1, CV_8UC3);
			  tmp.at<cv::Vec3b>(0, 0) = cv::Vec3b(blue, green, red);

			  // 将目标颜色转换到Lab颜色空间
			  // Converting the target to Lab color space 
			  cv::cvtColor(tmp, tmp, CV_BGR2Lab);

			  target = tmp.at<cv::Vec3b>(0, 0);
		  }
	  }
	  // 设置需要检测的颜色
	  // Sets the color to be detected
	  void setTargetColor(cv::Vec3b color) {

		  target= color;
	  }
	  // 取得需要检测的颜色
	  // Gets the color to be detected
	  cv::Vec3b getTargetColor() const {

		  return target;
	  }
};

#endif
