#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\loadDisplaySave.h"
#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\mat.h"
#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\logo.h"


using namespace std;
using namespace cv;


void Test1(){
	Mat image11;
	image11 = imread("image//11.jpg", IMREAD_GRAYSCALE);
	//namedWindow("image11");
	//imshow("image11", image11);

	// Í¼ÏñËõ·Å
	Mat image11_1;
	resize(image11, image11_1, Size(image11.cols / 5, image11.rows / 5), 0, 0, INTER_LINEAR);
	imshow("image11_1", image11_1);

	Rect rect(0, 250, 604, 300);
	Mat ROI = image11_1(rect);
	imshow("ROI", ROI);

	// Ö±·½Í¼¾ùºâ»¯
	Mat image11_2;
	equalizeHist(ROI, image11_2);
	imshow("image11_2", image11_2);

	// ±ßÔµ¼ì²â
	Mat image11_3;
	// RobertsËã×Ó
	//Mat kern = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	//filter2D(image11_2, image11_3, image11_2.depth(), kern);
	// SobelËã×Ó
	//Sobel(image11_2, image11_3, );
	// PrewittËã×Ó

	// CannyËã×Ó
	Canny(image11_2, image11_3, 100, 200);
	imshow("image11_3", image11_3);

	// ±ÕÔËËã
	Mat element5(5, 5, CV_8U, Scalar(1));
	Mat image11_4;
	morphologyEx(image11_3, image11_4, MORPH_CLOSE, element5);
	imshow("image11_4", image11_4);

	// ÇøÓòÌî³ä

}

void Test2() {
	Mat image11;
	image11 = imread("image//11.jpg", IMREAD_GRAYSCALE);

	// Í¼ÏñËõ·Å
	Mat image11_1;
	resize(image11, image11_1, Size(image11.cols / 5, image11.rows / 5), 0, 0, INTER_LINEAR);

	Rect rect(0, 250, 604, 300);
	Mat ROI = image11_1(rect);
	imshow("ROI", ROI);
}

int main() {
	//loadDisplaySave_Test();
	//mat_Test();
	logo_Test();
	//Test1();
	//Test2();



	//destroyAllWindows();
	waitKey(0);
	system("pause");
	return  0;
}
