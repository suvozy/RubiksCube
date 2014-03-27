// CubeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RubiksCube.h"
#include "CubeWnd.h"

// CubeWnd

IMPLEMENT_DYNAMIC(CubeWnd, CWnd)
CubeWnd::CubeWnd()
: selectedColor(2)
, m_hRC(NULL)
, flat(true)
, speed(800)
{
	for(int i=0; i<6; i++)
		colors[i]   = RGB(ColorValue[i][0], ColorValue[i][1], ColorValue[i][2]);

	ResetView();

	for(int k = 0; k < 6; k++) for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++)
		cubeFlat[k][i][j] = k;
 
	numTurns = speed / 40;
	if (numTurns < 2) numTurns = 2;

	while (90 % numTurns) numTurns++;

	aniDegree =  90 / numTurns;
	ASSERT(90 % Degrees == 0);
}

CubeWnd::~CubeWnd()
{
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(m_hRC);
}

// CubeWnd message handlers

BEGIN_MESSAGE_MAP(CubeWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ENABLE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CubeWnd::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	//	InitGL();		// Initialize OpenGL
}

void CubeWnd::OnEnable(BOOL bEnable)
{
	CWnd::OnEnable(bEnable);
	
	UpdateDisplay();
}

void CubeWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (flat)
	{
		DrawFlatCube(&dc);
		/*
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix
		glTranslated(0,0,-1);								//The Screen 7.0

		glPushMatrix();

		float m_FaceMaterial[4], m_Shininess[1], m_Faces[3], n[3];
		m_FaceMaterial[0] = m_FaceMaterial[1] = m_FaceMaterial[2] = 0.15f; m_FaceMaterial[3] = 1.0;
		m_Faces[0] = m_Faces[1] = m_Faces[2] = 0.1f;
		n[0] = n[1] = n[2] = 1;
		m_Shininess[0] = 2; 

		glMaterialfv(GL_FRONT, GL_SPECULAR, m_FaceMaterial);
		glMaterialfv(GL_FRONT, GL_SHININESS, m_Shininess);

		glBegin(GL_QUADS);

		glColor3fv(m_Faces);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Faces);
		glNormal3f(0, 0, 1);

		glVertex3f(-.5f, .5f, 0);
		glVertex3f( .5f, .5f, 0);
		glVertex3f( .5f,-.5f, 0);
		glVertex3f(-.5f,-.5f, 0);

		glColor3fv(n);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, n);
		glNormal3f(0, 0, 1);

		glVertex3f(-.3f, .3f, 0);
		glVertex3f( .3f, .3f, 0);
		glVertex3f( .3f,-.3f, 0);
		glVertex3f(-.3f,-.3f, 0);

		glEnd();

		glPopMatrix();

		SwapBuffers(m_hDC);*/
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix
		glTranslated(0,0,-7);								//The Screen 7.0
		glRotatef(rotateX,1,0,0);
		glRotatef(rotateY,0,1,0);

		cube3D.Render();

		SwapBuffers(m_hDC);
	}
}

void CubeWnd::DrawFlatCube(CDC *pDC)
{
	CDC MemDC;
	CBitmap Bmp;
	CRect Rect;

	GetClientRect(&Rect);
	MemDC.CreateCompatibleDC(pDC);
	Bmp.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());
	CBitmap *OldBmp = MemDC.SelectObject(&Bmp);

	int X = Rect.Width() / 12;
	int Y = Rect.Height() / 9;
	cubeWidth = min(X,Y);

	CBrush Brush(RGB(204, 204, 204));
	MemDC.FillRect(&Rect,&Brush);

	CPoint origin((Rect.Width() - (cubeWidth * 12)) / 2, (Rect.Height() - (cubeWidth * 9)) / 2);

	DrawFlatFace(&MemDC, FACE_UP,    origin + CPoint(cubeWidth * 3,0));
	DrawFlatFace(&MemDC, FACE_LEFT,  origin + CPoint(0,cubeWidth * 3));
	DrawFlatFace(&MemDC, FACE_FRONT, origin + CPoint(cubeWidth * 3,cubeWidth * 3));
	DrawFlatFace(&MemDC, FACE_RIGHT, origin + CPoint(cubeWidth * 6,cubeWidth * 3));
	DrawFlatFace(&MemDC, FACE_BACK,  origin + CPoint(cubeWidth * 9,cubeWidth * 3));
	DrawFlatFace(&MemDC, FACE_DOWN,  origin + CPoint(cubeWidth * 3,cubeWidth * 6));

	DrawColorPalet(&MemDC, origin + CPoint(cubeWidth * 7.5f,cubeWidth * 7.5f));

	pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,0,0,SRCCOPY);

	MemDC.SelectObject(OldBmp);
}

