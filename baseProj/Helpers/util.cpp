#include <array>
#include <vector>
#include "util.hpp"

int util::currentMode;
int util::medianWindowSize = 3;
int util::threshold = 128;
array<int,255> util::levels;

void util::CreateHistogram(ImageClass* image, ImageClass* resultImage)
{
	int totalPixels = image->SizeX() * image->SizeY();
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
			if(levels[i] > levels[maxLevel])
				maxLevel = i;
		}
	}
	cout << "all measured." << endl;
	int maxPixel = image->SizeY();
	int xAxisPixel = 0;
	int lineWidth = image->SizeX()/255;
	cout << "Level\tCount" << '\n';
	for (int i = 0; i < 255; i++) {
		int lineEnd = 0;
		cout << i<<"\t"<<levels[i] << '\n';
		if(levels[i]>0){
			lineEnd = (levels[i]/levels[maxLevel])*maxPixel;
		}
		if(util::currentMode != CGM_THRESHOLD_SEG)
			for (int j = 0; j < lineWidth; j++) {
				resultImage->DrawLineV(xAxisPixel+j,0,lineEnd, 255,0,0);
			}
		xAxisPixel+=lineWidth;
	}
	std::cout << "levels["<<maxLevel<<"]=" << levels[maxLevel] << " (max)\n";
	std::cout << "lineWidth=" << lineWidth << '\n';
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
	int pixelsChanged = 0;
	for(x = edgex; x<=(resultImage->SizeX() - edgex); x++){
		for(y = edgey; y<=(resultImage->SizeY() - edgey); y++){
			int colorArray[wWidth][wHeight];
			for(int fx=0; fx < wWidth; fx++)
				for(int fy=0; fy < wHeight; fy++)
					colorArray[fx][fy] = resultImage->GetPointIntensity(x + fx - edgex, y + fy - edgey);
			qsort((void*)colorArray, (wWidth*wHeight), sizeof(int),(int (*)(const void*,const void*))util::iGtJ);
			int medianIntensity = colorArray[wWidth/2][wHeight/2];
			if(medianIntensity != resultImage->GetPointIntensity(x,y)){
				resultImage->DrawPixel(x,y ,medianIntensity,medianIntensity,medianIntensity);
				pixelsChanged++;
			}
		}
	}
	std::cout << "ended MedianFilter" << '\n';
	std::cout << "pixels changed: " << pixelsChanged << '\n';
}

void util::MedianSegmentation(ImageClass* image, ImageClass* resultImage) {
	int wWidth, wHeight, x, y;
	wWidth = wHeight = util::medianWindowSize;
	int edgex = (wWidth / 2);
	int edgey = (wHeight / 2);

	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	image->CopyTo(resultImage);

	std::cout << "starting MedianSegmentation..." << '\n';
	std::cout << "array of "<<wWidth<<"x"<<wHeight<< " in an image of "<<resultImage->SizeX()<<"x"<<resultImage->SizeY()<<"\n";
	int pixelsChanged = 0;
	for(x = edgex; x<=(resultImage->SizeX() - edgex); x=x+wWidth){
		for(y = edgey; y<=(resultImage->SizeY() - edgey); y=y+wHeight){
			// std::cout << "x="<<x<<",y="<<y << ",intensity:"<<resultImage->GetPointIntensity(x,y)<<"\n";
			int colorArray[wWidth][wHeight];
			for(int fx=0; fx < wWidth; fx++)
				for(int fy=0; fy < wHeight; fy++){
					int x2=x+fx-edgex;
					int y2=y+fy-edgey;
					colorArray[fx][fy] = resultImage->GetPointIntensity(x2,y2);
					// std::cout << "\tx2="<<x2<<",y2="<<y2 << '\n';
				}
			qsort((void*)colorArray, (wWidth*wHeight), sizeof(int),(int (*)(const void*,const void*))util::iGtJ);
			int medianIntensity = colorArray[wWidth/2][wHeight/2];
			std::cout << "medianIntensity="<<medianIntensity << '\n';
			for(int fx=0; fx < wWidth; fx++)
				for(int fy=0; fy < wHeight; fy++){
					int x2=x+fx-edgex;
					int y2=y+fy-edgey;
					// int x2=x;
					// int y2=y;
					// std::cout << "x2="<<x2<<",y2="<<y2 << '\n';
					if(x2>=0 && x2<resultImage->SizeX() && y2>=0 && y2<resultImage->SizeY()){
						// if(medianIntensity != resultImage->GetPointIntensity(x2, y2)){
							resultImage->DrawPixel(x2,y2 ,medianIntensity,medianIntensity,medianIntensity);
							pixelsChanged++;
						// }
					}
				}
			// return;
		}
	}
	std::cout << "ended MedianSegmentation" << '\n';
	std::cout << "pixels changed: " << pixelsChanged << '\n';
}

void util::ThresholdSegmentation(ImageClass* image, ImageClass* resultImage) {
	int  x, y;

	util::CreateHistogram(image, resultImage);

	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	image->CopyTo(resultImage);

	vector<int> existingLevels;
	int maxLevel = 0;
	for (int l = 0; l < levels.size(); l++)
		if(levels[l] > 0){
			existingLevels.push_back(l);
			if(levels[l] > levels[maxLevel])
				maxLevel = l;
		}
	threshold = existingLevels[existingLevels.size()/2];
	// threshold = existingLevels[maxLevel];

	std::cout << "starting ThresholdSegmentation..." << '\n';
	std::cout << "threshold:" << threshold << '\n';
	for(x = 0; x < resultImage->SizeX(); x++){
		for(y = 0; y < resultImage->SizeY(); y++){
			int pixelIntensity = resultImage->GetPointIntensity(x, y);
			if(pixelIntensity <= util::threshold)
				resultImage->DrawPixel(x,y ,255,255,255);
			else{
				if(pixelIntensity == maxLevel)
					resultImage->DrawPixel(x,y ,255,0,0);
				else
					resultImage->DrawPixel(x,y ,0,0,0);
			}
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
