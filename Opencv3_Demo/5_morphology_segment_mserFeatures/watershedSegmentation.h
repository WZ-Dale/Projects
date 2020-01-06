// 20200104
// 学习并注释 by wangze

#if !defined WATERSHS
#define WATERSHS

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

class WatershedSegmenter {

  private:

	  cv::Mat markers;

  public:

	  void setMarkers(const cv::Mat& markerImage) {
		  // 转换成整数型图像
		// Convert to image of ints
		markerImage.convertTo(markers,CV_32S);
	  }

	  cv::Mat process(const cv::Mat &image) {
		  // 应用分水岭
		// Apply watershed
		cv::watershed(image,markers);

		return markers;
	  }

	  // 以图像的形式返回结果
	  // Return result in the form of an image
	  cv::Mat getSegmentation() {
		  
		cv::Mat tmp;
		// 所有标签大于255的区段都赋值255
		// all segment with label higher than 255
		// will be assigned value 255
		markers.convertTo(tmp,CV_8U);

		return tmp;
	  }

	  // 以图像的形式返回分水岭
	  // Return watershed in the form of an image
	  cv::Mat getWatersheds() {
	
		cv::Mat tmp;
		// 在变换前把每个像素p转换为255p+255
		markers.convertTo(tmp,CV_8U,255,255);

		return tmp;
	  }
};


#endif
