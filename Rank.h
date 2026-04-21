#pragma once
#include "afxdialogex.h"


// Rank 对话框

class Rank : public CDialogEx
{
	DECLARE_DYNAMIC(Rank)

public:
	Rank(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Rank();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RANK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listTable;
	virtual BOOL OnInitDialog();
	void AddTable();
};
