#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

int main(){
	cv::Mat image;
	image = cv::imread("11.jpg");
	cv::namedWindow("image11");
	// image11 = cv2.imread("11.jpg", cv2.IMREAD_GRAYSCALE)
	// cv2.imshow("image11", image11) 

	return  0;
}
