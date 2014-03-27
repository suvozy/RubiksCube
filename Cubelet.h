#pragma once

class Cubelet
{
public:
	Cubelet();
	~Cubelet(void);

	void Render();
	void SetColor(int face, int color);

private:
	float sideLength;
	float roundWidth;

	float faceColors[6][3]; // face, RGB
	float blankFaceColor[3];
	float sideMaterial[4];
	float shininess[1];
};
