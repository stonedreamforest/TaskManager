// ServicerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "任务管理器.h"
#include "ServicerDlg.h"
#include "afxdialogex.h"
#include <winsvc.h>
#include <vector>

// ServicerDlg 对话框

IMPLEMENT_DYNAMIC(ServicerDlg, CDialogEx)

ServicerDlg::ServicerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ServicerDlg::IDD, pParent)
{
	isInitEnd=false;
}

ServicerDlg::~ServicerDlg()
{
}

void ServicerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ServiceList);
}


BEGIN_MESSAGE_MAP(ServicerDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// ServicerDlg 消息处理程序


BOOL ServicerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CRect rect;
	m_ServiceList.GetClientRect(rect);
	m_ServiceList.SetExtendedStyle(m_ServiceList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//可选中风格
	CString str[] ={_T(""), _T("名称"), _T("服务类型"), _T("当前类型"), _T("启动类型"),  _T("用户组"), _T("所属组"),_T("路径")};

	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		if (i==7)
		{
			m_ServiceList.InsertColumn(i, str[i], LVCFMT_LEFT, 500);
		}
		else if(i==6)
		{
			m_ServiceList.InsertColumn(i, str[i], LVCFMT_LEFT, 230);
		}
		else if(i==2)
		{
			m_ServiceList.InsertColumn(i, str[i], LVCFMT_LEFT, 210);
		}
		else if(i==4)
		{
			m_ServiceList.InsertColumn(i, str[i], LVCFMT_LEFT, 370);
		}
		else
		{
			m_ServiceList.InsertColumn(i, str[i], LVCFMT_LEFT, 150);
		}

	}
		m_ServiceList.DeleteColumn(0);//删除第0列 用以文字居中

		ServiceIndex();
	isInitEnd=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void ServicerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(!isInitEnd){
		return;
	}
	CRect rc ;
	GetClientRect(&rc);
	rc.DeflateRect(10,0,0,0);
	m_ServiceList.MoveWindow(rc,FALSE) ;
	// TODO: 在此处添加消息处理程序代码
}

TCHAR * ServicerDlg::TheCurrentState(DWORD TheState)
{
	switch (TheState)
	{
	case SERVICE_CONTINUE_PENDING:
		return _T("继续挂起");
	case SERVICE_PAUSE_PENDING:
		return _T("暂停挂起");
	case SERVICE_PAUSED:
		return _T("暂停");
	case SERVICE_RUNNING:
		return _T("正在运行");
	case SERVICE_START_PENDING:
		return _T("正在启动");
	case SERVICE_STOP_PENDING:
		return _T("正在停止");
	default:
		break;
	}
	return _T("未运行");
}

TCHAR * ServicerDlg::TheServiceType(DWORD TheType)
{
	switch (TheType)
	{
	case SERVICE_FILE_SYSTEM_DRIVER:
		return _T("文件系统驱动程序");
	case SERVICE_KERNEL_DRIVER:
		return _T("设备驱动程序");
	case SERVICE_WIN32_OWN_PROCESS:
		return _T("该服务运行在它自己的过程中");
	default:
		break;
	}
	return _T("该服务与其他服务共享一个进程");
}

TCHAR * ServicerDlg::TheStartType(DWORD TheType)
{
	switch (TheType)
	{
	case SERVICE_AUTO_START:
		return _T("服务控制管理器在系统启动时自动启动服务");
	case SERVICE_BOOT_START:
		return _T("由系统装载程序启动的设备驱动程序,此值仅用于驱动程序服务");
	case SERVICE_DEMAND_START:
		return _T("服务开始由服务控制管理器,当一个进程调用startService函数");
	case SERVICE_DISABLED:
		return _T("不能启动的服务.尝试启动服务,导致错误的代码error_service_disabled");
	default:
		break;
	}
	return _T("由ioinitsystem功能启动设备驱动程序。此值仅用于驱动程序服务");
}


void ServicerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow==FALSE)
	{
		return;
	}
	ServiceIndex();//切换窗口刷新
	// TODO: 在此处添加消息处理程序代码
}

void ServicerDlg::ServiceIndex()
{
	SC_HANDLE hSCM=OpenSCManager(NULL,NULL,SC_MANAGER_ENUMERATE_SERVICE);
	DWORD dwServiceNum=0;
	DWORD dwSize=0;
	EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,//所有服务状态
		NULL,0,&dwSize,&dwServiceNum,
		NULL,NULL);
	//申请需要的内存 第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice=(LPENUM_SERVICE_STATUS_PROCESS)LocalAlloc(LPTR,dwSize);
	//第二次枚举
	BOOL bStattus=EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,//所有服务状态
		(PBYTE)pEnumSerice,dwSize,&dwSize,&dwServiceNum,
		NULL,NULL);
	//遍历信息
	for (DWORD i=0;i<dwServiceNum;i++)
	{
		//获取基础信息

		//	pEnumSerice[i].lpDisplayName;
		//	pEnumSerice[i].lpServiceName;//服务名
		//pEnumSerice[i].ServiceStatusProcess.dwCurrentState;//服务状态
		//pEnumSerice[i].ServiceStatusProcess.dwServiceType;//服务可执行类型
		TCHAR *ServiceType=TheServiceType(pEnumSerice[i].ServiceStatusProcess.dwServiceType);

		//	_stprintf_s(ServiceType,_T("%d"),);
		//获取当前类型
		TCHAR* CurrentState=TheCurrentState(pEnumSerice[i].ServiceStatusProcess.dwCurrentState);
		//	_stprintf_s(CurrentState,_T("%d"),);
		m_ServiceList.InsertItem(0,pEnumSerice[i].lpServiceName);
		m_ServiceList.SetItemText(0,1,ServiceType);
		m_ServiceList.SetItemText(0,2,CurrentState);
		//获取单个服务详细信息
		SC_HANDLE hService=OpenService(hSCM,pEnumSerice[i].lpServiceName,SERVICE_QUERY_CONFIG);

		//第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService,NULL,0,&dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig=(LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR,dwSize);
		//第二次调用获取信息
		QueryServiceConfig(hService,pServiceConfig,dwSize,&dwSize);
		//通过上面获取到的结构体信息具体到想要的值

		TCHAR *StartType=TheStartType(pServiceConfig->dwStartType);;//获取启动类型
		//_stprintf_s(StartType,_T("%d"),);
		//	_stprintf_s(StartType,_T("%d"),pServiceConfig->dwStartType);
		m_ServiceList.SetItemText(0,3,StartType);
		m_ServiceList.SetItemText(0,4,pServiceConfig->lpLoadOrderGroup);
		m_ServiceList.SetItemText(0,5,pServiceConfig->lpServiceStartName);
		m_ServiceList.SetItemText(0,6,pServiceConfig->lpBinaryPathName);


		m_VecServiceList.push_back(pEnumSerice+i);
	}
}
