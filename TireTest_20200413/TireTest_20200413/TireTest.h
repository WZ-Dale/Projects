#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>


// 鼠标事件
void onMouse(int event, int x, int y, int flags, void* param);
// 展示图片，可点击
void ShowImage(const cv::String &str, cv::Mat &image);
// 二值化
void ChangeBinarizationValue(cv::Mat inputImage, cv::Mat outputImage);
void ChangeBinarization(int, void*);
void BarbinarizationFun(cv::Mat inputImage, cv::Mat outputImage);
// 边缘检测
void ChangeThresholdValue(cv::Mat inputImage, cv::Mat outputImage);
void ChangeThreshold1(int, void*);
void ChangeThreshold2(int, void*);
void ThresholdFun(cv::Mat inputImage, cv::Mat outputImage);

// 闭运算

// 最大连通区域
void LargestConnectedComponent(cv::Mat srcImage, cv::Mat &dstImage);


void Test1();



