// RubiksCubeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RubiksCube.h"
#include "RubiksCubeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRubiksCubeDlg dialog

CRubiksCubeDlg::CRubiksCubeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRubiksCubeDlg::IDD, pParent)
, curIndex(0)
, flatView(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	solverKoci.run();
}

void CRubiksCubeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUBE, cubeWnd);
	DDX_Control(pDX, IDC_MOVELIST, moveList);
	DDX_Control(pDX, IDC_MOVETITLE, moveTitle);
}

BEGIN_MESSAGE_MAP(CRubiksCubeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SOLVE, OnBnClickedSolve)
	ON_BN_CLICKED(IDC_EXIT, OnBnClickedExit)
	ON_BN_CLICKED(IDC_SHUFFLE, OnBnClickedShuffle)
	ON_BN_CLICKED(IDC_RESET, OnBnClickedReset)
	ON_BN_CLICKED(IDC_PREV, OnBnClickedPrev)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_LBN_SELCHANGE(IDC_MOVELIST, OnLbnSelchangeMovelist)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHANGE_VIEW, &CRubiksCubeDlg::OnBnClickedChangeView)
	ON_BN_CLICKED(IDC_RESET_VIEW, &CRubiksCubeDlg::OnBnClickedResetView)
END_MESSAGE_MAP()


// CRubiksCubeDlg message handlers

BOOL CRubiksCubeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	cubeWnd.InitGL();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	EnableNextPrev();
	OnBnClickedChangeView();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRubiksCubeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRubiksCubeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//The Exit button was pressed
void CRubiksCubeDlg::OnBnClickedExit()
{
	CDialog::OnCancel();
	//if (MessageBox("Are you sure you want to exit?","Exit!",MB_YESNO|MB_ICONQUESTION) == IDYES)
	//{
	//	CDialog::OnCancel();
	//}
}

//Enable/Disable controls on the Screen
void CRubiksCubeDlg::Enable(bool enable)
{
	//cubeWnd.EnableWindow(enable);
	//GetDlgItem(IDC_RESET_VIEW)->EnableWindow(enable);
	//GetDlgItem(IDC_CHANGE_VIEW)->EnableWindow(enable);

	moveList.EnableWindow(enable);
	GetDlgItem(IDC_SOLVE)->EnableWindow(enable);
	GetDlgItem(IDC_SHUFFLE)->EnableWindow(enable);
	GetDlgItem(IDC_RESET)->EnableWindow(enable);
	GetDlgItem(IDC_EXIT)->EnableWindow(enable);

	if (enable)
	{
		EnableNextPrev();
	}
	else
	{
		GetDlgItem(IDC_NEXT)->EnableWindow(enable);
		GetDlgItem(IDC_PREV)->EnableWindow(enable);
	}
	moveList.SetFocus();
}

void CRubiksCubeDlg::EnableNextPrev()
{
	GetDlgItem(IDC_PREV)->EnableWindow(curIndex > 0);
	GetDlgItem(IDC_NEXT)->EnableWindow(curIndex < moveList.GetCount()-1);
}

