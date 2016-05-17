// NetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "任务管理器.h"
#include "NetDlg.h"
#include "afxdialogex.h"


// NetDlg 对话框

IMPLEMENT_DYNAMIC(NetDlg, CDialogEx)

NetDlg::NetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NetDlg::IDD, pParent)
{

}

NetDlg::~NetDlg()
{
}

void NetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NetDlg, CDialogEx)
END_MESSAGE_MAP()


// NetDlg 消息处理程序
