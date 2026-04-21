// CGame.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "framework.h"
#include "afxdialogex.h"
#include "CGame.h"
#include "CGameLogic.h" 
#include "GameSetting.h"
#include "GameInput.h"


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
		// 恢复区域（清除连线）并重绘地图
		UpdateMap();
	}
	else if (nIDEvent == 2) {
		/*CTime time = CTime::GetCurrentTime();
		CString strTime = time.Format(_T("%H:%M:%S"));

		SetDlgItemText(IDC_STATIC_TEXT, strTime);*/
		if (!m_bPause) {
			m_nSecond++;
		}

		int h = m_nSecond / 3600;
		int m = (m_nSecond % 3600) / 60;
		int s = m_nSecond % 60;
		CString strTime;
		strTime.Format(_T("已用时长\n%02d:%02d:%02d"), h, m, s);
		SetDlgItemText(IDC_STATIC_TEXT, strTime);

	}
	else {
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
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CGame::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_POINT, &CGame::OnBnClickedButtonPoint)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CGame::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_GAMESETTING, &CGame::OnBnClickedButtonGamesetting)
	ON_BN_CLICKED(IDC_BUTTON_BACKMAIN, &CGame::OnBnClickedButtonBackmain)
END_MESSAGE_MAP()


// CGame 消息处理程序

BOOL CGame::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitBackground();

	InitElement();
	
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_POINT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
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
    // 从原始背景 DC 恢复游戏区域的背景，以便删除的元素被清除，然后绘制当前地图
	m_dcMem.BitBlt(
		m_rtGameRect.left, m_rtGameRect.top,
		m_rtGameRect.Width(), m_rtGameRect.Height(),
		&m_dcBg, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);


    for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			int nInfo = m_gameControl.GetElement(i, j);
			// 跳过标记为 -1 的空格子
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
	// 初始化游戏地图

	m_gameControl.StartGame();

	UpdateMap();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_POINT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	m_bPause = false;
	m_nSecond = 0;
	SetTimer(2, 1000, NULL);  // 设置一个定时器，ID为2，间隔为1000毫秒
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

	if(m_bPause) {
		return CDialogEx::OnLButtonUp(nFlags, point);
	}

	// 计算相对于游戏区域左上角的行/列并验证
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
		// 判断是否为相同图片并获取连接路径
		if (m_gameControl.Link(avPath, pathLen)) {
			// 清除数据并更新地图
			UpdateMap();
			// 在内存DC上画出连线以覆盖背景
			DrawTipLine(avPath, pathLen);
			// 启动定时器，在 200ms 后清除连线（通过再次 UpdateMap 恢复背景）
			// 先确保没有上一次的定时器残留
			KillTimer(ID_LINE_TIMER);
			SetTimer(ID_LINE_TIMER, 200, NULL);
			OverGame();
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
    // 将提示框绘制到内存 DC 中，使其在重绘时保持，然后请求重绘窗口以显示更新后的内存 DC。
	CBrush brush(RGB(233, 43, 43));
	CRect rtTipFrame;
	rtTipFrame.left = m_ptGameTop.x + nCol * m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.y + nRow * m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;

	if (m_dcMem.GetSafeHdc()) {
		m_dcMem.FrameRect(&rtTipFrame, &brush);
		// 请求重绘以将内存 DC 的内容呈现在窗口上
		Invalidate(FALSE);
	} else {
		// 回退：直接在窗口 DC 上绘制
		CClientDC dc(this);
		dc.FrameRect(&rtTipFrame, &brush);
	}
}

bool CGame::IsLink() {
    // m_ptSel*.x 表示列, m_ptSel*.y 表示行。m_anMap 按 [row][col] 索引。
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
		CPen penLine(PS_SOLID, 2, RGB(0, 0, 255));
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

void CGame::OverGame() {
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			if (m_gameControl.m_anMap[i][j] >= 0) {
				return;
			}
		}
	}
	KillTimer(2);
		// 游戏结束处理
	/*MessageBox(L"恭喜通关！", L"提示", MB_OK);*/
	GameInput input;
	input.DoModal();
	RANKTIME.push_back(m_nSecond);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_POINT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(L"已用时长\n00:00:00");
	m_bPause = false;
	m_nSecond = 0;
	

}
void CGame::OnBnClickedButtonBack()
{
	m_bPause = false;
	
	GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(L"暂停游戏");
	GetDlgItem(IDC_BUTTON_POINT)->EnableWindow(TRUE);
	
	GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(L"已用时长\n00:00:00");
	m_bPause = false;
	m_nSecond = 0;
	SetTimer(2, 1000, NULL);  // 设置一个定时器，ID为2，间隔为1000毫秒
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonStart();
}

void CGame::OnBnClickedButtonPoint()
{
	// TODO: 在此添加控件通知处理程序代码


	flag = false;
	pointFlag = true;
	for(int i=0; i < MAP_ROWS; i++) {
		for(int j = 0; j < MAP_COLS; j++) {
			if(m_gameControl.m_anMap[i][j] >= 0) {
				Vertex v = {i, j, m_gameControl.m_anMap[i][j]};
				dfs(v);
				if(flag) return;
			}
		}
	}
	if (!pointFlag) {
		MessageBox(L"当前局面无可连通目标！请重排！", L"提示", MB_OK);
	}
}


void CGame::dfs(Vertex begin)
{
	
	int startVal = m_gameControl.m_anMap[begin.row][begin.col];
	if (startVal == BLANK)
		return;

	CGameLogic logic;
	Vertex avPath[4];
	int pathLen = 0;

	// 遍历所有格子，寻找相同图案且可连通的目标
	for (int r = 0; r < MAP_ROWS; ++r) {
		for (int c = 0; c < MAP_COLS; ++c) {
			// 跳过自身
			if (r == begin.row && c == begin.col) continue;
			// 值必须相等且不为空
			if (m_gameControl.m_anMap[r][c] != startVal) continue;
			Vertex candidate = { r, c, m_gameControl.m_anMap[r][c] };
			// 调用游戏逻辑判断是否可连通
			if (logic.IsLink(m_gameControl.m_anMap, begin, candidate, avPath, pathLen)) {
				// 绘制提示连线（不清除元素）
				flag = true;
				DrawTipLine(avPath, pathLen);
				// 启动短时定时器以便随后清除连线
				KillTimer(ID_LINE_TIMER);
				SetTimer(ID_LINE_TIMER, 500, NULL);
				return;
			}
		}
	}
	// 未找到可连通目标
	pointFlag = false;
	return;
}

void CGame::OnDestroy()
{
	// TODO: 在此处添加实现代码.
	CDialogEx::OnDestroy();
	KillTimer(2);
}

void CGame::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bPause = !m_bPause;
	if (m_bPause) {
		GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(L"继续游戏");
		GetDlgItem(IDC_BUTTON_POINT)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_BUTTON_STOP)->SetWindowText(L"暂停游戏");
		GetDlgItem(IDC_BUTTON_POINT)->EnableWindow(TRUE);
	}
}

void CGame::OnBnClickedButtonGamesetting()
{
	// TODO: 在此添加控件通知处理程序代码
	GameSetting dlg;
	dlg.DoModal();
}



void CGame::OnBnClickedButtonBackmain()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(0);
}
