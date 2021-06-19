% D:\Tire_Test\image3\1.jpg

close;clear;clc;
I=im2double(imread('D:\Tire_Test\image3\1.jpg'));
k=graythresh(I);              %得到最优阈值
J=im2bw(I,k);                  %转换成二值图，k为分割阈值
subplot(121);imshow(I); 
subplot(122);imshow(J);   