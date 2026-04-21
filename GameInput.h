#pragma once
#include "afxdialogex.h"


// GameInput 对话框

class GameInput : public CDialogEx
{
	DECLARE_DYNAMIC(GameInput)

public:
	GameInput(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~GameInput();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBack();
};