//override to get key presses
BOOL CRubiksCubeDlg::PreTranslateMessage(MSG* pMsg)
{
	// override to keep Enter and Esc to close dialog
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->wParam = NULL ;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CRubiksCubeDlg::ClearMoves()
{
	moveList.ResetContent();
	moveTitle.SetWindowText("Moves");
	curIndex = 0;
	EnableNextPrev();
}

//Shuffle/Scramble the cube
void CRubiksCubeDlg::OnBnClickedShuffle()
{
	ClearMoves();
	
	solverKoci.mix(&cubePos);
	cubeWnd.SetCube(cubePos);
	cubeWnd.Invalidate();
}

//reset the cube
void CRubiksCubeDlg::OnBnClickedReset()
{
	ClearMoves();
	
	cubePos.reset();
	cubeWnd.Reset();
	cubeWnd.SetCube(cubePos);
	cubeWnd.Invalidate();
}

//solve the cube
void CRubiksCubeDlg::OnBnClickedSolve()
{
	ClearMoves();

	Enable(FALSE);

	if(solverKoci.setPosition(cubePos, false)) solverKoci.run();

	generator = solverKoci.getGenerator();
	std::string moves = generator.toString(true);

	char faceStr[6][10] = {"Left ","Up ","Front ","Right ","Down ","Back "};
	char moveStr[6][12] = {"x","x","Middle ","Cube ","Slice ","Anti-Slice "};
	char qStr[3][10] = {"CW","180","CCW"};

	if(generator.getLength()!=0)
	{
		moveList.AddString("..");

		for(int i = generator.getLength()-1; i >= 0 ; i--)
		{
			CString sol = "";
			int m=generator.getMoves()[i],q=4-generator.getAmount()[i];

			if(m<6){
				sol += string("LUFRDB")[m];
			}else{
				sol += string("LUF")[m%3];
				sol += string("xxmcsa")[m/3];
			}
			if(q>1) sol += string("2'")[q-2];

			sol += "  -  ";
			if(m<6){
				sol.Append(faceStr[m]);
			}else{
				sol.Append(faceStr[m%3]);
				sol.Append(moveStr[m/3]);
			}
			sol.Append(qStr[q-1]);


			moveList.AddString(sol);
		}

		moveList.SetCurSel(0);
		curIndex = 0;
		CString str; str.Format("%d Moves",generator.getLength());
		moveTitle.SetWindowText(str);

		EnableNextPrev();
	}

	solverKoci.setPosition(cubePos, true);

	Enable(TRUE);

	/*
	if (Ret == 1)
	{
		MessageBox("Cube does not contain proper cubelets.  Check configuration and try again.","Configuration Error",MB_OK|MB_ICONWARNING);
	}
	else if (Ret == 2)
	{
		MessageBox("Cube mis-oriented.  If the configuration you have entered is indeed what you have,\n"
			"then your cube appears to have been disassembled, and re-assembled incorrctly.\n"
			"Check configuration and try again.","Unable To Solve Cube",MB_OK|MB_ICONHAND);
	}*/
}


//Do a Move out of the move list
void CRubiksCubeDlg::DoMove(int m,int q)
{
	cubeWnd.DoMove(m,q);
	cubePos.doMove(m,q,true);
	cubeWnd.SetCube(cubePos);
	cubeWnd.UpdateDisplay();
}

//go to previous move in the move list
void CRubiksCubeDlg::OnBnClickedPrev()
{
	Enable(FALSE);

	if (curIndex <= 0) return;

	--curIndex;

	DoMove(generator.getMoves()[generator.getLength()-1-curIndex], 
		generator.getAmount()[generator.getLength()-1-curIndex]);

	moveList.SetCurSel(curIndex);

	Enable(TRUE);
}

//go to the next move in the move list
void CRubiksCubeDlg::OnBnClickedNext()
{
	Enable(FALSE);

	if (curIndex >= moveList.GetCount()-1) return;

	DoMove(generator.getMoves()[generator.getLength()-1-curIndex], 
		4-generator.getAmount()[generator.getLength()-1-curIndex]);

	moveList.SetCurSel(++curIndex);

	Enable(TRUE);
}

//jump to a move in the move list
void CRubiksCubeDlg::OnLbnSelchangeMovelist()
{
	Enable(FALSE);

	int Index = moveList.GetCurSel();
	moveList.SetCurSel(curIndex);

	while (curIndex < Index) OnBnClickedNext(); //if we are going forward
	while (curIndex > Index) OnBnClickedPrev(); //if we are going back
	
	Enable(TRUE);
}

void CRubiksCubeDlg::OnBnClickedChangeView()
{
	flatView = !flatView;

	if (!flatView)
	{
		//OnColorSelected(NULL, NULL);
	}

	cubeWnd.SetFlat(flatView);
	cubeWnd.Invalidate();
	cubeWnd.UpdateWindow();
}

void CRubiksCubeDlg::OnBnClickedResetView()
{
	cubeWnd.ResetView();
}
