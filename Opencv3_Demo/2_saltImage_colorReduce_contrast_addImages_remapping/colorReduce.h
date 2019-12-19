// 20191205
// 学习并注释 by wangze

// 在编写图像处理函数时，需要充分考虑运行效率问题。但事实上，除非确实必要，不要以牺牲代码的清晰度来优化性能。
// 简洁的代码总是更容易调试和维护，只有对程序效率至关重要的代码段，才需要进行重度优化。 

#pragma once

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


// 就地处理
// 1st version
// see recipe Scanning an image with pointers
void colorReduce(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines	// 行数
      int nc= image.cols * image.channels(); // total number of elements per line	// 每行元素总数

      for (int j=0; j<nl; j++) {

		  // 取得行j的地址
          // get the address of row j
          uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {
			
			// 处理每个像素
            // process each pixel ---------------------
			// 基本减色算法
            data[i]= data[i]/div*div + div/2;

            // end of pixel processing ----------------

          } // end of line
      }
}

// 使用输入和输出参数，亦可就地处理
// version with input/ouput images
// see recipe Scanning an image with pointers
void colorReduceIO(const cv::Mat &image, // input image
	               cv::Mat &result,      // output image
	               int div = 64) {

	int nl = image.rows; // number of lines
	int nc = image.cols; // number of columns
	int nchannels = image.channels(); // number of channels

	// 如果需要的话，重新分配矩阵
	// allocate output image if necessary
	result.create(image.rows, image.cols, image.type());

	for (int j = 0; j<nl; j++) {

		// 获得第j行的输入和输出的地址
		// get the addresses of input and output row j
		const uchar* data_in = image.ptr<uchar>(j);
		uchar* data_out = result.ptr<uchar>(j);

		for (int i = 0; i<nc*nchannels; i++) {

			// process each pixel ---------------------

			data_out[i] = data_in[i] / div*div + div / 2;

			// end of pixel processing ----------------

		} // end of line
	}
}

// 解引用*
// Test 1
// this version uses the dereference operator *
void colorReduce1(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
	  uchar div2 = div >> 1; // div2 = div/2

      for (int j=0; j<nl; j++) {

          uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {

            
			  // process each pixel ---------------------

			  *data++= *data/div*div + div2;

			  // end of pixel processing ----------------

          } // end of line
      }
}

// 取模
// Test 2
// this version uses the modulo operator
void colorReduce2(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
	  uchar div2 = div >> 1; // div2 = div/2

      for (int j=0; j<nl; j++) {

          uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {

            // process each pixel ---------------------

                 int v= *data;
                 *data++= v - v%div + div2;

            // end of pixel processing ----------------

          } // end of line
      }
}

// 掩码
// Test 3
// this version uses a binary mask
void colorReduce3(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);

	  // 用来截取像素的值的掩码
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
      uchar div2= 1<<(n-1); // div2 = div/2

      for (int j=0; j<nl; j++) {

          uchar* data= image.ptr<uchar>(j);

		  for (int i = 0; i < nc; i++) {

			  // process each pixel ---------------------

			  *data &= mask;     // masking
			  *data++ |= div2;   // add div/2

            // end of pixel processing ----------------

          } // end of line
      }
}

// 带有二进制掩码的直接指针算法
// Test 4
// this version uses direct pointer arithmetic with a binary mask
void colorReduce4(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);

	  // step属性可以得到一行的总字节数（包括填充像素）
      int step= image.step; // effective width

	  // 用来截取像素的值的掩码
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
	  uchar div2 = div >> 1; // div2 = div/2

	  // data属性表示内存块第一个元素的地址，返回一个无符号字符型的指针
      // get the pointer to the image buffer
      uchar *data= image.data;

      for (int j=0; j<nl; j++) {

          for (int i=0; i<nc; i++) {

            // process each pixel ---------------------

            *(data+i) &= mask;
            *(data+i) += div2;

            // end of pixel processing ----------------

          } // end of line

          data+= step;  // next line
      }
}

// 每次重新计算行大小
// Test 5
// this version recomputes row size each time
void colorReduce5(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0

      for (int j=0; j<nl; j++) {

          uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<image.cols * image.channels(); i++) {

            // process each pixel ---------------------

            *data &= mask;
            *data++ += div/2;

            // end of pixel processing ----------------

          } // end of line
      }
}

