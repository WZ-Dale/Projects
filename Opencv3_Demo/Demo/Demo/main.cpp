#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//1
//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\loadDisplaySave.h"
//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\mat.h"
//#include "E:\Projects\Opencv3_Demo\1_loadDisplaySave_mat_logo\logo.h"
//2
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\saltImage.h"
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\colorReduce.h"
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\contrast.h"
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\addImages.h"
//#include "E:\Projects\Opencv3_Demo\2_saltImage_colorReduce_contrast_addImages_remapping\remapping.h"
//3
//#include "E:\Projects\Opencv3_Demo\3_colorDetection_colordetector_extractObject_huesaturation\colorDetection.h"
//#include "E:\Projects\Opencv3_Demo\3_colorDetection_colordetector_extractObject_huesaturation\extractObject.h"
//#include "E:\Projects\Opencv3_Demo\3_colorDetection_colordetector_extractObject_huesaturation\huesaturation.h"
//4
//#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\histograms.h"
//#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\contentFinders.h"
//#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\finder.h"
//#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\retrieve.h"
//#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\integrals.h"
#include "E:\Projects\Opencv3_Demo\4_histogram_finder_imageComparator_integral\tracking.h"


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
	//colorDetection_Test();
	//extractObject_Test();
	//huesaturation_Test();
//4
	//histograms_Test();
	//contentFinder_Test();
	//finder_Test();
	//retrieve_Test();
	//integral_Test();
	tracking_Test();

	//destroyAllWindows();
	waitKey(0);
	system("pause");
	return  0;
}

