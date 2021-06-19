% ПЊБедЫЫу

clear all;
close all;
clc;

a = imread('D:\Tire_Test\image5\13.png');
subplot(1,2,1);
imshow(a),title('sourcPic');
b = imclose( a, ones(10,3) );
subplot(1,2,2);
imshow(b),title('close');

