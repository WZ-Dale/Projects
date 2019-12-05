#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\loadDisplaySave.h"
//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\mat.h"
//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\logo.h"

#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\saltImage.h"
#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\colorReduce.h"


using namespace std;
using namespace cv;

int main() {
	//loadDisplaySave_Test();
	//mat_Test();
	//logo_Test();

	//saltImage_Test();
	colorReduce_Test();


	//destroyAllWindows();
	waitKey(0);
	system("pause");
	return  0;
}

