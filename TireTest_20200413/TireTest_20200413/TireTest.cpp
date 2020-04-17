#include "TireTest.h"

void onMouse(int event, int x, int y, int flags, void* param) {
	cv::Mat *im = reinterpret_cast<cv::Mat*>(param);
	// 调度事件
	switch (event) {	// dispatch the event
		// 鼠标左键按下事件
	case cv::EVENT_LBUTTONDOWN: // mouse button down event
		// 显示像素值
		// display pixel value at (x,y)
		std::cout << "at (" << x << "," << y << ") value is: " << static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
		break;
	}
}
void ShowImage(const cv::String &str, cv::Mat &image) {
	// 若读到图，并输出显示图片分辨率和通道数
	std::cout << "This image is " << image.rows << " x " << image.cols << std::endl;
	std::cout << "This image has " << image.channels() << " channel(s)" << std::endl;
	// 定义窗口（choice），显示图像
	cv::namedWindow(str);
	cv::imshow(str, image);
	// 注册回调函数，鼠标事件
	cv::setMouseCallback(str, onMouse, reinterpret_cast<void*>(&image));
	cv::waitKey(0);
}
void LargestConnectedComponent(cv::Mat srcImage, cv::Mat &dstImage)
{
	cv::Mat temp;
	cv::Mat labels;
	srcImage.copyTo(temp);

	//1. 标记连通域
	int n_comps = connectedComponents(temp, labels, 4, CV_16U);
	std::vector<int> histogram_of_labels;
	for (int i = 0; i < n_comps; i++)//初始化labels的个数为0
	{
		histogram_of_labels.push_back(0);
	}

	int rows = labels.rows;
	int cols = labels.cols;
	for (int row = 0; row < rows; row++) //计算每个labels的个数
	{
		for (int col = 0; col < cols; col++)
		{
			histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
		}
	}
	histogram_of_labels.at(0) = 0; //将背景的labels个数设置为0

	//2. 计算最大的连通域labels索引
	int maximum = 0;
	int max_idx = 0;
	for (int i = 0; i < n_comps; i++)
	{
		if (histogram_of_labels.at(i) > maximum)
		{
			maximum = histogram_of_labels.at(i);
			max_idx = i;
		}
	}

	//3. 将最大连通域标记为1
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (labels.at<unsigned short>(row, col) == max_idx)
			{
				labels.at<unsigned short>(row, col) = 255;
			}
			else
			{
				labels.at<unsigned short>(row, col) = 0;
			}
		}
	}

	//4. 将图像更改为CV_8U格式
	labels.convertTo(dstImage, CV_8U);
}

cv::Mat srcImage;
cv::Mat dstImage;

int threshold1 = 100; // bar 的初值，也是阈值的初值
int threshold2 = 150; // bar 的初值，也是阈值的初值
int threshold_bar_max_value = 255; // bar的最大值
void ChangeThresholdValue(cv::Mat inputImage, cv::Mat outputImage)
{
	cv::Canny(srcImage, dstImage, threshold1, threshold2); // 边缘检测
	cv::imshow("ThresholdWin", dstImage);
}
void ChangeThreshold1(int, void*)
{
	ChangeThresholdValue(srcImage, dstImage);
}
void ChangeThreshold2(int, void*)
{
	ChangeThresholdValue(srcImage, dstImage);
}
void ThresholdFun(cv::Mat inputImage, cv::Mat outputImage)
{
	cv::namedWindow("ThresholdWin");
	cv::createTrackbar("threshold1 bar", "ThresholdWin", &threshold1, threshold_bar_max_value, ChangeThreshold1);
	ChangeThreshold1(threshold1, 0);
	cv::createTrackbar("threshold2 bar", "ThresholdWin", &threshold2, threshold_bar_max_value, ChangeThreshold2);
	ChangeThreshold2(threshold2, 0);
}

