#ifndef ImageClassH__
#define ImageClassH__
// *****************************************************************************************
//  ImageClass.hpp
// *****************************************************************************************

#include "Image.hpp"

class ImageClass: public Image {

private:
	void SetColorMode();
protected:
	int PosX, PosY;
	float zoomH, zoomV;
	GLenum colorMode;

public:

	double Tamanho;
	ImageClass(int channels=3);
	ImageClass(int sizeX, int sizeY, int channels=3);

	int Load(const char *);
	void Save(const char *);
	void Display(void);
	void Delete(void);
	void DrawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	void DrawLineH(int y, int x1, int x2, unsigned char r, unsigned char g, unsigned char b);
	void DrawLineV(int x, int y1, int y2,unsigned char r, unsigned char g, unsigned char b );
	void ReadPixel(GLint x, GLint y, unsigned char &r, unsigned char &g, unsigned char &b);
	double GetPointIntensity(int x, int y);
	float GetZoomH() { return zoomH; };
	float GetZoomV() { return zoomV; };
	void SetZoomH(float H) { zoomH = H; };
	void SetZoomV(float V) { zoomV = V; };
	void CopyTo(ImageClass *i);
	void Clear();

	unsigned char *GetImagePtr();
	void SetSize(int sizeX, int sizeY, int channels=3);

	void SetPos(int X, int Y);

	void DrawBox(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b);
	void DrawLine(int x0,int y0,int x1, int y1,unsigned char r, unsigned char g, unsigned char b  );
	void FillBox(int x1,int y1,int x2,int y2,unsigned char r, unsigned char g, unsigned char b );
};
#endif /*ImageClassH__*/
