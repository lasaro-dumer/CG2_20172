#include "Image.hpp"

bool Image::LoadImageFile(const char *name) {
	data = SOIL_load_image( name, &sizeX, &sizeY, &channels, 0);
//	    TEX *temp;
//		temp = CarregaJPG(name, true);
//		data = temp->data;
//		sizeX = temp->dimx;
//		sizeY = temp->dimy;
//		channels = temp->ncomp;

	if (data == NULL) {
		cout << "Error loading image " << name << "." << endl;
		return false;
	}
	cout << "Image " << name << " loaded !"<< endl;
	cout << "Canais:" << channels << endl;

	return true;
}

void Image::FlipY() {

	unsigned long InicioLinhaA, InicioLinhaB, TamLinha;
	unsigned char  *ImgLineTemp=NULL;

	ImgLineTemp = (unsigned char *) malloc(sizeof(unsigned char)* sizeX * channels);

	TamLinha = sizeX * channels;
	InicioLinhaA = 0;
	InicioLinhaB = (sizeY-1) * TamLinha;

	for( unsigned int line = 0; line < sizeY/2; line++ ) {
		memcpy(ImgLineTemp, &data[InicioLinhaA],TamLinha);
		memcpy(&data[InicioLinhaA], &data[InicioLinhaB], TamLinha);
		memcpy(&data[InicioLinhaB], ImgLineTemp, TamLinha);
		InicioLinhaA += TamLinha;
		InicioLinhaB -= TamLinha;
	}
	free (ImgLineTemp);
}
