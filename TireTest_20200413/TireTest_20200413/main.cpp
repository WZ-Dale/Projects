#include "ExtractTread.h"
#include "GLCM.h"
#include "Radon.h"

// 小小的测试
void Test0() {
	for (int i = 10; i < 14; ++i) {
		// 原图
		cv::String x = to_string(i);
		cv::String str = ".//1//1" + x + ".jpg";
		cv::Mat image = cv::imread(str, cv::IMREAD_GRAYSCALE);
		if (image.empty()) {
			std::cout << "Error reading image..." << std::endl;
			return;
		}
		//ShowImage("image", image);

	// 截取图片
		//int x = 0, y = image.rows / 4;
		//cv::Rect rect(x, y, image.cols, 2 * y);
		//cv::Mat image_roi = image(rect);
		//ShowImage("image_roi", image_roi);

	// 缩小图片
		//cv::Mat image_min;
		//cv::resize(image_roi, image_min, cv::Size(), 0.2, 0.2);
		//ShowImage("image_min", image_min);

		//cv::imwrite(str, image);
	}
}


int main(int argc, const char ** argv) {
	//Test0();
	//Test1();
	Test2();					// 提取胎冠图像

	//for (int i = 1; i < 14; ++i) {
	//	cv::String x = to_string(i);
	//	cv::String str;
	//	if (i < 10) {
	//		str = ".//0//30" + x + ".jpg";
	//	}
	//	else {
	//		str = ".//0//3" + x + ".jpg";
	//	}
	//	Test3(str.c_str());			// 灰度共生矩阵（磨损）
	//}

	//Test4(argc, argv);				// Radon变换



	cv::waitKey(0); // 0 to indefinitely wait for a key pressed
	cv::destroyAllWindows();
	return 0;
}