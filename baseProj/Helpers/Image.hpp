#ifndef ImageH__
#define ImageH__
// *****************************************************************************************
//  Image.hpp
// *****************************************************************************************

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

#ifdef _MSC_VER

#endif

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
#endif

#ifdef __APPLE__
/* Para usar o include da GLUT desta forma, adicione as
seguintes clausulas nas configurac�es do Linker:
-framework CoreFoundation
-framework GLUT
-framework OpenGL
*/
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "../SOIL/SOIL.h"

class Image {

protected:
	unsigned char *data;
	int sizeX, sizeY, channels;

	bool LoadImageFile(const char *name);
	void FlipY();

	Image(int channels=3) {
		data = NULL;
		sizeX = 0;
		sizeY = 0;
		this->channels = channels;

	}
public:
	int SizeX() {
		return sizeX;
	};
	int SizeY() {
		return sizeY;
	};
	int Channels() {
		return channels;
	}

};

#endif /*ImageH__*/
