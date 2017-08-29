
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include "SOIL/SOIL.h"
#include "ImageClass.hpp"

using namespace std;
ImageClass Image;
ImageClass NewImage;
std::vector<string> imgList;
std::vector<string>::iterator itImgList;
bool firstImage = true;
const int LIMIAR = 120;
#define LARGURA_JAN 1000
#define ALTURA_JAN 500
int medianWindowSize = 5;
int myfunction (int* i,int* j) { return (*i>*j); }
void CreateHistogram()
{
	int totalPixels = Image.SizeX() * Image.SizeY();
	array<int,255> levels;
	unsigned char r,g,b;
	int x,y;
	for (int i = 0; i < 255; i++) {
		levels[i]=0;
	}
	int maxLevel = 0;
	cout << "Measuring levels...";
	for(x=0; x<Image.SizeX(); x++)
	{
		for(y=0; y<Image.SizeY(); y++)
		{
			int i = Image.GetPointIntensity(x,y);
			levels[i]++;
			if(levels[i]>maxLevel)
				maxLevel=levels[i];
		}
	}
	cout << "all measured." << endl;
	int maxPixel = Image.SizeY();
	int xAxisPixel = 0;
	int lineWidth = Image.SizeX()/255;
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
			NewImage.DrawLineV(xAxisPixel+j,0,lineEnd, 255,0,0);
		}
		xAxisPixel+=lineWidth;
	}
}

void MedianFilter(ImageClass* image, ImageClass* resultImage, int windowSize) {
	int wWidth, wHeight, x, y;
	wWidth = wHeight = windowSize;
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
			qsort((void*)colorArray, (wWidth*wHeight), sizeof(int),(int (*)(const void*,const void*))myfunction);
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

void loadNewImage(string path, ImageClass* image, ImageClass* resultImage){
	int r;
	cout << "imagem a ser carregada: '" << path << "'" << endl;
	// image->Clear();
	// r = image->Load(path.c_str()); // Carrega uma imagem
	ImageClass tempImage;
	r = tempImage.Load(path.c_str()); // Carrega uma imagem

	if (!r) exit(1); // Erro na carga da imagem
	else cout << ("Imagem carregada!\n");

	image->SetSize(tempImage.SizeX(), tempImage.SizeY(), tempImage.Channels());
	tempImage.CopyTo(image);

	// resultImage->Clear();
	// Ajusta o tamnho da imagem da direita, para que ela
	// passe a ter o mesmo tamnho da imagem recem carregada
	// Caso precise alterar o tamanho da nova imagem, mude os parâmetros
	// da na chamada abaixo
	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	cout << "Nova Imagem Criada" << endl;
}

void CopyResultToMain(ImageClass* image, ImageClass* resultImage){
	// image->SetSize(tempImage.SizeX(), tempImage.SizeY(), tempImage.Channels());
	resultImage->CopyTo(image);

	// resultImage->Clear();
	// Ajusta o tamnho da imagem da direita, para que ela
	// passe a ter o mesmo tamnho da imagem recem carregada
	// Caso precise alterar o tamanho da nova imagem, mude os parâmetros
	// da na chamada abaixo
	resultImage->SetSize(image->SizeX(), image->SizeY(), image->Channels());
	cout << "Nova Imagem Criada" << endl;
}

void nextImage() {
	if(firstImage)
		firstImage = false;
	else
		++itImgList;
	if(itImgList == imgList.end())
		itImgList = imgList.begin();
	loadNewImage(*itImgList, &Image, &NewImage);
}
// **********************************************************************
//  void init(void)
// **********************************************************************
void init()
{
	imgList.push_back("imgs/0648.png");
	imgList.push_back("imgs/0649.png");
	imgList.push_back("imgs/0695.png");
	imgList.push_back("imgs/Falcao.jpg");
	imgList.push_back("imgs/Ruido1.bmp");
	imgList.push_back("imgs/Ruido2.bmp");
	imgList.push_back("imgs/Ruido3.bmp");
	itImgList = imgList.begin();
	nextImage();
}
// **********************************************************************
//  void reshape( int w, int h )
//  OpenGL window resizing
// **********************************************************************
void reshape( int w, int h )
{

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	gluOrtho2D(0,w,0,h);
	// Set the clipping volume
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela preto
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	// Ajusta o ZOOM da imagem para que apareca na metade da janela
	float zoomH = (glutGet(GLUT_WINDOW_WIDTH)/2.0)/(double)Image.SizeX();
	float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Image.SizeY();
	Image.SetZoomH(zoomH);
	Image.SetZoomV(zoomV);
	//upper left
	Image.SetPos(0, 0);
	//middle
	NewImage.SetPos(glutGet(GLUT_WINDOW_WIDTH)/2, 0);

// Ajusta o ZOOM da imagem para que apareca na metade da janela
	NewImage.SetZoomH(zoomH);
	NewImage.SetZoomV(zoomV);

// Coloca as imagens na tela
	Image.Display();
	NewImage.Display();

// Mostra a tela OpenGL
	glutSwapBuffers();
}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27: // Termina o programa qdo
			NewImage.Delete();
			Image.Delete();
			exit ( 0 );   // a tecla ESC for pressionada
			break;
		case '1':
			CreateHistogram();
			glutPostRedisplay();	// obrigatório para redesenhar a tela
			break;
		case '2':
			MedianFilter(&Image, &NewImage, medianWindowSize);
			glutPostRedisplay();	// obrigatório para redesenhar a tela
			break;
		case 'c':
			CopyResultToMain(&Image, &NewImage);
			glutPostRedisplay();	// obrigatório para redesenhar a tela
			break;
		case 'x':
			nextImage();
			glutPostRedisplay();	// obrigatório para redesenhar a tela
			break;
		default:
			std::cout << "key:"<<key << '\n';
			break;
	}
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
	case GLUT_KEY_UP:	   // When Up Arrow Is Pressed...
		medianWindowSize++;
		std::cout << "medianWindowSize:"<<medianWindowSize << '\n';
		break;
	case GLUT_KEY_DOWN:	 // When Down Arrow Is Pressed...
		medianWindowSize--;
		std::cout << "medianWindowSize:"<<medianWindowSize << '\n';
		break;
	default:
		break;
	}
}
// **********************************************************************
//  void main ( int argc, char** argv )
// **********************************************************************

int main ( int argc, char** argv )
{
	glutInit			( &argc, argv );

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (10,10);

	// Define o tamanho da janela gráfica do programa
	glutInitWindowSize  ( LARGURA_JAN, ALTURA_JAN);
	glutCreateWindow	( "Image Loader" );

	init ();

	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	//glutIdleFunc ( display );

	glutMainLoop ( );
	return 0;
}