int binarization = 100; // bar 的初值，也是阈值的初值
int binarization_max_vale = 255; // 阈值的最大值
int binarization_bar_max_value = 255; // bar的最大值
void ChangeBinarizationValue(cv::Mat inputImage, cv::Mat outputImage)
{
	cv::threshold(srcImage, dstImage, binarization, binarization_max_vale, cv::THRESH_BINARY); // 二值化
	cv::imshow("BinarizationWin", dstImage);
}
void ChangeBinarization(int, void*)
{
	ChangeBinarizationValue(srcImage, dstImage);
}
void BarbinarizationFun(cv::Mat inputImage, cv::Mat outputImage)
{
	cv::namedWindow("BinarizationWin");
	cv::createTrackbar("binarization bar", "BinarizationWin", &binarization, binarization_bar_max_value, ChangeBinarization);
	ChangeBinarization(binarization, 0);
}


void Test1() {
// 原图
	cv::Mat image = cv::imread("11.jpg", cv::IMREAD_GRAYSCALE);
	//cv::Mat image = cv::imread("11.jpg", cv::IMREAD_COLOR);

	// 如果读图错误，则输出提示未读到图，并返回
	if (image.empty()) {
		std::cout << "Error reading image..." << std::endl;
		return;
	}
	//ShowImage("image", image);
	// 3024 x 3024

// 截取图片
	// 截取指定部分1512
	int x = 256, y = 1512;
	cv::Rect rect(x, y, 3024 - 2 * x + 150, 3024 - y);
	cv::Mat image_roi = image(rect);
	//ShowImage("image_roi", image_roi);

// 缩小图片
	cv::Mat image_min;
	cv::resize(image_roi, image_min, cv::Size(), 0.2, 0.2);
	//ShowImage("image_min", image_min);

// 直方图均衡化
	cv::Mat equal_hist;
	equalizeHist(image_min, equal_hist); //直方图均衡化
	//ShowImage("equal_hist", equal_hist);

	srcImage = equal_hist;
// 边缘检测
	cv::Canny(srcImage, dstImage, threshold1, threshold2);
	//ShowImage("ThresholdWin", dstImage);
	// 滑动条边缘检测
	//ThresholdFun(srcImage, dstImage);

// 二值化
	//cv::threshold(srcImage, dstImage, binarization, binarization_max_vale, cv::THRESH_BINARY);
	//ShowImage("BinarizationWin", dstImage);
	// 滑动条二值化
	//BarbinarizationFun(srcImage, dstImage);

// 膨胀操作
	cv::Mat input = dstImage;
	cv::Mat output;
	//获取自定义核
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	cv::dilate(input, output, element);
	ShowImage("Dilate", output);

// 腐蚀操作
	//腐蚀操作
	cv::erode(input, output, element);
	ShowImage("Erode", output);

// 高级形态学处理
	cv::morphologyEx(input, output, cv::MORPH_ERODE, element); // 腐蚀
	ShowImage("MorphologyEx", output);
	cv::morphologyEx(input, output, cv::MORPH_DILATE, element); // 膨胀
	ShowImage("MorphologyEx", output);
	cv::morphologyEx(input, output, cv::MORPH_OPEN, element); // 开运算
	ShowImage("MorphologyEx", output);
	cv::morphologyEx(input, output, cv::MORPH_CLOSE, element); // 闭运算
	ShowImage("MorphologyEx", output);
	cv::morphologyEx(input, output, cv::MORPH_GRADIENT, element); // 形态学梯度
	ShowImage("MorphologyEx", output);
	cv::morphologyEx(input, output, cv::MORPH_TOPHAT, element); // 顶帽
	ShowImage("MorphologyEx", output);
	cv::morphologyEx(input, output, cv::MORPH_BLACKHAT, element); // 黑帽
	ShowImage("MorphologyEx", output);

	//cv::Mat image_connect;
	//LargestConnectedComponent(image_min, image_connect);
	//ShowImage("image_connect", image_connect);

	//int initial_value = 50;
	//int max_value = 255;
	//createTrackbar("threshold", "Sliding bar", &initial_value, max_value, Changethreshold1);
	//Changethreshold1(initial_value, 0);
	//createTrackbar("threshold", "Sliding bar", &initial_value, max_value, Changethreshold2);
	//Changethreshold2(initial_value, 0);

}
