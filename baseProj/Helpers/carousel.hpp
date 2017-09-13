#ifndef carouselH
#define carouselH
#include <vector>
#include <string>
#include "ImageClass.hpp"

using namespace std;

class carousel {
private:
	ImageClass* leftImage;
	ImageClass* rightImage;
	std::vector<string> imgList;
	std::vector<string>::iterator itImgList;
public:
	carousel(ImageClass* lI, ImageClass* rI){
		this->leftImage = lI;
		this->rightImage = rI;
	}
	void previousImage();
	void nextImage();
	void addImage(string imgPath);
	void start();
};
#endif
