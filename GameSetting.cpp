// GameSetting.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "afxdialogex.h"
#include "GameSetting.h"
#include "CGame.h"


// GameSetting 对话框

IMPLEMENT_DYNAMIC(GameSetting, CDialogEx)

GameSetting::GameSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent)
{

}

GameSetting::~GameSetting()
{
}

void GameSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GameSetting, CDialogEx)
	
END_MESSAGE_MAP()


// GameSetting 消息处理程序
