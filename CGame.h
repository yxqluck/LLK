#pragma once
#include "afxdialogex.h"


// CGame 对话框

class CGame : public CDialogEx
{
	DECLARE_DYNAMIC(CGame)

public:
	CGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGame();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_BASIC };
#endif

protected:
	HICON m_hIcon;
	CDC m_dcMem;
    CBitmap m_bmpMain;
	CDC m_dcElement;
	CDC m_dcMask;
	int m_anMap[4][4];
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void InitBackground();
	void InitElement();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStart();
};