void CubeWnd::DrawFlatFace(CDC *pDC,int face,CPoint start)
{
	for (int i = 0; i < 3;i++)
	{
		for (int j = 0; j < 3;j++)
		{
			CPen Pen;
			CBrush Brush(colors[cubeFlat[face][i][j]]);

			Pen.CreatePen(PS_SOLID, .1 * cubeWidth, RGB(BorderColor[0], BorderColor[1], BorderColor[2]));

			CBrush *pOldBrush = pDC->SelectObject(&Brush);
			CPen *pOldPen = pDC->SelectObject(&Pen);

			CRect Rect(start + CPoint(cubeWidth * j, cubeWidth * i),
				start + CPoint(cubeWidth * (j+1), cubeWidth * (i+1)));
			pDC->Rectangle(&Rect);

			pDC->SelectObject(pOldBrush);
			pDC->SelectObject(pOldPen);
		}
	}

	cubeFlatRect[face].SetRect(start, start + CPoint(cubeWidth * 3,cubeWidth * 3));
}

void CubeWnd::DrawColorPalet(CDC *pDC, CPoint start)
{
	int paletWidth = cubeWidth * .7f;
	int selectedIndex;

	for(int i = 0; i < 6; i++)
	{
		if(i == selectedColor) selectedIndex = i;

		CRect Rect(start + CPoint(paletWidth * i, 0), start + CPoint(paletWidth * (i+1), paletWidth));
		paletRect[i] = Rect;

		pDC->FillSolidRect(Rect, colors[i]);
	}

	CPen Pen;
	CBrush Brush(colors[selectedIndex]);

	Pen.CreatePen(PS_SOLID, .15 * cubeWidth, RGB(BorderColor[0], BorderColor[1], BorderColor[2]));

	CBrush *pOldBrush = pDC->SelectObject(&Brush);
	CPen *pOldPen = pDC->SelectObject(&Pen);

	pDC->Rectangle(&paletRect[selectedIndex]);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CubeWnd::SetSquareUnderPoint(CPoint point)
{
	for(int Face = 0; Face < 6; Face++)
		if (cubeFlatRect[Face].PtInRect(point)) 
		{
			CRect Rect = cubeFlatRect[Face];

			int i = (point.y - Rect.top) / cubeWidth;
			int j = (point.x - Rect.left) / cubeWidth;

			if(i == 1 && j == 1) return;

			cubeFlat[Face][i][j] = selectedColor;
			cube3D.SetColor(Face,i,j,selectedColor);
			UpdateDisplay();

			return;
		}
}

void CubeWnd::SetColorUnderPoint(CPoint point)
{
	for(int i = 0; i < 6; i++)
		if(paletRect[i].PtInRect(point))
		{ selectedColor = i; return; }
}

void CubeWnd::UpdateDisplay()
{
	Invalidate();
	UpdateWindow();
}

void CubeWnd::YieldToOthers()
{
	DWORD Interval = speed;
	if (!flat)
	{
		Interval = Interval / ((speed/ 10)+1);
	}
	DWORD start = GetTickCount();
	DWORD Current = 0;   //so we do the loop at least once
	MSG Msg;
	while (Current < start + Interval)
	{
		while (PeekMessage(&Msg,NULL,0,0,PM_REMOVE))
		{
			if (!PreTranslateMessage(&Msg))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
		Current = GetTickCount();
	}
}

BOOL CubeWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CubeWnd::SetFlat(bool Flat)
{
	flat = Flat;
	
	UpdateDisplay();
}

void CubeWnd::ResetView()
{
	rotateX = 30.0f;
	rotateY = -35.0f;

	UpdateDisplay();
}

void CubeWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	lastPoint = point;
	SetCapture();

	if (flat)
	{
		SetSquareUnderPoint(point);
		SetColorUnderPoint(point);
	}

	UpdateDisplay();

	CWnd::OnLButtonDown(nFlags, point);
}

void CubeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void CubeWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!flat)
	{
		if (nFlags & MK_LBUTTON)
		{
			rotateX += (point.y - lastPoint.y);
			int TempRotate = (int)rotateX;
			if (TempRotate< 0)
			{
				TempRotate += 360;
			}
			TempRotate = TempRotate % 360;
			if (TempRotate > 90 && TempRotate < 270)
			{
				rotateY -= (point.x - lastPoint.x);
			}
			else
			{
				rotateY += (point.x - lastPoint.x);
			}

			lastPoint = point;

			UpdateDisplay();
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

int CubeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	InitGL();
	return 0;
}



//open gl functions
void CubeWnd::ReSizeGLScene( int width, int height)	// Resize And Initialize The GL Window
{
	// from NeHe's Tutorial 3
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0,(float)width/(float)height,0.1,100.0);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

// Intitialize OpenGl
// All Setup For OpenGL Goes Here
int CubeWnd::InitGL() 
{
	m_hDC = ::GetDC(this->m_hWnd);
	if(!SetPixelformat(m_hDC))	// set pixel format
	{
		::MessageBox(::GetFocus(),"SetPixelformat Failed!","Error",MB_OK);
	}

	m_hRC = wglCreateContext(m_hDC);	// Create an OpenGL rendering context 
	int i= wglMakeCurrent(m_hDC,m_hRC);	// Make rendering context current

	// from NeHe's Tutorial 3
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.8f, 0.8f, 0.8f, 0.5f);			    // Background
	glClearDepth(1.0);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f }; // faint white light
	GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0 }; // white diffuse
	GLfloat light_specular[] = { 0.35f, 0.35f, 0.35f, 1.0 }; // highlights
	GLfloat light_position[] = { 0.0, 10.0, 20.0, 1.0 };

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	CRect Rect;
	GetClientRect(&Rect);
	ReSizeGLScene(Rect.Width(),Rect.Height());
	return TRUE;										// Initialization Went OK
}

