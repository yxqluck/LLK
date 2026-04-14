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

	m_ptGameTop.x = 50;
	m_ptGameTop.y = 50;

	m_sizeElem.cx = 40;
	m_sizeElem.cy = 40;

	m_bFirstPoint = true;

	m_rtGameRect.top = m_ptGameTop.y;
	m_rtGameRect.left = m_ptGameTop.x;
    m_rtGameRect.right = m_rtGameRect.left + MAP_COLS * m_sizeElem.cx;
	m_rtGameRect.bottom = m_rtGameRect.top + MAP_ROWS * m_sizeElem.cy;

}

void CGame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_LINE_TIMER) {
		KillTimer(ID_LINE_TIMER);
		// restore area (clears the line) and redraw map
		UpdateMap();
	} else {
		CDialogEx::OnTimer(nIDEvent);
	}
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

	// create a DC that holds the original background bitmap (select m_bmpMain into it)
	if (!m_dcBg.GetSafeHdc())
		m_dcBg.CreateCompatibleDC(&dc);
	m_dcBg.SelectObject(&m_bmpMain);

	// create a back-buffer DC and bitmap (m_dcMem will hold the composed image)
	if (!m_dcMem.GetSafeHdc())
		m_dcMem.CreateCompatibleDC(&dc);
	// determine bitmap size
	BITMAP bm = {0};
	m_bmpMain.GetBitmap(&bm);
	int bmpW = bm.bmWidth ? bm.bmWidth : 800;
	int bmpH = bm.bmHeight ? bm.bmHeight : 600;
	// create a compatible bitmap for the back buffer and select it
	m_bmpMem.CreateCompatibleBitmap(&dc, bmpW, bmpH);
	CBitmap* pOldMemBmp = m_dcMem.SelectObject(&m_bmpMem);
	// copy the original background into the back buffer
	m_dcMem.BitBlt(0, 0, bmpW, bmpH, &m_dcBg, 0, 0, SRCCOPY);
	// restore selection of the old mem bitmap if needed
	(void)pOldMemBmp;
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
  ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CGame::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CGame 消息处理程序

BOOL CGame::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitBackground();

	InitElement();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CGame::InitElement() {
	//获取当前对话框视频内存
	CClientDC dc(this);

	//加载BMP图片资源
	HANDLE hBmpBG=::LoadImage(NULL,_T("theme\\picture\\fruit_element.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	//创建兼容DC
	m_dcElement.CreateCompatibleDC(&dc);

	//将位图选入DC
	m_dcElement.SelectObject(hBmpBG);

	//加载BMP图片资源
	HANDLE hMask = ::LoadImage(NULL, _T("theme\\picture\\fruit_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//创建兼容DC
	m_dcMask.CreateCompatibleDC(&dc);

	//将位图选入DC
	m_dcMask.SelectObject(hMask);

}

void CGame::UpdateMap() {
	int nLeft = m_ptGameTop.x;
	int nTop = m_ptGameTop.y;
	int nElemW = m_sizeElem.cx;
	int nElemH = m_sizeElem.cy;
    // Restore the background for the game area from the original background DC
	// so removed elements are cleared before drawing current map.
	m_dcMem.BitBlt(
		m_rtGameRect.left, m_rtGameRect.top,
		m_rtGameRect.Width(), m_rtGameRect.Height(),
		&m_dcBg, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);


    for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			int nInfo = m_gameControl.GetElement(i, j);
			// skip empty slots marked as -1
			if (nInfo < 0)
				continue;
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcMask, 0, nInfo * nElemH, SRCPAINT);
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcElement, 0, nInfo * nElemH, SRCAND);
		}
	}

	Invalidate(FALSE);
}

