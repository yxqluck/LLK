#pragma once
#include "afxdialogex.h"
#include "global.h"
#include "CGameControl.h"


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
    CDC m_dcBg; // DC that holds the original background bitmap
	CBitmap m_bmpMem; // bitmap selected into m_dcMem (back buffer)
	CDC m_dcElement;
	CDC m_dcMask;
    CRect m_rtGameRect;
  int m_anMap[MAP_ROWS][MAP_COLS];
	CPoint m_ptGameTop;
	CSize m_sizeElem;
	bool m_bFirstPoint;
	Vertex m_ptSelFirst;
	Vertex m_ptSelSec;

	bool flag;
	bool pointFlag;

	CGameControl m_gameControl;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void InitBackground();
	void InitElement();
	void OnLButtonUp(UINT nFlags, CPoint point);
    void DrawTipFrame(int nRow, int nCol);
	bool IsLink();
	void DrawTipLine(Vertex avPath[], int pathLen);
	void UpdateMap();
	void OverGame();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonStart();
    enum { ID_LINE_TIMER = 1 };
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonPoint();
protected:
	void dfs(Vertex begin);
};


