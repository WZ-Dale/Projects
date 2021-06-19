
image=imread('原图.png');

%%%%%%%%og=[]

I_gray=rgb2gray(image);
b=double(image);
n=size(gm(:,1));


%%%%%计算四边形分割后的轴坐标
[height_divise_lable,width_divise_lable]=fengezuobiao(hengxiang_number,zongxiang_number,I_gray) ;  %%%%四边形分割坐标

distortion_factor_matix=gmodify(pic,gm,og)


[hh,ww]=size(b(:,:,1));
sps=zeros(hh,ww,3)+255;

for i=1:zongxiang_number
    for j=1:hengxiang_number
        
a0=distortion_factor_matix(:,1,(i-1)*zongxiang_number+j);   %%%%%%%记录各个四边形的畸变参数
b0=distortion_factor_matix(:,2,(i-1)*zongxiang_number+j);
[h,w]=size(b(height_divise_lable(i)+1:height_divise_lable(i+1),width_divise_lable(j)+1:width_divise_lable(j+1),1));
sps(height_divise_lable(i)+1:height_divise_lable(i+1),width_divise_lable(j)+1:width_divise_lable(j+1),:)=zeros(h,w,3)+255;
for ii=height_divise_lable(i)+1:height_divise_lable(i)+h     %从理想图像矩阵出发处理
    for jj=width_divise_lable(j)+1:width_divise_lable(j)+w
        x=[1,ii-og(1),jj-og(2),(ii-og(2))*(jj-og(1))];
        u=x*a0+og(2);  % 逆向映射（j，i）到畸变图像矩阵（v，u）
        v=x*b0+og(1);
        if (u>width_divise_lable(j))&&(u<width_divise_lable(j)+w)&&(v>height_divise_lable(i))&&(v<height_divise_lable(i)+h) %处理在图像大小范围内的像素点
            if u<=1
                uu=floor(u)+1;   %对u取整
            else
                uu=floor(u);
            end
             if v<=1
                vv=floor(v)+1;   %对u取整
            else
                vv=floor(v);
            end  %对v取整
            arf=u-uu;     %计算上面提到的
            bta=v-vv;     %计算上面提到的
            for k=1:3         %进行灰度双线性插值
                ft1=(1-bta)*b(vv,uu,k)+bta*b(vv+1,uu,k);
                ft2=(1-bta)*b(vv,uu+1,k)+bta*b(vv+1,uu+1,k);
                sps(ii,jj,k)=(1-arf)*ft1+arf*ft2;
            end
        end
    end
end

    end
   
end

imshow(uint8(sps));      %显示校正图像




