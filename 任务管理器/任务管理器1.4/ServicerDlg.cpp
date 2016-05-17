// ServicerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "任务管理器.h"
#include "ServicerDlg.h"
#include "afxdialogex.h"


// ServicerDlg 对话框

IMPLEMENT_DYNAMIC(ServicerDlg, CDialogEx)

ServicerDlg::ServicerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ServicerDlg::IDD, pParent)
{

}

ServicerDlg::~ServicerDlg()
{
}

void ServicerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ServicerDlg, CDialogEx)
END_MESSAGE_MAP()


// ServicerDlg 消息处理程序
