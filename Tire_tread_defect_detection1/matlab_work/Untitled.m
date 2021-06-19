clear all;
close all;
clc;

rgb = imread('D:\Tire_Test\image2\4.jpg');
gray = rgb2gray(rgb);
P = gray;
%%将灰度图像中大于阈值的区域标记为红色
[row,col]=size(P);
u=0.9;
H=zeros(row,col,3);
H(:,:,1)=(P<u).*P+(P>u);
H(:,:,2)=(P<u).*P;      
H(:,:,3)=(P<u).*P;
figure,imshow(H);