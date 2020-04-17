#include "TireTest.h"


void Test0() {
	cv::Mat image = cv::imread("11.jpg", cv::IMREAD_GRAYSCALE);
	if (image.empty()) {
		std::cout << "Error reading image..." << std::endl;
		return;
	}
	std::cout << "This image is " << image.rows << " x " << image.cols << std::endl;
	cv::namedWindow("Image");
	cv::imshow("Image", image);
	// 注册回调函数，鼠标事件
	cv::setMouseCallback("Image", onMouse, reinterpret_cast<void*>(&image));

	cv::waitKey(0);
}

int main() {
	//Test0();
	Test1();



	cv::waitKey(0); // 0 to indefinitely wait for a key pressed
	cv::destroyAllWindows();
	return 0;
}