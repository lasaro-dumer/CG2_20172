#ifndef utilH__
#define utilH__
#include <array>
#include "ImageClass.hpp"
#define CGM_MEDIAN_SIZE 1
#define CGM_THRESHOLD 2
#define CGM_REGION 3
#define CGM_THRESHOLD_SEG 4

class util {
	int static currentMode;
	int static medianWindowSize;
	int static threshold;
	array<int,255> static levels;
public:
	int static iGtJ (int* i,int* j) { return (*i>*j); }

	void static CreateHistogram(ImageClass* image, ImageClass* resultImage);
	void static MedianFilter(ImageClass* image, ImageClass* resultImage);
	void static MedianSegmentation(ImageClass* image, ImageClass* resultImage);
	void static SobelFilter(ImageClass* image, ImageClass* resultImage);
	void static HighPassFilter(ImageClass* image, ImageClass* resultImage);
	void static BinaryDilation(ImageClass* image, ImageClass* resultImage);
	void static ThresholdSegmentation(ImageClass* image, ImageClass* resultImage);
	void static LoadNewImage(string path, ImageClass* image, ImageClass* resultImage);
	void static CopyResultToMain(ImageClass* image, ImageClass* resultImage);
	void static SwitchMode(int newMode);
	void static IncValue();
	void static DecValue();
};

#endif /*utilH__*/
