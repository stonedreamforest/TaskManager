
// 任务管理器Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "任务管理器.h"
#include "任务管理器Dlg.h"
#include "afxdialogex.h"
#include "ProcessDlg.h"
//#include "NetDlg.h"
#include <PowrProf.h>
#include "showexe.h"
#include "ServicerDlg.h"
#pragma comment(lib,"PowrProf.lib")
#include <afxwin2.inl>
#include "../WH_Keybroad/stdafx.h"
#pragma comment(lib, "../Debug/hh.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_LIST1, m_ProList);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C任务管理器Dlg 对话框



C任务管理器Dlg::C任务管理器Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C任务管理器Dlg::IDD, pParent)
	, m_IsWindowHide(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_IsWindowHide=false;
}

void C任务管理器Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
//	DDX_Control(pDX, IDC_STATIC1, m_StaticText);
}

BEGIN_MESSAGE_MAP(C任务管理器Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &C任务管理器Dlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON3, &C任务管理器Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &C任务管理器Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C任务管理器Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &C任务管理器Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C任务管理器Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &C任务管理器Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &C任务管理器Dlg::OnBnClickedButton7)
	ON_WM_HOTKEY()
	//ON_UPDATE_COMMAND_UI(AFX_IDS_UPDATE_MENU, &C任务管理器Dlg::OnUpdateAfxIdsUpdateMenu)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &C任务管理器Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &C任务管理器Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &C任务管理器Dlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// C任务管理器Dlg 消息处理程序

BOOL C任务管理器Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	::RegisterHotKey(this->GetSafeHwnd(),0xa001,MOD_CONTROL|MOD_SHIFT,'P');//注册热键 
	m_Tab.InsertItem(0,_T("显示程序"));
	m_Tab.InsertItem(1,_T("进程管理"));
	m_Tab.InsertItem(2,_T("服务"));
	m_Tab.addDlg(3,IDD_DIALOG5,new showexe, IDD_DIALOG2, new ProcessDlg,IDD_DIALOG8,new ServicerDlg);
	m_Tab.setshow(0);
	
	SetTimer(0x4,1000,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C任务管理器Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

//void C任务管理器Dlg::OnPaint()
//{
//	
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // 用于绘制的设备上下文
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// 使图标在工作区矩形中居中
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// 绘制图标
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialogEx::OnPaint();
//	}
//}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C任务管理器Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C任务管理器Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void C任务管理器Dlg::AccessPrivileges()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = {0};
	TOKEN_PRIVILEGES stToken_Privileges = {0};
	hProcess = GetCurrentProcess();  //获取当前应用程序进程句柄
	if(!OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken))//打开进程访问令牌
	{
		return;
	}
	if (!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&uID))//获取系统权限的特权值
	{
		return;
	}
	stToken_Privileges.PrivilegeCount=1;////欲调整的权限个数
	stToken_Privileges.Privileges[0].Luid=uID; //权限的LUID
	stToken_Privileges.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;//权限的属性,SE_PRIVILEGE_ENABLED为使能该权限
	if (!AdjustTokenPrivileges(hToken,FALSE,&stToken_Privileges,sizeof(stToken_Privileges),NULL,NULL))
	{
		return;
	}
	CloseHandle(hToken);
	return;
}



void C任务管理器Dlg::OnBnClickedButton3()//关机
{
	AccessPrivileges();
	ExitWindowsEx(EWX_SHUTDOWN,0);
	// TODO: 在此添加控件通知处理程序代码
}


void C任务管理器Dlg::OnBnClickedButton1()//注销
{
	// TODO: 在此添加控件通知处理程序代码
	AccessPrivileges();
	ExitWindowsEx(EWX_LOGOFF,0);
}



void C任务管理器Dlg::OnBnClickedButton2()//睡眠
{
	// TODO: 在此添加控件通知处理程序代码
	AccessPrivileges();
	SetSuspendState(TRUE,FALSE,FALSE);
}


void C任务管理器Dlg::OnBnClickedButton4()//重启
{
	// TODO: 在此添加控件通知处理程序代码
	AccessPrivileges();
	ExitWindowsEx(EWX_REBOOT, 0); //重启
}


void C任务管理器Dlg::OnBnClickedButton5()//强制注销 可能会丢失数据
{
	AccessPrivileges();
	ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);
	// TODO: 在此添加控件通知处理程序代码
}