// 优化连续图像的情况，一维数组
// Test 6
// this version optimizes the case of continuous image
void colorReduce6(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines
      int nc= image.cols * image.channels(); // total number of elements per line

      if (image.isContinuous())  {
          // then no padded pixels
          nc= nc*nl;
          nl= 1;  // it is now a 1D array
       }

      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
	  uchar div2 = div >> 1; // div2 = div/2

     // this loop is executed only once
     // in case of continuous images
      for (int j=0; j<nl; j++) {

          uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {

            // process each pixel ---------------------

            *data &= mask;
            *data++ += div2;

            // end of pixel processing ----------------

          } // end of line
      }
}

// reshape方法
// Test 7
// this versions applies reshape on continuous image
void colorReduce7(cv::Mat image, int div=64) {

      if (image.isContinuous()) {
		// 修改矩阵维数
        // no padded pixels
        image.reshape(1,   // new number of channels
                      1) ; // new number of rows
      }
      // number of columns set accordingly

      int nl= image.rows; // number of lines
      int nc= image.cols*image.channels() ; // number of columns

      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
	  uchar div2 = div >> 1; // div2 = div/2

      for (int j=0; j<nl; j++) {

          uchar* data= image.ptr<uchar>(j);

          for (int i=0; i<nc; i++) {

            // process each pixel ---------------------

            *data &= mask;
            *data++ += div2;

            // end of pixel processing ----------------

          } // end of line
      }
}

// 迭代器的使用，对以上方法进行改写。但迭代器运行速度较慢，主要是问了简化扫描过程，降低出错的可能。
// Test 8
// this version processes the 3 channels inside the loop with Mat_ iterators
void colorReduce8(cv::Mat image, int div=64) {

      // get iterators
      cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
      cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();
	  uchar div2 = div >> 1; // div2 = div/2

      for ( ; it!= itend; ++it) {

        // process each pixel ---------------------

        (*it)[0]= (*it)[0]/div*div + div2;
        (*it)[1]= (*it)[1]/div*div + div2;
        (*it)[2]= (*it)[2]/div*div + div2;

        // end of pixel processing ----------------
      }
}

// Test 9
// this version uses iterators on Vec3b
void colorReduce9(cv::Mat image, int div=64) {

      // get iterators
      cv::MatIterator_<cv::Vec3b> it= image.begin<cv::Vec3b>();
      cv::MatIterator_<cv::Vec3b> itend= image.end<cv::Vec3b>();

      const cv::Vec3b offset(div/2,div/2,div/2);

      for ( ; it!= itend; ++it) {

        // process each pixel ---------------------

        *it= *it/div*div+offset;
        // end of pixel processing ----------------
      }
}

// Test 10
// this version uses iterators with a binary mask
void colorReduce10(cv::Mat image, int div=64) {

      // div must be a power of 2
      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
	  uchar div2 = div >> 1; // div2 = div/2

      // get iterators
      cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
      cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();

      // scan all pixels
      for ( ; it!= itend; ++it) {

        // process each pixel ---------------------

        (*it)[0]&= mask;
        (*it)[0]+= div2;
        (*it)[1]&= mask;
        (*it)[1]+= div2;
        (*it)[2]&= mask;
        (*it)[2]+= div2;

        // end of pixel processing ----------------
      }
}

// Test 11
// this versions uses ierators from Mat_ 
void colorReduce11(cv::Mat image, int div=64) {

      // get iterators
      cv::Mat_<cv::Vec3b> cimage= image;
      cv::Mat_<cv::Vec3b>::iterator it=cimage.begin();
      cv::Mat_<cv::Vec3b>::iterator itend=cimage.end();
	  uchar div2 = div >> 1; // div2 = div/2

      for ( ; it!= itend; it++) {

        // process each pixel ---------------------

        (*it)[0]= (*it)[0]/div*div + div2;
        (*it)[1]= (*it)[1]/div*div + div2;
        (*it)[2]= (*it)[2]/div*div + div2;

        // end of pixel processing ----------------
      }
}

