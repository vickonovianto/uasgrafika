// RubiksEdwin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <windows.h>
#include <glut.h>
#include <cstring>

using namespace std;

#include "imageloader.h"

#include "cubeops.h"

#define KPI 3.1415926

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define MOUSE_SENSITIVITY 0.75
#define SHADOW_BLUR 10
#define SHADOW_FADE 0.99

#define BUFFER_LENGTH 64

GLfloat ambientlight[] = { .1,.1,.1,1 };
GLfloat diffuselight[] = { .5,.5,.5,1 };
GLfloat specularlight[] = { .5,.5,.5,1 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

GLuint white_textureId;

GLuint red_textureId;

GLuint blue_textureId;

GLuint green_textureId;

GLuint yellow_textureId;

GLuint orange_textureId;

GLuint _displayListId_smallcube; //The OpenGL id of the display list

								 //Makes the image into a texture, and returns the id of the texture

GLuint loadTexture(Image* image) {

	GLuint textureId;

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D,

		0,

		GL_RGB,

		image->width, image->height,

		0,

		GL_RGB,

		GL_UNSIGNED_BYTE,

		image->pixels);

	return textureId;
}


GLfloat cubeverts[] = {
	1,1,1,				// 0
	1,-1,1,				// 1
	-1,-1,1,			// 2
	-1,1,1,				// 3
	1,1,-1,				// 4
	1,-1,-1,			// 5
	-1,-1,-1,			// 6
	-1,1,-1				// 7
};

GLubyte cube[] = {
	0,1,2,3,
	4,5,6,7,
	0,1,5,4,
	1,2,6,5,
	2,3,7,6,
	3,0,7,4
};

GLfloat rubikverts[] = {
	-.95,.95,.95,
	-.9,.9,1,
	.9,.9,1,
	.95,.95,.95,
	.9,1,.9,
	-.9,1,.9
};

GLfloat rubiknorms[] = {
	-.577,.577,.577,
	0,0,1,
	0,0,1,
	.577,.577,.577,
	0,1,0,
	0,1,0
};

GLubyte rubikmod[] = {
	0,1,2,3,
	0,3,4,5
};

GLfloat color[][3] = {
	{ 0,0,0 },
	{ 1,0,0 },
	{ 0,0,1 },
	{ 0,1,0 },
	{ 1,1,0 },
	{ 1,1,1 },
	{ 1,.7,0 }
};

static int facemap[9][12] = {
	{ 0,0,0, 1,1,1, 0,0,0, 0,0,0 },
	{ 0,0,0, 1,1,1, 0,0,0, 0,0,0 },
	{ 0,0,0, 1,1,1, 0,0,0, 0,0,0 },

	{ 4,4,4, 2,2,2, 5,5,5, 6,6,6 },
	{ 4,4,4, 2,2,2, 5,5,5, 6,6,6 },
	{ 4,4,4, 2,2,2, 5,5,5, 6,6,6 },

	{ 0,0,0, 3,3,3, 0,0,0, 0,0,0 },
	{ 0,0,0, 3,3,3, 0,0,0, 0,0,0 },
	{ 0,0,0, 3,3,3, 0,0,0, 0,0,0 }
};

int cubepos[27][3] = {
	{ -1, 1,-1 },{ 0, 1,-1 },{ 1, 1,-1 },
	{ -1, 0,-1 },{ 0, 0,-1 },{ 1, 0,-1 },
	{ -1,-1,-1 },{ 0,-1,-1 },{ 1,-1,-1 },

	{ -1, 1, 0 },{ 0, 1, 0 },{ 1, 1, 0 },
	{ -1, 0, 0 },{ 0, 0, 0 },{ 1, 0, 0 },
	{ -1,-1, 0 },{ 0,-1, 0 },{ 1,-1, 0 },

	{ -1, 1, 1 },{ 0, 1, 1 },{ 1, 1, 1 },
	{ -1, 0, 1 },{ 0, 0, 1 },{ 1, 0, 1 },
	{ -1,-1, 1 },{ 0,-1, 1 },{ 1,-1, 1 },
};

int cubeop[27];


static GLfloat theta[] = { 0,0 };  // X rotation, Y rotation
static GLfloat mouse[] = { -1,-1 };
static bool action[] = { false,false };
static int actioncube = -1;
static int actionaxis[] = { -1,-1 };
static int actionplane = 0;
static float actionangle[] = { 0,0 };

