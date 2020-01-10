// 20200108
// 学习并注释 by wangze

#if !defined LINEF
#define LINEF

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#define PI 3.1415926

class LineFinder {

  private:

	  // original image
	  cv::Mat img;

	  // vector containing the end points （包含被检测侧直线的端点的向量）
	  // of the detected lines
	  std::vector<cv::Vec4i> lines;

	  // accumulator resolution parameters（累加器分辨率参数）
	  double deltaRho;
	  double deltaTheta;

	  // minimum number of votes that a line （确认直线之前必须收到最小投票数）
	  // must receive before being considered
	  int minVote;

	  // min length for a line（直线的最小长度）
	  double minLength;

	  // max allowed gap along the line（直线上允许的最大空隙）
	  double maxGap;

  public:

	  // Default accumulator resolution is 1 pixel by 1 degree（默认累加器分辨率为1像素，1度）
	  // no gap, no mimimum length（没有空隙，没有最小长度）
	  LineFinder() : deltaRho(1), deltaTheta(PI/180), minVote(10), minLength(0.), maxGap(0.) {}

	  // Set the resolution of the accumulator（设置累加器的分辨率）
	  void setAccResolution(double dRho, double dTheta) {

		  deltaRho= dRho;
		  deltaTheta= dTheta;
	  }

	  // Set the minimum number of votes（设置最小投票数）
	  void setMinVote(int minv) {

		  minVote= minv;
	  }

	  // Set line length and gap（设置直线长度和空隙）
	  void setLineLengthAndGap(double length, double gap) {

		  minLength= length;
		  maxGap= gap;
	  }

	  // Apply probabilistic Hough Transform（应用概率霍夫变换）
	  std::vector<cv::Vec4i> findLines(cv::Mat& binary) {

		  lines.clear();
		  cv::HoughLinesP(binary,lines,deltaRho,deltaTheta,minVote, minLength, maxGap);

		  return lines;
	  }

	  // Draw the detected lines on an image（在图像上绘制检测到的直线）
	  void drawDetectedLines(cv::Mat &image, cv::Scalar color=cv::Scalar(255,255,255)) {
	
		  // Draw the lines（画直线）
		  std::vector<cv::Vec4i>::const_iterator it2= lines.begin();
	
		  while (it2!=lines.end()) {
		
			  cv::Point pt1((*it2)[0],(*it2)[1]);        
			  cv::Point pt2((*it2)[2],(*it2)[3]);

			  cv::line( image, pt1, pt2, color);
		
			  ++it2;	
		  }
	  }

	  // Eliminates lines that do not have an orientation equals to
	  // the ones specified in the input matrix of orientations
	  // At least the given percentage of pixels on the line must 
	  // be within plus or minus delta of the corresponding orientation
	  std::vector<cv::Vec4i> removeLinesOfInconsistentOrientations(
		  const cv::Mat &orientations, double percentage, double delta) {

			  std::vector<cv::Vec4i>::iterator it= lines.begin();
	
			  // check all lines
			  while (it!=lines.end()) {

				  // end points
				  int x1= (*it)[0];
				  int y1= (*it)[1];
				  int x2= (*it)[2];
				  int y2= (*it)[3];
		   
				  // line orientation + 90o to get the parallel line
				  double ori1= atan2(static_cast<double>(y1-y2),static_cast<double>(x1-x2))+PI/2;
				  if (ori1>PI) ori1= ori1-2*PI;

				  double ori2= atan2(static_cast<double>(y2-y1),static_cast<double>(x2-x1))+PI/2;
				  if (ori2>PI) ori2= ori2-2*PI;
	
				  // for all points on the line
				  cv::LineIterator lit(orientations,cv::Point(x1,y1),cv::Point(x2,y2));
				  int i,count=0;
				  for(i = 0, count=0; i < lit.count; i++, ++lit) { 
		
					  float ori= *(reinterpret_cast<float *>(*lit));

					  // is line orientation similar to gradient orientation ?
					  if (std::min(fabs(ori-ori1),fabs(ori-ori2))<delta)
						  count++;
		
				  }

				  double consistency= count/static_cast<double>(i);

				  // set to zero lines of inconsistent orientation
				  if (consistency < percentage) {
 
					  (*it)[0]=(*it)[1]=(*it)[2]=(*it)[3]=0;

				  }

				  ++it;
			  }

			  return lines;
	  }
};


#endif
