function undistorted_img = undistortImage(img, K, D)
% Corrects an image for lens distortion.
% K为内参矩阵，用来归一化坐标的，给定的
% D为径向畸变参数，给定的
 
[height, width] = size(img);
 
fx = K(1,1);
fy = K(2,2);
cx = K(1,3);
cy = K(2,3);
 
 
%Finish image undistorted function
undistorted_img = uint8(zeros(height, width));
 
% 依次找到去畸变后的图像坐标（y,x）对应的畸变坐标
for y = 1:height
    for x = 1:width
 
        % 第一步，通过内参矩阵归一化畸变图像的坐标       
        x1 = (x-cx)/fx;  
        y1 = (y-cy)/fy;
        
        % 第二步，通过径向畸变模型得到归一化的畸变坐标
        r2 = x1^2+y1^2;
        x2  = x1*(1+D(1)*r2+D(2)*r2^2); 
        y2 = y1*(1+D(1)*r2+D(2)*r2^2);
        
        % 第三步，坐标映射回去，去归一化，得到畸变坐标。
        u_distorted = fx*x2+cx;  % 列
        v_distorted = fy*y2+cy;  % 行
        
        % 通过上面的步骤，我们找到了正常图像坐标（y,x）对应的畸变图像坐标（u_distorted, v_distorted），下一步就是赋值了，这里用的是最近邻插值法
 
        % 第四步，最近邻插值
        if (u_distorted >= 0 && v_distorted >= 0 && u_distorted < width && v_distorted < height) % 防止行列越界
            undistorted_img(y, x) = img(round(v_distorted), round(u_distorted)); % 通过round函数取最近的数字
        else
            undistorted_img(y, x) = 0;
        end
    end
end
 
end
 