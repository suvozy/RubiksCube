#pragma once

#include "Cubelet.h"

class Cube3D
{
public:
	Cube3D(void);
	~Cube3D(void);

	void Render();
	void Rotate(int axis, int block, int degrees);
	void ResetAngle(){ for(int i=0;i<3;i++) for(int j=0;j<3;j++) rotateAng[i][j]=0; }
	void SetColor(int face,int row,int col,int color);

	void Reset();

private:
	Cubelet *cubes[3][3][3];
	float rotateAng[3][3]; // axis, block
};
