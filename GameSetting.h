#pragma once
#include "afxdialogex.h"


// GameSetting 对话框

class GameSetting : public CDialogEx
{
	DECLARE_DYNAMIC(GameSetting)

public:
	GameSetting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~GameSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
};