BOOL CubeWnd::SetPixelformat(HDC hdc)
{

	PIXELFORMATDESCRIPTOR *ppfd; 
	int pixelformat; 

	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_GENERIC_FORMAT |
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		32,                    // 32-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		8,                     // no alpha buffer 
		0,                     // shift bit ignored 
		8,                     // no accumulation buffer 
		0, 0, 0, 0,            // accum bits ignored 
		64,                    // 64-bit z-buffer	 
		8,                     // stencil buffer 
		8,                     // auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	}; 


	ppfd = &pfd;


	if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
	{ 
		::MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
		return FALSE; 
	} 

	if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
	{ 
		::MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
		return FALSE; 
	} 

	return TRUE; 

}
void CubeWnd::DoMove(int m, int q)
{
	if(m>=9 && m<=11) return; // TODO: m=9-11, cube turn

	int axis=m%3, block;

	switch(m<6 ? m : m%3)
	{
	case 0: block=0; break;
	case 1: block=2; break;
	case 2: block=2; break;
	case 3: block=2; break;
	case 4: block=0; break;
	case 5: block=0; break;
	}

	if(m>=6 && m<=8) block=1; // m=6-8, middle layer turn

	for (int i = 0; i < numTurns * (q==2?2:1);i++)
	{
		cube3D.Rotate(axis, block, (q==3?-1:1) * (block==2?-1:1) * aniDegree );

		if((m>=12 && m<=14) || m>14)// m=12-14, slice move // m=15-17, anti-slice move
			cube3D.Rotate(axis, block==0?2:0, (q==3?-1:1) * (block==2?-1:1) * aniDegree * (m>14?-1:1));

		if (!flat)
		{
			UpdateDisplay();
			YieldToOthers();
		}
	}
}

void CubeWnd::Reset()
{
	cube3D.Reset();

	for(int k = 0; k < 6; k++) for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++)
		cubeFlat[k][i][j] = k;
}

void CubeWnd::SetCube(CubePosition cubePos)
{
	cube3D.ResetAngle();

	cubePos.getFaceletColors();

	int c=0;

	for(int k = 0; k < 6; k++)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				cubeFlat[k][i][j] = cubePos.cubeletPerm[20+cubePos.faceletColor[c]]-20;
				cube3D.SetColor(k,i,j,cubePos.cubeletPerm[20+cubePos.faceletColor[c]]-20);

				c++;
			}
		}
	}
}