void RenderScene(void);
void ChangeSize(GLsizei, GLsizei);
void IdleCallback();
void MouseCallback(int, int, int, int);
void MouseMotionCallback(int, int);
void SelectCube(int, int);

void DrawMiniCube(int, int);
void DrawRubikMod();

int CubeFacing(float, float);

int main(int argc, char *argv[])
{
	memset(cubeop, 0, sizeof(int) * 27);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Rubik's Cube");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(IdleCallback);

	glutMouseFunc(MouseCallback);
	glutMotionFunc(MouseMotionCallback);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glutMainLoop();


}

void RenderScene(void)
{
	int i;
	float intensity;
	float scale = 1.0;

	// Initialize Selection Array
	glInitNames();
	glPushName(30);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularlight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularlight);
	glMateriali(GL_FRONT, GL_SHININESS, 100);

	// Try Shadowing
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, cubeverts);

	glLoadIdentity();

	glTranslatef(0.0, -5.0, -10.0);
	glScalef(2.5, 2.5, 2.5);
	glPushMatrix();


	for (i = 0; i<SHADOW_BLUR; i++)
	{
		intensity = 1.0 - (i + 1) / float(SHADOW_BLUR);
		scale *= SHADOW_FADE;

		glPopMatrix();
		glPushMatrix();
		glScalef(scale, 0.0, scale);
		glRotatef(theta[0], 1.0, 0.0, 0.0);
		glRotatef(theta[1], 0.0, 1.0, 0.0);

		glColor3f(intensity, intensity, intensity);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube);

	}
	glPopMatrix();

	// Draw Cube
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, rubikverts);
	glNormalPointer(GL_FLOAT, 0, rubiknorms);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glLoadIdentity();

	glTranslatef(0.0, 2.0, -10.0);
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);

	glColor3f(0.0, 0.0, 0.0);

	for (i = 0; i<27; i++)
	{
		glPushMatrix();

		glTranslatef(cubepos[i][0] * 2, cubepos[i][2] * 2, cubepos[i][1] * 2);

		glLoadName(i); // Selection buffer info
		if (!cubeop[i])
			DrawMiniCube(i, actioncube);

		glPopMatrix();
	}

	switch (actionaxis[actionplane])
	{
	case 0:
		glRotatef(actionangle[actionplane], 1.0, 0.0, 0.0);
		break;
	case 1:
		glRotatef(actionangle[actionplane], 0.0, 0.0, 1.0);
		break;
	case 2:
		glRotatef(actionangle[actionplane], 0.0, 1.0, 0.0);
		break;
	}

	for (i = 0; i<27; i++)
	{
		glPushMatrix();

		glTranslatef(cubepos[i][0] * 2, cubepos[i][2] * 2, cubepos[i][1] * 2);

		glLoadName(i); // Selection buffer info
		if (cubeop[i])
			DrawMiniCube(i, actioncube);

		glPopMatrix();
	}

	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90, 1, 1, 20);

	glMatrixMode(GL_MODELVIEW);
}

void IdleCallback()
{
	glutPostRedisplay();
}

void MouseCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			SelectCube(x, y);
			action[0] = true;
			actionangle[0] = 0;
			actionangle[1] = 0;
			actionplane = -1;
		}
		else
		{
			if (abs(actionangle[actionplane])>50)
				performop(facemap, cubepos[actioncube][0], cubepos[actioncube][1], cubepos[actioncube][2], CubeFacing(theta[0], theta[1]), actionaxis[actionplane], actionangle[actionplane]);

			action[0] = false;

			actioncube = -1;
			memset(cubeop, 0, sizeof(int) * 27);
		}
	}

	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
			action[1] = true;
		else
			action[1] = false;
	}

	mouse[0] = x;
	mouse[1] = y;

	glutPostRedisplay();
}

