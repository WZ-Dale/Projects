% 胎面提取
% clear all;
close all;
clc;
% 原图
rgb =imread('D:\Tire_Test\image6\2.png');
subplot(4,5,1);imshow(rgb),title('rgb');
% 灰度图
gray =rgb2gray(rgb);
subplot(4,5,2);imshow(gray),title('gray');
% 直方图
subplot(4,5,3);imhist(gray);title('原图的直方图');
% 调节灰度图像
J=imadjust(gray,[0.15 0.9],[0 1]);
subplot(4,5,4);imshow(J);title('新直方图得到的新图像');
subplot(4,5,5);imhist(J);title('利用imadjust调节后的新直方图');
% 直方图均衡化
K=histeq(gray);
subplot(4,5,6);imshow(K);title('由均衡化后的直方图得到的均衡后的图像');
subplot(4,5,7);imhist(K);title('均衡化后的直方图');
% 边缘检测1
I=gray;
BW1=edge(I,'Roberts',0.16,'both');
BW2=edge(I,'Sobel',0.16,'both');
BW3=edge(I,'Prewitt',0.06,'both');
BW4=edge(I,'LOG',0.012,'both'); 
BW5=edge(I,'Canny','both');
% BW1=edge(I,'Roberts');
% BW2=edge(I,'Sobel');
% BW3=edge(I,'Prewitt');
% BW4=edge(I,'LOG'); 
% BW5=edge(I,'Canny');
subplot(4,5,8);imshow(I);title('原图');
subplot(4,5,11);imshow(BW1);title('Robert算子边缘检测')
subplot(4,5,12);imshow(BW2);title('Sobel算子边缘检测')
subplot(4,5,13);imshow(BW3);title('Prewitt算子边缘检测');
subplot(4,5,14);imshow(BW4);title('LOG算子边缘检测');
subplot(4,5,15);imshow(BW5);title('Canny边缘检测');
% 边缘检测2
I=J;
BW1=edge(I,'Roberts');
BW2=edge(I,'Sobel');
BW3=edge(I,'Prewitt');
BW4=edge(I,'LOG'); 
BW5=edge(I,'Canny');
subplot(4,5,9);imshow(I);title('原图');
subplot(4,5,16);imshow(BW1);title('Robert算子边缘检测')
subplot(4,5,17);imshow(BW2);title('Sobel算子边缘检测')
subplot(4,5,18);imshow(BW3);title('Prewitt算子边缘检测');
subplot(4,5,19);imshow(BW4);title('LOG算子边缘检测');
subplot(4,5,20);imshow(BW5);title('Canny边缘检测');