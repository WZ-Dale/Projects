% 四个角度的 Radon变换

clear all;
close all;
clc;

rgb = imread('D:\Tire_Test\image3\Radon\a.jpg');
gray = rgb2gray(rgb);
%gray = histeq(gray); 
%ed = edge(grayPic,'canny');
% subplot(2,5,1);
% imshow(gray),title('sourcPic');
% 
% r  = radon(gray,0);
% subplot(2,5,2);
% plot(r),title('Radon 0');
% 
% r1 = radon(gray,45);
% subplot(2,5,3);
% plot(r1),title('Radon 45');

r2 = radon(gray,90);
% subplot(2,5,4);
plot(r2),title('Radon 90');
hold on;
b = smooth(r2,100,'moving');
plot(b),title('Radon 90');


% r3 = radon(gray,135);
% subplot(2,5,5);
% plot(r3),title('Radon 135');



% rgb = imread('D:\Tire_Test\image3\e2.jpg');
% gray =rgb2gray(rgb);
% % gray = histeq(gray);
% % thresh = graythresh(gray);
% % thresh = 0.4;
% % gray = im2bw(gray,thresh);
% %ed = edge(grayPic,'canny');
% subplot(2,5,6);
% imshow(gray),title('sourcPic');
% 
% r  = radon(gray,0);
% subplot(2,5,7);
% plot(r),title('Radon 0');
% 
% r1 = radon(gray,45);
% subplot(2,5,8);
% plot(r1),title('Radon 45');
% 
% r2 = radon(gray,90);
% subplot(2,5,9);
% plot(r2),title('Radon 90');
% 
% r3 = radon(gray,135);
% subplot(2,5,10);
% plot(r3),title('Radon 135');