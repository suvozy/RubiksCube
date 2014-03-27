// RubiksCubeDlg.h : header file
//

#pragma once

#include "CubeWnd.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Solver/Settings.h"
#include "Solver/CubePosition.h"
#include "Solver/SolverKociemba.h"

// CRubiksCubeDlg dialog
class CRubiksCubeDlg : public CDialog
{
private:
	CubePosition   cubePos;
	SolverKociemba solverKoci;
	MoveSequence   generator;

	int            curIndex;
	bool           flatView;
	CListBox       moveList;
	CStatic        moveTitle;

	CubeWnd       cubeWnd;

	void ClearMoves();
	void DoMove(int m, int q);
	void Enable(bool enable);
	void EnableNextPrev();

// Construction
public:
	CRubiksCubeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RUBIKSCUBE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnBnClickedSolve();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedShuffle();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	afx_msg void OnLbnSelchangeMovelist();
	afx_msg void OnBnClickedChangeView();
	afx_msg void OnBnClickedResetView();
};
