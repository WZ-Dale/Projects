%%%%%%质心求取程序

%
function [plot_x,plot_y]=calculate_zhixin(I)
 I=imread('原图.png');%读取原图像  
figure(1);imshow(I);%显示原图像  
  
I_gray=rgb2gray(I);%原图像变为灰度图像  



I=imcomplement(I_gray);
T=imtophat(I,strel('disk',10));
I_bw=~im2bw(T,graythresh(T));
figure(2),imshow(I_bw)





%level=graythresh(I_gray);%计算图像I_gray的全局阈值，level为标准化灰度值，其范围为[0 1]  
[height,width]=size(I_gray);%计算灰度图像的长宽  
%I_bw=im2bw(I_gray,level);%im2bw使用阈值level将灰度图像转换为二值图像．  
%figure(2);imshow(I_bw);%显示二值图像（背景为白色）  
  


for i=1:height %%循环中进行反色  
    for j=1:width     
        if I_bw(i,j)==1        
            I_bw(i,j)=0;    
        else I_bw(i,j)=1;   
        end  
    end  
end  
figure(3);imshow(I_bw);%显示取反后的二值图像（背景为黑色）  


  
[L,num]=bwlabel(I_bw,8);%bwlabel标注二值图像I_bw中的目标物体，返回标识矩阵Ｌ和I_bw中目标物体的数量num，８表示连通数．  
plot_x=zeros(num,1);%%zeros(m,n)产生m×n的全0矩阵．用于记录质心位置的横坐标  
plot_y=zeros(num,1);%zeros(m,n)产生m×n的全0矩阵．用于记录质心位置的纵坐标  
  
for k=1:num  %%num个区域依次统计质心位置      
    sum_x=0;    sum_y=0;    area=0; %初始化  
    for i=1:height     
        for j=1:width   
            if L(i,j)==k       
                sum_x=sum_x+i;  %计算第Ｋ区域的横坐标总和  
                sum_y=sum_y+j;  %计算第Ｋ区域的纵坐标总和   
                area=area+1;    %计算第Ｋ区域的由多少个坐标点表示  
            end  
        end  
    end  
    plot_x(k)=fix(sum_x/area);  %计算第Ｋ区域的质心横坐标  
    plot_y(k)=fix(sum_y/area);%计算第Ｋ区域的质心纵坐标  
end  
  
figure(4);imshow(I_bw);%显示取反后的二值图像（背景为黑色）,并在图上标注质心点位置  
for i=1:num  
    hold  on  
    plot(plot_y(i) ,plot_x(i), '*')  
end  
zhixin=[plot_x,plot_y];
end
