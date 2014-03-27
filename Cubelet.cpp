#include "StdAfx.h"
#include "cubelet.h"
#include <gl\gl.h>
#include <math.h>

#define M_PI 3.1415926535897932f

Cubelet::Cubelet()
{
	sideLength = 0.8;
	roundWidth = (1-sideLength)/2;

	blankFaceColor[0] = BorderColor[0] / 225;
	blankFaceColor[1] = BorderColor[1] / 225;
	blankFaceColor[2] = BorderColor[2] / 225;

	for(int i = 0; i < 6; i++) for(int j = 0; j < 3; j++) 
		faceColors[i][j] = blankFaceColor[j];

	sideMaterial[0] = sideMaterial[1] = sideMaterial[2] = 0.15f;
	sideMaterial[3] = 1.0;
	shininess[0] = 2; // The sides have small highlights.
}

Cubelet::~Cubelet(void)
{
}


void Cubelet::Render()
{
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, sideMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glBegin(GL_QUADS);

	// Right
	glColor3fv(faceColors[FACE_RIGHT]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColors[FACE_RIGHT]);
	glNormal3f(1, 0, 0);

	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2,  sideLength / 2);
	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2, -sideLength / 2);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2, -sideLength / 2);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2,  sideLength / 2);

	glColor3fv(blankFaceColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blankFaceColor);

	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth);
	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth);
	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2,               sideLength / 2 + roundWidth);

	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2,               sideLength / 2 + roundWidth);

	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2,  sideLength / 2 + roundWidth);
	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2,  sideLength / 2);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2,  sideLength / 2);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2,  sideLength / 2 + roundWidth);

	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2, -sideLength / 2 - roundWidth);
	glVertex3f(sideLength / 2 + roundWidth,  sideLength / 2, -sideLength / 2);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2, -sideLength / 2);
	glVertex3f(sideLength / 2 + roundWidth, -sideLength / 2, -sideLength / 2 - roundWidth);


	//Left
	glColor3fv(faceColors[FACE_LEFT]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColors[FACE_LEFT]);
	glNormal3f(-1, 0, 0);

	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,  sideLength / 2);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2, -sideLength / 2);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2, -sideLength / 2);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,  sideLength / 2);

	glColor3fv(blankFaceColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blankFaceColor);

	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,               sideLength / 2 + roundWidth);

	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,               sideLength / 2 + roundWidth);

	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,  sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,  sideLength / 2);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,  sideLength / 2);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,  sideLength / 2 + roundWidth);

	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2, -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2, -sideLength / 2);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2, -sideLength / 2);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2, -sideLength / 2 - roundWidth);


	// Top
	glColor3fv(faceColors[FACE_UP]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColors[FACE_UP]);
	glNormal3f(0, 1, 0);

	glVertex3f(-sideLength / 2,  sideLength / 2 + roundWidth,  -sideLength / 2);
	glVertex3f(-sideLength / 2,  sideLength / 2 + roundWidth,   sideLength / 2);
	glVertex3f( sideLength / 2,  sideLength / 2 + roundWidth,   sideLength / 2);
	glVertex3f( sideLength / 2,  sideLength / 2 + roundWidth,  -sideLength / 2);

	glColor3fv(blankFaceColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blankFaceColor);

	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth,  -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,               sideLength / 2 + roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,               sideLength / 2 + roundWidth,  -sideLength / 2 - roundWidth);

	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth,  -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2,               sideLength / 2 + roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2,               sideLength / 2 + roundWidth,  -sideLength / 2 - roundWidth);

	glVertex3f(-sideLength / 2,               sideLength / 2 + roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,               sideLength / 2 + roundWidth,   sideLength / 2);
	glVertex3f( sideLength / 2,               sideLength / 2 + roundWidth,   sideLength / 2);
	glVertex3f( sideLength / 2,               sideLength / 2 + roundWidth,   sideLength / 2 + roundWidth);

	glVertex3f(-sideLength / 2,               sideLength / 2 + roundWidth,  -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2,               sideLength / 2 + roundWidth,  -sideLength / 2);
	glVertex3f( sideLength / 2,               sideLength / 2 + roundWidth,  -sideLength / 2);
	glVertex3f( sideLength / 2,               sideLength / 2 + roundWidth,  -sideLength / 2 - roundWidth);


	//Bottom
	glColor3fv(faceColors[FACE_DOWN]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColors[FACE_DOWN]);
	glNormal3f(0, -1, 0);

	glVertex3f(-sideLength / 2, -sideLength / 2 - roundWidth, -sideLength / 2);
	glVertex3f(-sideLength / 2, -sideLength / 2 - roundWidth,  sideLength / 2);
	glVertex3f( sideLength / 2, -sideLength / 2 - roundWidth,  sideLength / 2);
	glVertex3f( sideLength / 2, -sideLength / 2 - roundWidth, -sideLength / 2);

	glColor3fv(blankFaceColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blankFaceColor);

	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth,  -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,                -sideLength / 2 - roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,                -sideLength / 2 - roundWidth,  -sideLength / 2 - roundWidth);

	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth,  -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2,              -sideLength / 2 - roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2,              -sideLength / 2 - roundWidth,  -sideLength / 2 - roundWidth);

	glVertex3f(-sideLength / 2,              -sideLength / 2 - roundWidth,   sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,              -sideLength / 2 - roundWidth,   sideLength / 2);
	glVertex3f( sideLength / 2,              -sideLength / 2 - roundWidth,   sideLength / 2);
	glVertex3f( sideLength / 2,              -sideLength / 2 - roundWidth,   sideLength / 2 + roundWidth);

	glVertex3f(-sideLength / 2,              -sideLength / 2 - roundWidth,  -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2,              -sideLength / 2 - roundWidth,  -sideLength / 2);
	glVertex3f( sideLength / 2,              -sideLength / 2 - roundWidth,  -sideLength / 2);
	glVertex3f( sideLength / 2,              -sideLength / 2 - roundWidth,  -sideLength / 2 - roundWidth);


	// Front
	glColor3fv(faceColors[FACE_FRONT]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColors[FACE_FRONT]);
	glNormal3f(0, 0, 1);

	glVertex3f( sideLength / 2,  sideLength / 2, sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,  sideLength / 2, sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2, -sideLength / 2, sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2, -sideLength / 2, sideLength / 2 + roundWidth);

	glColor3fv(blankFaceColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blankFaceColor);

	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth, sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2,              sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,              sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth, sideLength / 2 + roundWidth);

	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth, sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2,              sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,              sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth, sideLength / 2 + roundWidth);

	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2,                sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2,               sideLength / 2,                sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2,              -sideLength / 2,                sideLength / 2 + roundWidth);
	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2,                sideLength / 2 + roundWidth);

	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,                sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,               sideLength / 2,                sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2,              -sideLength / 2,                sideLength / 2 + roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,                sideLength / 2 + roundWidth);


	//Back
	glColor3fv(faceColors[FACE_BACK]);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, faceColors[FACE_BACK]);
	glNormal3f(0, 0, -1);

	glVertex3f( sideLength / 2,  sideLength / 2, -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2,  sideLength / 2, -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2, -sideLength / 2, -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2, -sideLength / 2, -sideLength / 2 - roundWidth);

	glColor3fv(blankFaceColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blankFaceColor);

	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth);

	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,              -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth, -sideLength / 2 - roundWidth);

	glVertex3f( sideLength / 2 + roundWidth,  sideLength / 2,                -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2,               sideLength / 2,                -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2,              -sideLength / 2,                -sideLength / 2 - roundWidth);
	glVertex3f( sideLength / 2 + roundWidth, -sideLength / 2,                -sideLength / 2 - roundWidth);

	glVertex3f(-sideLength / 2 - roundWidth,  sideLength / 2,                -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2,               sideLength / 2,                -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2,              -sideLength / 2,                -sideLength / 2 - roundWidth);
	glVertex3f(-sideLength / 2 - roundWidth, -sideLength / 2,                -sideLength / 2 - roundWidth);

	glEnd();

	glPopMatrix();
}


void Cubelet::SetColor(int face,int color)
{
	faceColors[face][0] = ColorValue[color][0] / 225;
	faceColors[face][1] = ColorValue[color][1] / 225;
	faceColors[face][2] = ColorValue[color][2] / 225;
}
