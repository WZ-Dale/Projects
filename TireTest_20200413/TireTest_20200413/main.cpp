#include "ExtractTread.h"
#include "GLCM.h"
#include "Radon.h"


void Test0() {
// ԭͼ
	cv::Mat image = cv::imread("T05.jpg", cv::IMREAD_GRAYSCALE);
	if (image.empty()) {
		std::cout << "Error reading image..." << std::endl;
		return;
	}
	//ShowImage("image", image);

// ��ȡͼƬ
	int x = 0, y = image.rows / 4;
	cv::Rect rect(x, y, image.cols, 2 * y);
	cv::Mat image_roi = image(rect);
	//ShowImage("image_roi", image_roi);

// ��СͼƬ
	cv::Mat image_min;
	cv::resize(image_roi, image_min, cv::Size(), 0.2, 0.2);
	ShowImage("image_min", image_min);

	cv::imwrite("T05.jpg", image_min);
}

int main(int argc, const char ** argv) {
	//Test0();
	//Test1();
	//Test2();
	//Test3();
	Test4(argc, argv);



	cv::waitKey(0); // 0 to indefinitely wait for a key pressed
	cv::destroyAllWindows();
	return 0;
}