// showexe.cpp : 实现文件
//

#include "stdafx.h"
#include "任务管理器.h"
#include "showexe.h"
#include "afxdialogex.h"


// showexe 对话框

IMPLEMENT_DYNAMIC(showexe, CDialogEx)
showexe* showexe::m_myThis = NULL;
showexe::showexe(CWnd* pParent /*=NULL*/)
	: CDialogEx(showexe::IDD, pParent)
{
	isInitEnd = FALSE;
	m_myThis = this;
}

showexe::~showexe()
{
}

void showexe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, Exelist);
}


BEGIN_MESSAGE_MAP(showexe, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()



_TCHAR str[MAX_PATH];
_TCHAR str1[MAX_PATH];
BOOL CALLBACK showexe::PROC1(HWND hwnd,LPARAM lparam)	
{
	
 	if (!::IsWindowVisible(hwnd))
 	{
 		return TRUE;
 	}
 	if (!::GetWindowText(hwnd,str,MAX_PATH))
 	{
 		return TRUE;
 	}
 	m_myThis->Exelist.InsertItem(0,str);//pmc.WorkingSetSize
 	m_myThis->Exelist.SetItemText(0,1,L"正在运行");
 	//_tprintf(L"%s\n",str);
 	//GetClassName(hwnd,str1,MAX_PATH);
	return TRUE;
 }	
// showexe 消息处理程序


BOOL showexe::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect;
	Exelist.GetClientRect(rect);
	Exelist.SetExtendedStyle(Exelist.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//可选中风格
	CString str[] ={ _T("程序名"), _T("状态")};
	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		Exelist.InsertColumn(i, str[i], LVCFMT_LEFT, rect.Width()/2);
	}
//	Exelist.DeleteColumn(0);//删除第0列 用以文字居中
	EnumWindows(PROC1,0);

	m_myThis->isInitEnd=TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void showexe::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(!isInitEnd){
		return;
	}
	CRect rc ;
	GetClientRect(&rc);
	rc.DeflateRect(10,0,0,0);
	Exelist.MoveWindow(rc,FALSE) ;
	// TODO: 在此处添加消息处理程序代码
}
