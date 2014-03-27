#include "StdAfx.h"
#include "Cube3D.h"
#include <gl\gl.h>

Cube3D::Cube3D(void)
{
	//create each cubelet that makeup the rubic's cube
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) for (int k = 0; k < 3; k++)
		cubes[i][j][k] = new Cubelet();

	//set the rotation angles to 0 degree
	ResetAngle();

	//set the colors for each side
	Reset();
}

Cube3D::~Cube3D(void)
{
	//destroy the cubelets of the cube
	for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) for (int k = 0; k < 3; k++)
				delete cubes[i][j][k];
}

void Cube3D::Render() 
{
	//render each cublete
	for (int i = 0; i < 3; i++) 
	{
		glRotatef(rotateAng[0][i], 1, 0, 0); // x axis
		for (int j = 0; j < 3; j++) 
		{
			glRotatef(rotateAng[1][j], 0, 1, 0); // y axis
			for (int k = 0; k < 3; k++) 
			{
				glRotatef(rotateAng[2][k], 0, 0, 1); // z axis

				glTranslatef(i-1.0f, j-1.0f, k-1.0f);
				cubes[i][j][k]->Render();
				glTranslatef(1.0f-i, 1.0f-j, 1.0f-k);

				glRotatef(-rotateAng[2][k], 0, 0, 1);
			}
			glRotatef(-rotateAng[1][j], 0, 1, 0);
		}
		glRotatef(-rotateAng[0][i], 1, 0, 0);
	}
}

void Cube3D::Rotate(int axis, int block, int degrees) 
{
	rotateAng[axis][block] += degrees;
}

void Cube3D::SetColor(int face,int row,int col,int color)
{
	int X = 0, Y = 0, Z = 0;

	switch (face)
	{
	case FACE_FRONT :  Y = abs(row-2);	X = col;		Z = 2;			break;
	case FACE_BACK  :  Y = abs(row-2);	X = abs(col-2);	Z = 0;			break;
	case FACE_UP    :  Y = 2;			X = col;		Z = row;		break;
	case FACE_DOWN  :  Y = 0;			X = col;		Z = abs(row-2);	break;
	case FACE_LEFT  :  X = 0;			Y = abs(row-2);	Z = col;		break;
	case FACE_RIGHT :  X = 2;			Y = abs(row-2);	Z = abs(col-2);	break;
	}

	cubes[X][Y][Z]->SetColor(face, color);
}


void Cube3D::Reset()
{
	for(int f = 0; f < 6; f++)
	{
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				SetColor(f, i, j, f);
	}
}
