// Rank.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "Rank.h"
#include "global.h"
#include<fstream>


// Rank 对话框

IMPLEMENT_DYNAMIC(Rank, CDialogEx)

Rank::Rank(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RANK, pParent)
{

}

Rank::~Rank()
{
}

void Rank::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, m_listTable);
}


BEGIN_MESSAGE_MAP(Rank, CDialogEx)
END_MESSAGE_MAP()


// Rank 消息处理程序

BOOL Rank::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_listTable.InsertColumn(0, _T("排名"), LVCFMT_CENTER, 80);
	m_listTable.InsertColumn(1, _T("玩家"), LVCFMT_CENTER, 120);
	m_listTable.InsertColumn(2, _T("用时"), LVCFMT_CENTER, 120);
	
	AddTable();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void Rank::AddTable() {
	int nCount = RANK.size();
	//排序
	for(int i = 0; i < nCount - 1; i++) {
		for(int j = 0; j < nCount - i - 1; j++) {
			if(RANKTIME[j] > RANKTIME[j + 1]) {
				std::swap(RANKTIME[j], RANKTIME[j + 1]);
				std::swap(RANK[j], RANK[j + 1]);
			}
		}
	}
	CStdioFile file(_T("rank.txt"), CFile::modeCreate | CFile::modeWrite);

	//加入表
	for(int i = 0; i < nCount; i++) {
		CString str;
		str.Format(_T("%d"), i + 1);
		m_listTable.InsertItem(i, str);
		m_listTable.SetItemText(i, 1, RANK[i]);
		int h = RANKTIME[i] / 3600;
		int m = (RANKTIME[i] % 3600) / 60;
		int s = RANKTIME[i] % 60;
		str.Format(_T("%02d:%02d:%02d"), h, m, s);
		m_listTable.SetItemText(i, 2, str);
		
		CString line;
		line.Format(_T("%s %d"),RANK[i], RANKTIME[i]);
		
		file.WriteString(line + _T("\n"));

	}

	file.Close();
}