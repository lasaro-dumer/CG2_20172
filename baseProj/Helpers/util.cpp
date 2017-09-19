#include <array>
#include <vector>
#include <math.h>
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

void util::SobelFilter(ImageClass* image, ImageClass* resultImage) {
	// MAKE THE THINGS
	int wWidth, wHeight, x, y;
	wWidth = wHeight = util::medianWindowSize;
	int edgex = (wWidth / 2);
	int edgey = (wHeight / 2);

	// DEFINE SOBEL OPERATOR
	// double x_op[3][3] =
	// 	{
	// 		{-1, 0, 1},
	// 		{-2, 0, 2},
	// 		{-1, 0, 1}
	// 	};
	// double y_op[3][3] =
	// 	{
	// 		{1, 2, 1},
	// 		{0, 0, 0},
	// 		{-1, -2, -1}
	// 	};
	double x_op[3][3] =
		{
			{3, 0, -3},
			{10, 0, -10},
			{3, 0, -3}
		};
	double y_op[3][3] =
		{
			{3, 10, 3},
			{0, 0, 0},
			{-3, -10, -3}
		};

	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	image->CopyTo(resultImage);

	std::cout << "starting SobelFilter..." << '\n';
	std::cout << "array of "<<wWidth<<"x"<<wHeight<< " in an image of "<<resultImage->SizeX()<<"x"<<resultImage->SizeY()<<"\n";

	// CALCULATE CONVOLUTION?
	for(x = edgex; x<=(resultImage->SizeX() - edgex); x++){
		for(y = edgey; y<=(resultImage->SizeY() - edgey); y++){

			short x_weight=0;
    	short y_weight=0;

			for(int fx=0; fx < wWidth; fx++){
				for(int fy=0; fy < wHeight; fy++){
					int x2=x+fx-edgex;
					int y2=y+fy-edgey;
					x_weight += image->GetPointIntensity(x2,y2)*x_op[fx][fy];
					y_weight += image->GetPointIntensity(x2,y2)*y_op[fx][fy];
				}
			}

			short val=sqrt((x_weight^2)+(y_weight^2));
			// This value means something, but I haven't figured out what yet. But it changes the lines.
			if(val>5)
        val=0;
    	else if(val<5)
        val=255;

			// image->ReadPixel(x,y,r,g,b);
			// resultImage->DrawPixel(x,y,0.3*val*r,0.59*val*g,0.11*val*b);
			resultImage->DrawPixel(x,y,val,val,val);
		}
	}

	std::cout << "Finished SobelFilter" << '\n';
}

void util::BinaryDilation(ImageClass* image, ImageClass* resultImage) {
	// MAKE THE THINGS
	int wWidth, wHeight, x, y;
	wWidth = wHeight = util::medianWindowSize;
	int edgex = (wWidth / 2);
	int edgey = (wHeight / 2);

	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	image->CopyTo(resultImage);

	double op[3][3] =
		{
			{1, 1, 1},
			{1, 1, 1},
			{1, 1, 1}
		};

	std::cout << "starting BinaryDilation..." << '\n';
	std::cout << "array of "<<wWidth<<"x"<<wHeight<< " in an image of "<<resultImage->SizeX()<<"x"<<resultImage->SizeY()<<"\n";

	// CALCULATE CONVOLUTION?
	for(x = edgex; x<=(resultImage->SizeX() - edgex); x++){
		for(y = edgey; y<=(resultImage->SizeY() - edgey); y++){

			short weight=0;
			short val=255;

			for(int fx=0; fx < wWidth; fx++){
				for(int fy=0; fy < wHeight; fy++){
					int x2=x+fx-edgex;
					int y2=y+fy-edgey;
					weight = image->GetPointIntensity(x2,y2)*op[fx][fy];
					if(val>weight)
						val = weight;
				}
			}

			// image->ReadPixel(x,y,r,g,b);
			// resultImage->DrawPixel(x,y,0.3*val*r,0.59*val*g,0.11*val*b);
			resultImage->DrawPixel(x,y,val,val,val);
		}
	}

	std::cout << "Finished BinaryDilation" << '\n';
}

void util::HighPassFilter(ImageClass* image, ImageClass* resultImage) {
	// MAKE THE THINGS
	int wWidth, wHeight, x, y;
	wWidth = wHeight = util::medianWindowSize;
	int edgex = (wWidth / 2);
	int edgey = (wHeight / 2);

	double op[3][3] =
		{
			{-1, -1, -1},
			{-1, 9, -1},
			{-1, -1, -1}
		};

	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	image->CopyTo(resultImage);

	std::cout << "starting HighPassFilter..." << '\n';
	std::cout << "array of "<<wWidth<<"x"<<wHeight<< " in an image of "<<resultImage->SizeX()<<"x"<<resultImage->SizeY()<<"\n";

	// CALCULATE CONVOLUTION?
	for(x = edgex; x<=(resultImage->SizeX() - edgex); x++){
		for(y = edgey; y<=(resultImage->SizeY() - edgey); y++){

			short weight=0;

			for(int fx=0; fx < wWidth; fx++){
				for(int fy=0; fy < wHeight; fy++){
					int x2=x+fx-edgex;
					int y2=y+fy-edgey;
					weight += image->GetPointIntensity(x2,y2)*op[fx][fy];
				}
			}

			// THE WEIGHT HERE IS THE THRESHOLD OF PASSAGE AND INDICATES HOW WELL THE LINES ADEQUATE
			if(weight>200)
        weight=255;
    	else if(weight<200)
        weight=0;

			// image->ReadPixel(x,y,r,g,b);
			// resultImage->DrawPixel(x,y,0.3*val*r,0.59*val*g,0.11*val*b);
			resultImage->DrawPixel(x,y,weight,weight,weight);
		}
	}

	std::cout << "Finished HighPassFilter" << '\n';
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
