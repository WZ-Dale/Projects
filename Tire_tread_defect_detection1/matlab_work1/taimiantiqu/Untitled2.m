% clear all;
close all;
clc;

I_origin2 = imread('D:\Tire_Test\image6\1.png');
I_gray = rgb2gray(I_origin2);
Edge_gray = edge(I_gray, 'log');
subplot(4,5,1);imshow(Edge_gray),title('Edge_gray');

Instance_R = I_origin2(:, :, 1);
Instance_G = I_origin2(:, :, 2);
Instance_B = I_origin2(:, :, 3);
Edge_R = edge(Instance_R, 'log');
Edge_G = edge(Instance_G, 'log');
Edge_B = edge(Instance_B, 'log');
subplot(4,5,2);imshow(Edge_gray),title('Edge_R');
subplot(4,5,3);imshow(Edge_gray),title('Edge_G');
subplot(4,5,4);imshow(Edge_gray),title('Edge_B');

rgb = im2uint8(cat(3, Edge_R, Edge_G, Edge_B));
subplot(4,5,5);imshow(rgb),title('rgb');
I_RGB = rgb2gray(rgb);
subplot(4,5,6);imshow(I_RGB),title('I_RGB');


