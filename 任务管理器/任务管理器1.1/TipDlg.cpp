// TipDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "任务管理器.h"
#include "TipDlg.h"
#include "afxdialogex.h"


// TipDlg 对话框

IMPLEMENT_DYNAMIC(TipDlg, CDialogEx)

TipDlg::TipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(TipDlg::IDD, pParent)
{

}

TipDlg::~TipDlg()
{
}

void TipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TipDlg, CDialogEx)
END_MESSAGE_MAP()


// TipDlg 消息处理程序
