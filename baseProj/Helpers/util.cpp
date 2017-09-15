#include <array>
#include "util.hpp"

int util::currentMode;
int util::medianWindowSize = 3;
int util::threshold = 128;

void util::CreateHistogram(ImageClass* image, ImageClass* resultImage)
{
	int totalPixels = image->SizeX() * image->SizeY();
	array<int,255> levels;
	unsigned char r,g,b;
	int x,y;
	for (int i = 0; i < 255; i++) {
		levels[i]=0;
	}
	int maxLevel = 0;
	cout << "Measuring levels...";
	for(x=0; x<image->SizeX(); x++)
	{
		for(y=0; y<image->SizeY(); y++)
		{
			int i = image->GetPointIntensity(x,y);
			levels[i]++;
			if(levels[i]>maxLevel)
				maxLevel=levels[i];
		}
	}
	cout << "all measured." << endl;
	int maxPixel = image->SizeY();
	int xAxisPixel = 0;
	int lineWidth = image->SizeX()/255;
	std::cout << "maxLevel=" << maxLevel << '\n';
	std::cout << "lineWidth=" << lineWidth << '\n';
	cout << "Level\tCount" << '\n';
	for (int i = 0; i < 255; i++) {
		int lineEnd = 0;
		cout << i<<"\t"<<levels[i] << '\n';
		if(levels[i]>0){
			lineEnd = (levels[i]/maxLevel)*maxPixel;
		}
		for (int j = 0; j < lineWidth; j++) {
			resultImage->DrawLineV(xAxisPixel+j,0,lineEnd, 255,0,0);
		}
		xAxisPixel+=lineWidth;
	}
}

void util::MedianFilter(ImageClass* image, ImageClass* resultImage) {
	int wWidth, wHeight, x, y;
	wWidth = wHeight = util::medianWindowSize;
	int edgex = (wWidth / 2);
	int edgey = (wHeight / 2);

	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	image->CopyTo(resultImage);

	std::cout << "starting MedianFilter..." << '\n';
	std::cout << "array of "<<wWidth<<"x"<<wHeight<< '\n';
	int pixelsAmount = (resultImage->SizeX() - edgex) * (resultImage->SizeY() - edgey);
	int pixelsProcessed = 0;
	for(x = edgex; x<=(resultImage->SizeX() - edgex); x++){
		for(y = edgey; y<=(resultImage->SizeY() - edgey); y++){
			int colorArray[wWidth][wHeight];
			for(int fx=0; fx < wWidth; fx++)
				for(int fy=0; fy < wHeight; fy++)
					colorArray[fx][fy] = resultImage->GetPointIntensity(x + fx - edgex, y + fy - edgey);
			qsort((void*)colorArray, (wWidth*wHeight), sizeof(int),(int (*)(const void*,const void*))util::iGtJ);
			int medianIntensity = colorArray[wWidth/2][wHeight/2];
			resultImage->DrawPixel(x,y ,medianIntensity,medianIntensity,medianIntensity);
		}
		pixelsProcessed += y;
		// cout << pixelsProcessed << "/" << pixelsAmount << endl;
		// int p = (pixelsProcessed*100)/pixelsAmount;
		// if((p % 25)==0)
		// 	std::cout << p << "\%" << '\n';
	}
	std::cout << "ended MedianFilter" << '\n';
}

void util::ThresholdSegmentation(ImageClass* image, ImageClass* resultImage) {
	int  x, y;
	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	image->CopyTo(resultImage);

	std::cout << "starting ThresholdSegmentation..." << '\n';
	for(x = 0; x < resultImage->SizeX(); x++){
		for(y = 0; y < resultImage->SizeY(); y++){
			if(resultImage->GetPointIntensity(x, y) <= util::threshold)
				resultImage->DrawPixel(x,y ,255,255,255);
			else
				resultImage->DrawPixel(x,y ,0,0,0);
		}
	}
	std::cout << "ended ThresholdSegmentation" << '\n';
}

void util::LoadNewImage(string path, ImageClass* image, ImageClass* resultImage){
	int r;
	cout << "Image to load: '" << path << "'" << endl;

	ImageClass tempImage;
	r = tempImage.Load(path.c_str());

	if (!r) exit(1);
	else cout << ("Image loaded!\n");

	image->SetSize(tempImage.SizeX(), tempImage.SizeY(), tempImage.Channels());
	tempImage.CopyTo(image);

	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	cout << "New image created" << endl;
}

void util::CopyResultToMain(ImageClass* image, ImageClass* resultImage){
	resultImage->CopyTo(image);
	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	cout << "Copied result to source" << endl;
}

void util::SwitchMode(int newMode) {
	currentMode = newMode;
}

void util::IncValue() {
	switch (util::currentMode) {
		case CGM_MEDIAN_SIZE:
			util::medianWindowSize++;
			std::cout << "medianWindowSize: " << util::medianWindowSize << '\n';
			break;
		case CGM_THRESHOLD_SEG:
			util::threshold++;
			std::cout << "threshold: " << util::threshold << '\n';
			break;
		default:
			break;
	}
}

void util::DecValue() {
	switch (util::currentMode) {
		case CGM_MEDIAN_SIZE:
			util::medianWindowSize--;
			std::cout << "medianWindowSize: " << util::medianWindowSize << '\n';
			break;
		case CGM_THRESHOLD_SEG:
			util::threshold--;
			std::cout << "threshold: " << util::threshold << '\n';
			break;
		default:
			break;
	}
}
