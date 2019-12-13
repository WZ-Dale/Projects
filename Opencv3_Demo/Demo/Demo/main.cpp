#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\loadDisplaySave.h"
//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\mat.h"
//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\logo.h"

//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\saltImage.h"
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\colorReduce.h"
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\contrast.h"
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\addImages.h"
#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\remapping.h"

#include "E:\Projects\Opencv3_Demo\3_colorDetection_colordetector_extractObject_huesaturation\colorDetection.h"

using namespace std;
using namespace cv;

int main() {
// 1
	//loadDisplaySave_Test();
	//mat_Test();
	//logo_Test();
// 2
	//saltImage_Test();
	//colorReduce_Test();
	//contrast_Test();
	//addImages_Test();
	//remapping_Test();
// 3
	colorDetection_Test();


	//destroyAllWindows();
	waitKey(0);
	system("pause");
	return  0;
}

