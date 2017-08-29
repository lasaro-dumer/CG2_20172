
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef _MSC_VER

#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#include "ImageClass.hpp"


void ImageClass::SetColorMode()
{
	if (channels == 3)
		colorMode = GL_RGB;
	if (channels == 4)
		colorMode = GL_RGBA;

}
// **********************************************************************
//
//	Constructor
// **********************************************************************
ImageClass::ImageClass(int channels) {
	data = NULL;
	PosX = PosY = 0;
	zoomH = zoomV = 1;
	SetColorMode();
}

// **********************************************************************
//
//	Constructor
// **********************************************************************
ImageClass::ImageClass(int sizeX, int sizeY, int channels) {

	SetSize(sizeX,sizeY,channels);
	PosX = PosY = 0;
	zoomH = zoomV = 1;
	SetColorMode();

}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::SetSize(int sizeX, int sizeY, int channels) {
	unsigned int tam;

	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->channels = channels;

	tam = sizeof(unsigned char) * sizeX * sizeY * channels;
	data = (unsigned char *) malloc (tam);
	memset(data,255,tam);

	PosX = PosY = 0;
	zoomH = zoomV = 1;

	SetColorMode();

}
// **********************************************************************
//
//
// **********************************************************************
unsigned char *ImageClass::GetImagePtr() {
	return data;
}
// **********************************************************************
//
//
// **********************************************************************
void ImageClass::SetPos(int X, int Y) {
	PosX = X;
	PosY = Y;
}


// **********************************************************************
//
//
// **********************************************************************
int ImageClass::Load(const char * nome) {
	if (data)
		free(data); // é necessário desalocar a área da imagem antiga.
	if (LoadImageFile(nome)) {
		FlipY();
		SetColorMode();
		return 1;
	}
	return 0;
}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Save(const char *nome) {
	// save_bmp(nome, data, sizeX, sizeY);
	FlipY();
	SOIL_save_image	(nome, SOIL_SAVE_TYPE_BMP,
		SizeX(), SizeY(), Channels(), GetImagePtr());
    FlipY();
}
/*
void ImageClass::Save(const char *nome) {
    unsigned int x, y, w = SizeX(), h = SizeY();
    FILE *f;
    unsigned char *img = NULL;
    img = (unsigned char *)malloc(3*w*h);

    int filesize = 54 + 3*w*h;  //w is your image width, h is image height, both int

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen(nome,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            x=i; y=(h-1)-j;
            img[(x+y*w)*3+2] = data[(x+y*w)*3+0];
            img[(x+y*w)*3+1] = data[(x+y*w)*3+1];
            img[(x+y*w)*3+0] = data[(x+y*w)*3+2];
        }
    }

    for(int i=h-1; i>-1; i--)
    {
        fwrite(img+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }
    fclose(f);
}
*/
// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Display() {
// NOVO
// Isto resolve o problema de ter a imagem com
// largura múltipla de 4
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelZoom(zoomH , zoomV);
	glRasterPos2f(PosX, PosY);
	glDrawPixels(sizeX, sizeY, colorMode, GL_UNSIGNED_BYTE, data);
}


// **********************************************************************
//
//
// **********************************************************************
void ImageClass::Delete() {
	// Cleanup
	if (data) {
		free(data);
		data = NULL;
	}
}
// **********************************************************************
//
//
// **********************************************************************
void ImageClass::DrawPixel(GLint x, GLint y, unsigned char r, unsigned char g, unsigned char b) {
	unsigned long addr;

	addr = (unsigned long)( y *(sizeX)* channels + x * channels );
	data[addr++] = r;
	data[addr++] = g;
	data[addr] = b;
}
// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::DrawLineH(int y, int x1, int x2,unsigned char r, unsigned char g, unsigned char b ) {
	int x;
	if (x1 <= x2)
		for (x = x1; x<=x2; x++) {
			DrawPixel(x,y,r,g,b);
		}
	else
		for (x = x2; x<=x1; x++) {
			DrawPixel(x,y,r,g,b);
		}

}
// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::DrawLineV(int x, int y1, int y2,unsigned char r, unsigned char g, unsigned char b ) {
	int y;
	for (y = y1; y<=y2; y++) {
		DrawPixel(x,y,r,g,b);
	}
}

// **********************************************************************
//
//
// **********************************************************************
void ImageClass::ReadPixel(GLint x, GLint y, unsigned char &r, unsigned char &g, unsigned char &b) {
	unsigned long addr;

	addr = (unsigned long)( y *(sizeX)* channels + x * channels );
	r = data[addr++];
	g = data[addr++];
	b = data[addr];
}


// **********************************************************************
//
//
//
// **********************************************************************
double ImageClass::GetPointIntensity(int x, int y) {
	unsigned char r,g,b;
	double i;

	ReadPixel(x,y,r,g,b);
	i = (0.3 * r + 0.59 * g + 0.11 * b);
	return i;
}

// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::CopyTo(ImageClass *i) {

	unsigned int tam;

	tam = sizeof(unsigned char) * sizeX * sizeY * channels;
	memcpy(i->data, data,tam);

}

// **********************************************************************
//
//
//
// **********************************************************************
void ImageClass::Clear() {
	unsigned int tam;
	tam = sizeof(unsigned char) * SizeX() * SizeY() * channels;
	memset(data,255,tam);

}

// **************************************************************
//
// **************************************************************
void ImageClass::DrawBox(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b ) {
	DrawLineH(y1, x1, x2, r,g,b);
	DrawLineH(y2, x1, x2, r,g,b);
	DrawLineV(x1, y1, y2, r,g,b);
	DrawLineV(x2, y1, y2, r,g,b);
}
// **************************************************************
//
// **************************************************************
void ImageClass::FillBox(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b )
{

    for (int y = y1; y<=y2; y++)
        {
            DrawLineH(y, x1, x2, r,g,b);
        }


}

// **************************************************************
//
// **************************************************************
void ImageClass::DrawLine(int x0,int y0,int x1, int y1,unsigned char r, unsigned char g, unsigned char bb  ) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int x,y;
	float b, m;

	DrawPixel(x0, y0, r,g,bb);
	if (abs(dx) > abs(dy)) {          // slope < 1
		m = (float) dy / (float) dx;      // compute slope
		b = y0 - m*x0;
		dx = (dx < 0) ? -1 : 1;
		while (x0 != x1) {
			x0 += dx;
			y = (int)((m*x0 + b)+0.5);
			DrawPixel(x0, y, r,g,bb);
		}
	} else if (dy != 0) {                             // slope >= 1
		float m = (float) dx / (float) dy;      // compute slope
		float b = x0 - m*y0;
		dy = (dy < 0) ? -1 : 1;
		while (y0 != y1) {
			y0 += dy;
			x = (int) ((m*y0 + b)+0.5);
			DrawPixel(x, y0, r,g,bb);
		}
	}
}
