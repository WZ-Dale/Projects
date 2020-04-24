#include "ExtractTread.h"
#include "GLCM.h"
//#include "Radon.h"


void Test0() {
	for (int i = 10; i < 14; ++i) {
		// ԭͼ
		cv::String x = to_string(i);
		cv::String str = ".//1//1" + x + ".jpg";
		cv::Mat image = cv::imread(str, cv::IMREAD_GRAYSCALE);
		if (image.empty()) {
			std::cout << "Error reading image..." << std::endl;
			return;
		}
		//ShowImage("image", image);

	// ��ȡͼƬ
		//int x = 0, y = image.rows / 4;
		//cv::Rect rect(x, y, image.cols, 2 * y);
		//cv::Mat image_roi = image(rect);
		//ShowImage("image_roi", image_roi);

	// ��СͼƬ
		//cv::Mat image_min;
		//cv::resize(image_roi, image_min, cv::Size(), 0.2, 0.2);
		//ShowImage("image_min", image_min);

		cv::imwrite(str, image);
	}
}

int main(int argc, const char ** argv) {
	//Test0();
	//Test1();
	//Test2();

	for (int i = 1; i < 14; ++i) {
		cv::String x = to_string(i);
		cv::String str;
		if (i < 10) {
			str = ".//0//30" + x + ".jpg";
		}
		else {
			str = ".//0//3" + x + ".jpg";
		}
		Test3(str.c_str());
	}

	//Test4(argc, argv);



	cv::waitKey(0); // 0 to indefinitely wait for a key pressed
	cv::destroyAllWindows();
	return 0;
}