void CGame::OnBnClickedButtonStart()
{
	//初始化游戏地图

	m_gameControl.StartGame();

	UpdateMap();

	//int nLeft = 50;
	//int nTop = 50;
	//int nElemW = 40;
	//int nElemH = 40;

	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		/*m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcElement, 0, m_anMap[i][j] * nElemH, SRCCOPY);*/
	//		m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcMask, 0, m_anMap[i][j] * nElemH, SRCPAINT);
	//		m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcElement, 0, m_anMap[i][j] * nElemH, SRCAND);
	//	}
	//}

	//Invalidate(FALSE);
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point) {

	// compute row/col relative to game top-left and validate
	int nRow = (point.y - m_ptGameTop.y) / m_sizeElem.cy;
	int nCol = (point.x - m_ptGameTop.x) / m_sizeElem.cx;

    if (nRow < 0 || nCol < 0 || nRow >= MAP_ROWS || nCol >= MAP_COLS) {
		return CDialogEx::OnLButtonUp(nFlags, point);
	}
	if (m_bFirstPoint) {
		DrawTipFrame(nRow, nCol);
        m_gameControl.SetFirstPoint(nRow, nCol);
		m_ptSelFirst.row = nRow;
		m_ptSelFirst.col = nCol;
	}
	else {
		DrawTipFrame(nRow, nCol);
     m_gameControl.SetSectPoint(nRow, nCol);
		m_ptSelSec.row = nRow;
		m_ptSelSec.col = nCol;

        Vertex avPath[4];
		int pathLen = 0;
		//判断是否为相同图片并获取连接路径
		if (m_gameControl.Link(avPath, pathLen)) {
			// 清除数据并更新地图
			UpdateMap();
			// 在内存DC上画出连线以覆盖背景
			DrawTipLine(avPath, pathLen);
			// 启动定时器，在 200ms 后清除连线（通过再次 UpdateMap 恢复背景）
			// 先确保没有上一次的定时器残留
			KillTimer(ID_LINE_TIMER);
			SetTimer(ID_LINE_TIMER, 200, NULL);
		}
		else {
			// 无连接，刷新显示
			UpdateMap();
			InvalidateRect(m_rtGameRect, FALSE);
		}
	}
	m_bFirstPoint = !m_bFirstPoint;
}

void CGame::DrawTipFrame(int nRow, int nCol) {
    // Draw the tip frame into the memory DC so it persists across repaints,
	// then request a repaint of the window to present the updated memory DC.
	CBrush brush(RGB(233, 43, 43));
	CRect rtTipFrame;
	rtTipFrame.left = m_ptGameTop.x + nCol * m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.y + nRow * m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;

	if (m_dcMem.GetSafeHdc()) {
		m_dcMem.FrameRect(&rtTipFrame, &brush);
		// trigger repaint to blit memory DC onto the window
		Invalidate(FALSE);
	} else {
		// fallback: draw directly on window DC
		CClientDC dc(this);
		dc.FrameRect(&rtTipFrame, &brush);
	}
}

bool CGame::IsLink() {
    // m_ptSel*.x is column, m_ptSel*.y is row. m_anMap is indexed as [row][col].
	// ignore if same cell or either is already empty
	if (m_ptSelFirst.row == m_ptSelSec.row && m_ptSelFirst.col == m_ptSelSec.col)
		return false;
    int val1 = m_gameControl.GetElement(m_ptSelFirst.row, m_ptSelFirst.col);
	int val2 = m_gameControl.GetElement(m_ptSelSec.row, m_ptSelSec.col);
	if (val1 < 0 || val2 < 0)
		return false;
	return (val1 == val2);
}

void CGame::DrawTipLine(Vertex avPath[], int pathLen) {
	if (pathLen < 2) return;

	auto pointFor = [this](const Vertex &v) {
		return CPoint(m_ptGameTop.x + v.col * m_sizeElem.cx + m_sizeElem.cx / 2,
			m_ptGameTop.y + v.row * m_sizeElem.cy + m_sizeElem.cy / 2);
	};

	if (m_dcMem.GetSafeHdc()) {
		CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
		CPen* pOldPen = m_dcMem.SelectObject(&penLine);

		CPoint p0 = pointFor(avPath[0]);
		m_dcMem.MoveTo(p0);
		for (int i = 1; i < pathLen; ++i) {
			CPoint pi = pointFor(avPath[i]);
			m_dcMem.LineTo(pi);
		}

		m_dcMem.SelectObject(pOldPen);
		Invalidate(FALSE);
	} else {
		CClientDC dc(this);
		CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));
		CPen* pOldPen = dc.SelectObject(&penLine);
		CPoint p0 = pointFor(avPath[0]);
		dc.MoveTo(p0);
		for (int i = 1; i < pathLen; ++i) {
			CPoint pi = pointFor(avPath[i]);
			dc.LineTo(pi);
		}
		dc.SelectObject(pOldPen);
	}
}
