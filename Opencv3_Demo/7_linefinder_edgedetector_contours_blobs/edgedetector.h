// 20200108
// Ñ§Ï°²¢×¢ÊÍ by wangze

#if !defined SOBELEDGES
#define SOBELEDGES

#define PI 3.1415926

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class EdgeDetector {

  private:

	  // original image
	  cv::Mat img;

	  // 16-bit signed int image
	  cv::Mat sobel;

	  // Aperture size of the Sobel kernel
	  int aperture;

	  // Sobel magnitude
	  cv::Mat sobelMagnitude;

	  // Sobel orientation
	  cv::Mat sobelOrientation;

  public:

	  EdgeDetector() : aperture(3) {}

	  // Set the aperture size of the kernel
	  void setAperture(int a) {

		  aperture= a;
	  }

	  // Get the aperture size of the kernel
	  int getAperture() const {

		  return aperture;
	  }

	  // Compute the Sobel
	  void computeSobel(const cv::Mat& image) {

		  cv::Mat sobelX;
		  cv::Mat sobelY;

		  // Compute Sobel
		  cv::Sobel(image, sobelX, CV_32F, 1, 0, aperture);
		  cv::Sobel(image, sobelY, CV_32F, 0, 1, aperture);

		  // Compute magnitude and orientation
		  cv::cartToPolar(sobelX, sobelY, sobelMagnitude, sobelOrientation);
	  }

	  // Compute the Sobel
	  void computeSobel(const cv::Mat& image, cv::Mat &sobelX, cv::Mat &sobelY) {

		  // Compute Sobel
		  cv::Sobel(image, sobelX, CV_32F, 1, 0, aperture);
		  cv::Sobel(image, sobelY, CV_32F, 0, 1, aperture);

		  // Compute magnitude and orientation
		  cv::cartToPolar(sobelX, sobelY, sobelMagnitude, sobelOrientation);
	  }

	  // Get Sobel magnitude
	  cv::Mat getMagnitude() {

		  return sobelMagnitude;
	  }

	  // Get Sobel orientation
	  cv::Mat getOrientation() {

		  return sobelOrientation;
	  }

	  // Get a thresholded binary map
	  cv::Mat getBinaryMap(double threshold) {

		  cv::Mat bin;		  
		  cv::threshold(sobelMagnitude,bin,threshold,255,cv::THRESH_BINARY_INV);

		  return bin;
	  }

	  // Get a CV_8U image of the Sobel
	  cv::Mat getSobelImage() {

		  cv::Mat bin;

		  double minval, maxval;
		  cv::minMaxLoc(sobelMagnitude,&minval,&maxval);
		  sobelMagnitude.convertTo(bin,CV_8U,255/maxval);

		  return bin;
	  }

	  // Get a CV_8U image of the Sobel orientation
	  // 1 gray-level = 2 degrees
	  cv::Mat getSobelOrientationImage() {

		  cv::Mat bin;

		  sobelOrientation.convertTo(bin,CV_8U,90/PI);

		  return bin;
	  }

};


#endif