// 实现用at方法访问像素的函数，该方法运行速度较慢。只有在需要随机访问像素的时候使用，不要在扫描图像时使用。
// Test 12
// this version uses the at method
void colorReduce12(cv::Mat image, int div=64) {

      int nl= image.rows; // number of lines
      int nc= image.cols; // number of columns
	  uchar div2 = div >> 1; // div2 = div/2

      for (int j=0; j<nl; j++) {
          for (int i=0; i<nc; i++) {

            // process each pixel ---------------------

                  image.at<cv::Vec3b>(j,i)[0]=	 image.at<cv::Vec3b>(j,i)[0]/div*div + div2;
                  image.at<cv::Vec3b>(j,i)[1]=	 image.at<cv::Vec3b>(j,i)[1]/div*div + div2;
                  image.at<cv::Vec3b>(j,i)[2]=	 image.at<cv::Vec3b>(j,i)[2]/div*div + div2;

            // end of pixel processing ----------------

          } // end of line
      }
}


// Test 13
// this version uses Mat overloaded operators
void colorReduce13(cv::Mat image, int div=64) {

      int n= static_cast<int>(log(static_cast<double>(div))/log(2.0) + 0.5);
      // mask used to round the pixel value
      uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0

      // perform color reduction
      image=(image&cv::Scalar(mask,mask,mask))+cv::Scalar(div/2,div/2,div/2);
}

// Test 14
// this version uses a look up table
void colorReduce14(cv::Mat image, int div=64) {

      cv::Mat lookup(1,256,CV_8U);// 创建一维查找表
		//定义减色查找表的值
      for (int i=0; i<256; i++) {

        lookup.at<uchar>(i)= i/div*div + div/2;
      }
		// 对每个通道应用查找表
      cv::LUT(image,lookup,image);
}

#define NTESTS 15
#define NITERATIONS 10

int colorReduce_Test (){
	// read the image
	cv::Mat image = cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg");

	// 计算图像处理时间
	// time and process the image
	const int64 start = cv::getTickCount();
	colorReduce(image, 64);
	//Elapsed time in seconds
	double duration = (cv::getTickCount() - start) / cv::getTickFrequency();
	// 其中：getTickCount()返回计算机开机到当前的时钟周期数，getTickFrequency()返回每秒的时钟周期数
	
	// display the image
	std::cout << "Duration= " << duration << "secs" << std::endl;
	cv::namedWindow("Image");
	cv::imshow("Image", image);

	cv::waitKey();


	// test different versions of the function
	int64 t[NTESTS], tinit;
	// timer values set to 0
	for (int i = 0; i<NTESTS; i++)
		t[i] = 0;

	cv::Mat images[NTESTS];
	cv::Mat result;

	// the versions to be tested
	typedef void(*FunctionPointer)(cv::Mat, int);
	FunctionPointer functions[NTESTS] = { colorReduce, colorReduce1, colorReduce2, colorReduce3, colorReduce4,
										  colorReduce5, colorReduce6, colorReduce7, colorReduce8, colorReduce9,
										  colorReduce10, colorReduce11, colorReduce12, colorReduce13, colorReduce14};
	// 重复测试几次，为了求平均值
	// repeat the tests several times
	int n = NITERATIONS;
	for (int k = 0; k<n; k++) {

		std::cout << k << " of " << n << std::endl;

		// 测试每一种函数的执行时间
		// test each version
		for (int c = 0; c < NTESTS; c++) {

			images[c] = cv::imread("E:\\Projects\\Opencv3_Demo\\images\\boldt.jpg");

			// set timer and call function
			tinit = cv::getTickCount();
			functions[c](images[c], 64);
			t[c] += cv::getTickCount() - tinit;

			std::cout << ".";
		}

		std::cout << std::endl;
	}

	// short description of each function
	std::string descriptions[NTESTS] = {
		"original version:",
		"with dereference operator:",
		"using modulo operator:",
		"using a binary mask:",
		"direct ptr arithmetic:",
		"row size recomputation:",
		"continuous image:",
		"reshape continuous image:",
		"with iterators:",
		"Vec3b iterators:",
		"iterators and mask:",
		"iterators from Mat_:",
		"at method:",
		"overloaded operators:",
		"look-up table:",
	};

	for (int i = 0; i < NTESTS; i++) {

		cv::namedWindow(descriptions[i]);
		cv::imshow(descriptions[i], images[i]);
	}

	// 打印平均执行时间
	// print average execution time
	std::cout << std::endl << "-------------------------------------------" << std::endl << std::endl;
	for (int i = 0; i < NTESTS; i++) {
		std::cout << i << ". " << descriptions[i] << t[i] / cv::getTickFrequency() / n << "s" << std::endl;
		std::cout << i << ". " << descriptions[i] << 1000.*t[i] / cv::getTickFrequency() / n << "ms" << std::endl;
	}

	cv::waitKey();
	return 0;
}