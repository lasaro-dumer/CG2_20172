#ifndef utilH__
#define utilH__
#include <array>
#include "ImageClass.hpp"
class util {
public:
	int static medianWindowSize;
	int static iGtJ (int* i,int* j) { return (*i>*j); }

	void static CreateHistogram(ImageClass* image, ImageClass* resultImage);
	void static MedianFilter(ImageClass* image, ImageClass* resultImage, int windowSize);
	void static LoadNewImage(string path, ImageClass* image, ImageClass* resultImage);
	void static CopyResultToMain(ImageClass* image, ImageClass* resultImage);
};

#endif /*utilH__*/