void MouseMotionCallback(int x, int y)
{
	int i, plane;
	float invert;

	if (mouse[0]<0)
		mouse[0] = x;
	if (mouse[1]<0)
		mouse[1] = y;

	if (action[0])
	{
		if (CubeFacing(theta[0], theta[1])<0)
			invert = -1;
		else
			invert = 1;

		actionangle[0] += (float(x) - mouse[0])*float(MOUSE_SENSITIVITY) / 1.75;
		actionangle[1] += (float(y) - mouse[1])*float(MOUSE_SENSITIVITY) / 1.75*invert;

		if (actionangle[0]>90)
			actionangle[0] = 90;
		if (actionangle[0]<-90)
			actionangle[0] = -90;

		if (actionangle[1]>90)
			actionangle[1] = 90;
		if (actionangle[1]<-90)
			actionangle[1] = -90;

		plane = CubeFacing(theta[0], theta[1]);
		if (abs(plane) - 1<0)
			return;

		switch (abs(plane))
		{
		case 1:
			actionaxis[0] = 2;
			actionaxis[1] = 1;
			break;
		case 2:
			actionaxis[0] = 1;
			actionaxis[1] = 0;
			break;
		case 3:
			actionaxis[0] = 2;
			actionaxis[1] = 0;
			break;
		case 4:
			actionaxis[0] = 0;
			actionaxis[1] = 1;
			break;
		}

		plane = actionplane;
		if (abs(actionangle[0])>abs(actionangle[1]))
			actionplane = 0;
		else
			actionplane = 1;

		if (plane != actionplane)
		{
			cout << CubeFacing(theta[0], theta[1]) << ":" << theta[0] << " " << theta[1] << endl;
			for (i = 0; i<27; i++)
			{
				if (cubepos[actioncube][actionaxis[actionplane]] == cubepos[i][actionaxis[actionplane]])
					cubeop[i] = 1;
				else
					cubeop[i] = 0;
			}
		}
	}
	else if (action[1])
	{
		theta[0] += (y - mouse[1])*MOUSE_SENSITIVITY;
		theta[1] += (x - mouse[0])*MOUSE_SENSITIVITY;

		if (theta[0]>360)
			theta[0] -= 360;
		if (theta[0]<0)
			theta[0] += 360;

		if (theta[1]>360)
			theta[1] -= 360;
		if (theta[1]<0)
			theta[1] += 360;
	}

	mouse[0] = x;
	mouse[1] = y;
}

void SelectCube(int mousex, int mousey)
{
	int n;
	GLuint minz;
	GLuint selectBuff[BUFFER_LENGTH];
	GLuint hits;
	int *viewport;

	viewport = new int[4];

	glSelectBuffer(BUFFER_LENGTH, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Change Viewport properties to Selection Mode
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glRenderMode(GL_SELECT);

	glLoadIdentity();
	gluPickMatrix(mousex, viewport[3] - mousey, 2, 2, viewport);
	gluPerspective(90.0f, 1, 1, 20);

	glMatrixMode(GL_MODELVIEW);

	// Draw everything with the selection mode enabled
	RenderScene();

	// Find out what the user clicked on
	hits = glRenderMode(GL_RENDER);

	if (hits>0)
	{
		n = 0;
		minz = selectBuff[n + 1];
		actioncube = selectBuff[n + 3];
		while (selectBuff[n] == 1)
		{
			if (minz>selectBuff[n + 1])
			{
				minz = selectBuff[n + 1];
				actioncube = selectBuff[n + 3];
			}

			n += 4;
		}
	}

	// Reset viewing settings
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	delete viewport;
}

void DrawMiniCube(int cubeID, int selected)
{
	int colorID;

	if (actioncube == cubeID)
	{
		glColor3f(0, 0, 0);
		//		glDisable(GL_LIGHTING);
	}
	else
		glColor3f(0, 0, 0);

	DrawRubikMod();

	Image* image3 = loadBMP("lena.bmp");

	orange_textureId = loadTexture(image3);

	delete image3;

	glEnable(GL_LIGHTING);

	// "Front" Face
	glPushMatrix();
	glTranslatef(0, 0, 1);
	glColor3f(0, 0, 0);

	if (cubepos[cubeID][1] == 1)
	{
		colorID = facemap[4 - cubepos[cubeID][2]][4 + cubepos[cubeID][0]];
		glColor3f(color[colorID][0],color[colorID][1],color[colorID][2]);
	}

	
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);

	glVertex3f(.9, .9, 0);

	glVertex3f(.9, -.9, 0);

	glVertex3f(-.9, -.9, 0);

	glVertex3f(-.9, .9, 0);
	glEnd();


	glPopMatrix();

	


	// "Back" Face
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 0, 1);
	glColor3f(0, 0, 0);

	if (cubepos[cubeID][1] == -1)
	{
		colorID = facemap[4 - cubepos[cubeID][2]][10 - cubepos[cubeID][0]];
		//glColor3f(color[colorID][0], color[colorID][1], color[colorID][2]);
	}

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, orange_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(.9, .9, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(.9, -.9, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-.9, -.9, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-.9, .9, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	// "Right" Face
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, 1);
	glColor3f(0, 0, 0);

	if (cubepos[cubeID][0] == 1)
	{
		colorID = facemap[4 - cubepos[cubeID][2]][7 - cubepos[cubeID][1]];
		glColor3f(color[colorID][0], color[colorID][1], color[colorID][2]);
	}

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(.9, .9, 0);
	glVertex3f(.9, -.9, 0);
	glVertex3f(-.9, -.9, 0);
	glVertex3f(-.9, .9, 0);
	glEnd();

	glPopMatrix();

	// "Left" Face
	glPushMatrix();
	glRotatef(90, 0, -1, 0);
	glTranslatef(0, 0, 1);
	glColor3f(0, 0, 0);

	if (cubepos[cubeID][0] == -1)
	{
		colorID = facemap[4 - cubepos[cubeID][2]][1 + cubepos[cubeID][1]];
		glColor3f(color[colorID][0], color[colorID][1], color[colorID][2]);
	}

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(.9, .9, 0);
	glVertex3f(.9, -.9, 0);
	glVertex3f(-.9, -.9, 0);
	glVertex3f(-.9, .9, 0);
	glEnd();

	glPopMatrix();

	// "Top" Face
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	glTranslatef(0, 0, 1);
	glColor3f(0, 0, 0);

	if (cubepos[cubeID][2] == 1)
	{
		colorID = facemap[1 + cubepos[cubeID][1]][4 + cubepos[cubeID][0]];
		glColor3f(color[colorID][0], color[colorID][1], color[colorID][2]);
	}

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(.9, .9, 0);
	glVertex3f(.9, -.9, 0);
	glVertex3f(-.9, -.9, 0);
	glVertex3f(-.9, .9, 0);
	glEnd();

	glPopMatrix();

	// "Bottom" Face
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, 1);
	glColor3f(0, 0, 0);

	if (cubepos[cubeID][2] == -1)
	{
		colorID = facemap[7 - cubepos[cubeID][1]][4 + cubepos[cubeID][0]];
		glColor3f(color[colorID][0], color[colorID][1], color[colorID][2]);
	}

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(.9, .9, 0);
	glVertex3f(.9, -.9, 0);
	glVertex3f(-.9, -.9, 0);
	glVertex3f(-.9, .9, 0);
	glEnd();

	glPopMatrix();
}

