function [height_divise_lable,width_divise_lable]=fengezuobiao(hengxiang_number,zongxiang_number,I_gray)


%%%%hengxiang_number   %%%%%图像横向矩形个数
%%%%zongxiang_numbe;    %%%%%图像纵向矩形个数
K=0.1;   %各个不同矩形边长比例
height_divise_lable=zeros(zongxiang_number+1,1);   %%%%%纵向四边形分割像素坐标
width_divise_lable=zeros(hengxiang_number+1,1);     %6%%%横向四边形分割像素坐标
[height,width]=size(I_gray);
s1=0;
for i=1:(zongxiang_number-1)/2
s1=s1+2*(i-1)*K+2;
end
s1=s1+(zongxiang_number-1)/2*K+1;

zong_edge=fix(height/s1);
height_divise_lable(1)=0;
for i=2:zongxiang_number
    if i<=((zongxiang_number+1)/2+1)
        
   height_divise_lable(i)= fix(height_divise_lable(i-1)+zong_edge*(1+K*(i-2)));
    else
       height_divise_lable(i) = fix(height_divise_lable(i-1)+zong_edge*(1+K*(zongxiang_number-i+1)));
    end
end
height_divise_lable(zongxiang_number+1)=height;


s2=0;
for i=1:(hengxiang_number-1)/2
s2=s2+2*(i-1)*K+2;
end
s2=s2+(hengxiang_number-1)/2*K+1;

heng_edge=fix(width/s2);
width_divise_lable(1)=0;
for i=2:hengxiang_number
    if i<=((hengxiang_number+1)/2+1)
        
  width_divise_lable(i)=fix(width_divise_lable(i-1)+heng_edge*(1+K*(i-2)));
    else
      width_divise_lable(i) = fix(width_divise_lable(i-1)+heng_edge*(1+K*(hengxiang_number-i+1)));
    end
end
width_divise_lable(hengxiang_number+1)=width;


end

