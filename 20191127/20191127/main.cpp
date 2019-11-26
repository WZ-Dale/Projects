#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat image11;
	image11 = imread("image//11.jpg", IMREAD_GRAYSCALE);
	//namedWindow("image11");
	//imshow("image11", image11);
	
	// 图像缩放
	Mat image11_1;
	resize(image11, image11_1, Size(image11.cols / 5, image11.rows / 5), 0, 0, INTER_LINEAR);
	imshow("image11_1", image11_1);

	// 直方图均衡化
	Mat image11_2;
	equalizeHist(image11_1, image11_2);
	imshow("image11_2", image11_2);

	// 边缘检测
	Mat image11_3;
	// Roberts算子
	//Mat kern = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	//filter2D(image11_2, image11_3, image11_2.depth(), kern);
	// Sobel算子
	//Sobel(image11_2, image11_3, );
	// Prewitt算子

	// Canny算子
	Canny(image11_2, image11_3, 100, 200);
	imshow("image11_3", image11_3);

	// 闭运算
	Mat element5(5, 5, CV_8U, Scalar(1));
	Mat image11_4;
	morphologyEx(image11_3, image11_4, MORPH_CLOSE, element5);
	imshow("image11_4", image11_4);

	// 区域填充



	//destroyAllWindows();
	waitKey(0);
	return  0;
}