void DrawRubikMod()
{
	glPushMatrix();

	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 0, 0, 1);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 0, 0, 1);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 0, 0, 1);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);

	glPopMatrix();
	glPushMatrix();

	glScalef(1, 1, -1);

	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 0, 0, 1);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 0, 0, 1);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 0, 0, 1);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);

	glPopMatrix();
	glPushMatrix();

	glRotatef(90, 0, 1, 0);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 1, 0, 0);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 1, 0, 0);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);
	glRotatef(90, 1, 0, 0);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, rubikmod);

	glPopMatrix();
}

int CubeFacing(float rx, float ry)
{
	float x, y, z;
	bool posx, negx, posy, negy, posz, negz;

	x = y = 0;
	z = 1.732;

	y = z*sin(rx*KPI / 180);
	x = z*sin(ry*KPI / 180);//*cos(rx*KPI/180);
	z = z*cos(ry*KPI / 180);

	if (z>1)
		posz = true;
	else
		posz = false;

	if (z<-1)
		negz = true;
	else
		negz = false;

	if (x>1)
		posx = true;
	else
		posx = false;

	if (x<-1)
		negx = true;
	else
		negx = false;

	if (y<-1.2)
		posy = true;
	else
		posy = false;

	if (y>1.2)
		negy = true;
	else
		negy = false;

	// Check states to see which action to take
	// if(posz&&posy)
	// 	return 2;

	// if(negz&&posy)
	// 	return 2;

	// if(posz&&negy)
	// 	return 2;

	// if(negz&&negy)
	// 	return 2;

	// if(posx&&posy)
	// 	return 4;

	// if(negx&&posy)
	// 	return 4;

	// if(posx&&negy)
	// 	return 4;

	// if(negx&&negy)
	// 	return 4;


	if (posz)
		return 3;

	if (negz)
		return -3;

	if (posx)
		return 1;

	if (negx)
		return -1;

	// if(posy)
	// 	return 2;

	// if(negy)
	// 	return -2;

	return 0;
}
