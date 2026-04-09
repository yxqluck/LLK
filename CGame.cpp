// CGame.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "framework.h"
#include "afxdialogex.h"
#include "CGame.h"


// CGame 对话框

IMPLEMENT_DYNAMIC(CGame, CDialogEx)

CGame::CGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_BASIC, pParent)
{

}

CGame::~CGame()
{
}

void CGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CGame::InitBackground() {
    // 加载位图到成员变量，保证位图在整个对话框生命周期内有效
	if (!m_bmpMain.LoadBitmap(IDB_FRUIT_BG)) {
		AfxMessageBox(_T("加载背景位图失败: IDB_FRUIT_BG"));
		return;
	}

	// 创建兼容DC并将成员位图选入
	CClientDC dc(this);
	if (!m_dcMem.GetSafeHdc())
		m_dcMem.CreateCompatibleDC(&dc);
	m_dcMem.SelectObject(&m_bmpMain);
}

afx_msg void CGame::OnPaint() {
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);

		CDialogEx::OnPaint();
	}
}


BEGIN_MESSAGE_MAP(CGame, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGame 消息处理程序

BOOL CGame::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitBackground();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