void C任务管理器Dlg::OnBnClickedButton6()//强制重启  可能会丢失数据
{
	AccessPrivileges();
	// TODO: 在此添加控件通知处理程序代码
	ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
}


void C任务管理器Dlg::OnBnClickedButton7()//强制关机  可能会丢失数据
{
	// TODO: 在此添加控件通知处理程序代码
	AccessPrivileges();
	ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0);
}


// BOOL C任务管理器Dlg::PreTranslateMessage(MSG* pMsg)
// {
// 	// TODO: 在此添加专用代码和/或调用基类
// 	
// 	if ((pMsg->message==WM_HOTKEY)&&(pMsg->wParam==0xa001))
// 	{
// 		//隐藏窗口
// 		if (m_IsWindowHide==true)
// 		{
// 			ShowWindow(SW_HIDE);
// 			m_IsWindowHide=false;
// 		}
// 		else
// 		{
// 			ShowWindow(SW_SHOW);
// 			m_IsWindowHide=true;
// 		}
// 	}
// 	return CDialogEx::PreTranslateMessage(pMsg);
// }


void C任务管理器Dlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHotKeyId==0xa001)
	{
		//隐藏窗口
		if (m_IsWindowHide==true)
		{
			ShowWindow(SW_SHOW);
			m_IsWindowHide=false;
		}
		else
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide=true;
		}
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}



//void C任务管理器Dlg::OnUpdateAfxIdsUpdateMenu(CCmdUI *pCmdUI)
//{
//	// TODO: 在此添加命令更新用户界面处理程序代码
//
//}


BOOL C任务管理器Dlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::PreCreateWindow(cs);
}
//__int64 C任务管理器Dlg::CompareFileTime ( FILETIME time1, FILETIME time2 )//转换为整数时间
//{
//	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;
//	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime ;
//
//	return   (b - a);
//}
//TCHAR strCpu[MAX_PATH];
//void C任务管理器Dlg::CpuOccupation()
//{
//	HANDLE hEvent;
//	BOOL res ;
//
//	FILETIME preidleTime;
//	FILETIME prekernelTime;		//不做了 没有更好的方法解决 卡顿  
//	FILETIME preuserTime;
//
//	FILETIME idleTime;
//	FILETIME kernelTime;
//	FILETIME userTime;
//
//	res = GetSystemTimes( &idleTime, &kernelTime, &userTime );
//
//	preidleTime = idleTime;
//	prekernelTime = kernelTime;
//	preuserTime = userTime ;
//
//	hEvent = CreateEvent (NULL,FALSE,FALSE,NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
//
//	//WaitForSingleObject( hEvent,1000 ); //等待
//	res = GetSystemTimes( &idleTime, &kernelTime, &userTime );
//
//	int idle = CompareFileTime( preidleTime,idleTime);
//	int kernel = CompareFileTime( prekernelTime, kernelTime);
//	int user = CompareFileTime(preuserTime, userTime);
//
//	float cpu = (kernel +user - idle) *100.0/(kernel+user);
//	float cpuidle = ( idle) *100.0/(kernel+user);
//	_stprintf_s(strCpu,_T("CPU利用率%d"),cpu);
//	//	cout << "CPU利用率:" << cpu << "%" << "      CPU空闲率:" <<cpuidle << "%" <<endl;
//	m_StaticText.SetWindowText(strCpu);
//
//	preidleTime = idleTime;
//	prekernelTime = kernelTime;
//	preuserTime = userTime ;
//
//}
void C任务管理器Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnTimer(nIDEvent);
//	CpuOccupation();
}


bool ishookkey=false;
DWORD ThreadProc(LPVOID lpAddr)
{
	if (InstallHook())
	{
		while (ishookkey)
		{
			
		}
		//system("pause");
		
	}

	return 0;
}

void C任务管理器Dlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	ishookkey=true;
	CreateThread(0,0,(LPTHREAD_START_ROUTINE)ThreadProc,0,0,0);
}


void C任务管理器Dlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	ishookkey=false;
	UninstallHook();
}


void C任务管理器Dlg::OnBnClickedButton10()
{
	
	CPoint   point;
	GetCursorPos(&point);
	CRect rect(point.x,point.y,point.x,point.y);
	ClipCursor(rect);
}
