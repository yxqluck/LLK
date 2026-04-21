// GameInput.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "GameInput.h"
#include "global.h"


std::vector<CString> RANK;
std::vector<int> RANKTIME;

// GameInput 对话框

IMPLEMENT_DYNAMIC(GameInput, CDialogEx)

GameInput::GameInput(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INPUT, pParent)
{

}

GameInput::~GameInput()
{
}

void GameInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GameInput, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &GameInput::OnBnClickedButtonBack)
END_MESSAGE_MAP()


// GameInput 消息处理程序

void GameInput::OnBnClickedButtonBack()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_EDIT_INPUT, str);
	RANK.push_back(str);
	EndDialog(0);
}
