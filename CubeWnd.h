#pragma once

#include "Cube3D.h"
#include "Solver/CubePosition.h"

// CubeWnd
class CubeWnd : public CWnd
{
	DECLARE_DYNAMIC(CubeWnd)

public:
	CubeWnd();
	virtual ~CubeWnd();

	//mutator functions
	void SetFlat(bool flat);
	bool IsFlat() { return flat; }
	void ResetView();

	//opengl functions
	int InitGL();

	void Reset();
	void SetCube(CubePosition cubePos);
	void DoMove(int m, int q);

	void UpdateDisplay();

protected:
	virtual void PreSubclassWindow();

	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	void YieldToOthers();

private:
	int               selectedColor;  //currently selected color
	bool              flat;        //flat or 3d

	//this is the color that is going to be assigned
	//to cube on lbuttondown
	CPoint            lastPoint;   //used for the rotation of 3d Cube
	float             rotateX;     //rotation of 3d Cube
	float             rotateY;
	HDC               m_hDC;         //hDC is of this window
	HGLRC		      m_hRC;         //hRC for 3d cube
	int               cubeFlat[6][3][3];
	CRect             cubeFlatRect[6];
	Cube3D           cube3D;      //3d cube object
	COLORREF          colors[6];
	CRect             paletRect[6];
	int				  cubeWidth;
	int               speed, aniDegree, numTurns;

	//drawing flat cube
	void DrawFlatCube(CDC *pDC);
	void DrawFlatFace(CDC *pDC,int face,CPoint start);
	void DrawColorPalet(CDC *pDC,CPoint start);

	//opengl functions
	void ReSizeGLScene( int width, int height);
	BOOL SetPixelformat(HDC hdc);	

	//assign color ot square on cube
	void SetSquareUnderPoint(CPoint point);
	void SetColorUnderPoint(CPoint point);
};
