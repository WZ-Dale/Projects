// 20200107
// 学习并注释 by wangze

#if !defined LAPLACEZC
#define LAPLACEZC

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class LaplacianZC {

  private:
	  // laplacian（拉普拉斯算子）
	  cv::Mat laplace;

	  // Aperture size of the laplacian kernel（拉普拉斯内核的孔径大小）
	  int aperture;

  public:

	  LaplacianZC() : aperture(3) {}

	  // Set the aperture size of the kernel（设置内核的孔径大小）
	  void setAperture(int a) {

		  aperture= a;
	  }

	  // Get the aperture size of the kernel
	  int getAperture() const {

		  return aperture;
	  }

	  // Compute the floating point Laplacian（计算浮点数类型的拉普拉斯算子）
	  cv::Mat computeLaplacian(const cv::Mat& image) {

		  // Compute Laplacian（计算拉普拉斯算子）
		  cv::Laplacian(image,laplace,CV_32F,aperture);

		  return laplace;
	  }

	  // Get the Laplacian result in 8-bit image （获得拉普拉斯结果，存在8位图像中）
	  // zero corresponds to gray level 128（0表示灰度级128）
	  // if no scale is provided, then the max value will be（如果不指定缩放比例，那么最大值会放大到255）
	  // scaled to intensity 255
	  // You must call computeLaplacian before calling this method（在调用这个函数之前，必须先调用computeLaplacian）
	  cv::Mat getLaplacianImage(double scale=-1.0) {

		  if (scale<0) {
	
			  double lapmin, lapmax;
			  // 取得最小和最大拉普拉斯值
			  cv::minMaxLoc(laplace,&lapmin,&lapmax);
			  // 缩放拉普拉斯算子到127
			  scale= 127/ std::max(-lapmin,lapmax);
		  }
		  // 生成灰度图像
		  cv::Mat laplaceImage;
		  laplace.convertTo(laplaceImage,CV_8U,scale,128);

		  return laplaceImage;
	  }

	  // Get a binary image of the zero-crossings（获得过零点的二值图像）
	  // laplacian image should be CV_32F（拉普拉斯图像的类型必须是CV_32F）
	  cv::Mat getZeroCrossings(cv::Mat laplace) {

		  // threshold at 0（阈值为0）
		  // negative values in black（负数用黑色）
		  // positive values in white（正数用白色）
		  cv::Mat signImage;
		  cv::threshold(laplace,signImage,0,255,cv::THRESH_BINARY);

		  // convert the +/- image into CV_8U（把+/-图像转换成CV_8U）
		  cv::Mat binary;
		  signImage.convertTo(binary,CV_8U);

		  // dilate the binary image of +/- regions（膨胀+/-区域的二值图像）
		  cv::Mat dilated;
		  cv::dilate(binary,dilated,cv::Mat());
	
		  // return the zero-crossing contours（返回过零点的轮廓）
		  return dilated-binary;
	  }
};


#endif
