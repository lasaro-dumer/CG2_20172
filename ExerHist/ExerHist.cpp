
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include "SOIL/SOIL.h"
#include "Helpers/ImageClass.hpp"
#include "Helpers/util.hpp"
#include "Helpers/carousel.hpp"

using namespace std;
ImageClass Imagem;
ImageClass NewImage;
carousel* imgCarousel;
const int LIMIAR = 120;
#define LARGURA_JAN 1000
#define ALTURA_JAN 500

// **********************************************************************
//  void init(void)
// **********************************************************************
void init()
{
	util::medianWindowSize = 3;
	imgCarousel = new carousel(&Imagem, &NewImage);
	imgCarousel->addImage("imgs/0648.png");
	imgCarousel->addImage("imgs/0649.png");
	imgCarousel->addImage("imgs/0695.png");
	imgCarousel->addImage("imgs/Falcao.jpg");
	imgCarousel->addImage("imgs/Ruido1.bmp");
	imgCarousel->addImage("imgs/Ruido2.bmp");
	imgCarousel->addImage("imgs/Ruido3.bmp");
	imgCarousel->start();
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
	float zoomH = (glutGet(GLUT_WINDOW_WIDTH)/2.0)/(double)Imagem.SizeX();
	float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)Imagem.SizeY();
	Imagem.SetZoomH(zoomH);
	Imagem.SetZoomV(zoomV);
	//upper left
	Imagem.SetPos(0, 0);
	//middle
	NewImage.SetPos(glutGet(GLUT_WINDOW_WIDTH)/2, 0);

// Ajusta o ZOOM da imagem para que apareca na metade da janela
	NewImage.SetZoomH(zoomH);
	NewImage.SetZoomV(zoomV);

// Coloca as imagens na tela
	Imagem.Display();
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
			Imagem.Delete();
			exit ( 0 );   // a tecla ESC for pressionada
			break;
		case '1':
			util::CreateHistogram(&Imagem, &NewImage);
			glutPostRedisplay();	// obrigatório para redesenhar a tela
			break;
		case '2':
			util::MedianFilter(&Imagem, &NewImage, util::medianWindowSize);
			glutPostRedisplay();	// obrigatório para redesenhar a tela
			break;
		case 'c':
			util::CopyResultToMain(&Imagem, &NewImage);
			glutPostRedisplay();	// obrigatório para redesenhar a tela
			break;
		case ',':
			imgCarousel->previousImage();
			glutPostRedisplay();
			break;
		case '.':
			imgCarousel->nextImage();
			glutPostRedisplay();
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
		util::medianWindowSize++;
		std::cout << "medianWindowSize:"<<util::medianWindowSize << '\n';
		break;
	case GLUT_KEY_DOWN:	 // When Down Arrow Is Pressed...
		util::medianWindowSize--;
		std::cout << "medianWindowSize:"<<util::medianWindowSize << '\n';
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
