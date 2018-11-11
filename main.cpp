
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "imageloader.h"


using namespace std;

void handleKeypress(unsigned char, int, int);
void handleResize(int, int);
GLuint loadTexture(Image*);
void initRendering();

void drawScene();
void drawBackground();
void drawBaby();

GLuint textureBg;
GLuint textureBaby;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 500);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("teletubbies baby");
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}

void initRendering() {
	GLfloat specular [] = {0.9,0.8,0.1,1.0};
	GLfloat shininess[] = { 10.0 };
	GLfloat position[] = { 0.0,0.0,0.0,1.0};
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	Image* image = loadBMP("back.bmp");
	textureBg = loadTexture(image);
	delete image;

	image = loadBMP("baby.bmp");
	textureBaby = loadTexture(image);
	delete image;
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-10, -3, 50.0, 0, 0, 0, 0.0, 1.0, 0.0);

	glEnable(GL_TEXTURE_2D);

	drawBackground();
	drawBaby();

	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}


void drawBackground() {

	glBindTexture(GL_TEXTURE_2D, textureBg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(-75, -40, -16);

	glTexCoord2f(0, 1);
	glVertex3f(-75, 40, -16);

	glTexCoord2f(1, 1);
	glVertex3f(75, 40, -16);

	glTexCoord2f(1, 0);
	glVertex3f(75, -40, -16);

	glNormal3f(0, 0, 1);

	glEnd();



}
float R = 0.0;
float angle;


void drawBaby() {

	glBindTexture(GL_TEXTURE_2D, textureBaby);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	double y1s = 14.0;
	double x1s = 16.0;
	double radius = 6.5;
	
	
	float radian, x, y , xcos , ysin , tx , ty;
	glTranslatef(-R, 0, 0);

	glBegin(GL_POLYGON);
	
	for (angle = 0.0; angle < 360.0; angle += 2.0)
	{
		radian = angle * (3.14 / 180.0f);
		xcos = (float)cos(radian);
		ysin = (float)sin(radian);
		x = xcos * radius + 16.0 ;
		y = ysin * radius + 14.0 ;
		tx = xcos * 0.5 + 0.8;
		ty = ysin * 0.5 + 0.5;

		glTexCoord2f(tx, ty);
		glVertex2f(x, y);
		glNormal3f(-1, 0, 0);

	}
	glEnd();
	R += 0.01;

	glutPostRedisplay();              //Tell GLUT that the display has changed
	glutTimerFunc(25, 0, 0);          //Tell GLUT to call update again in 25 milliseconds


	
}


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}
