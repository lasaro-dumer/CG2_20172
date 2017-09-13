#include <vector>
#include <string>
#include "ImageClass.hpp"
#include "util.hpp"
#include "carousel.hpp"

using namespace std;

void carousel::previousImage() {
	if(this->itImgList == this->imgList.begin())
		this->itImgList = this->imgList.end();
	--this->itImgList;
	util::LoadNewImage(*this->itImgList, this->leftImage, this->rightImage);
}
void carousel::nextImage() {
	++this->itImgList;
	if(this->itImgList == this->imgList.end())
		this->itImgList = this->imgList.begin();
	util::LoadNewImage(*this->itImgList, this->leftImage, this->rightImage);
}
void carousel::addImage(string imgPath) {
	this->imgList.push_back(imgPath);
}
void carousel::start()
{
	this->itImgList = this->imgList.begin();
	--this->itImgList;
	this->nextImage();
}
