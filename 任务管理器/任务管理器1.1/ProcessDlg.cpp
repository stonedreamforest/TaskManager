// ProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "任务管理器.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <tlhelp32.h>
#include "Psapi.h"
#include "TipDlg.h"
#pragma comment(lib,"Psapi.lib")
// ProcessDlg 对话框

IMPLEMENT_DYNAMIC(ProcessDlg, CDialogEx)

ProcessDlg::ProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ProcessDlg::IDD, pParent)
{
	isInitEnd = FALSE;
}

ProcessDlg::~ProcessDlg()
{
}

void ProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProList);
}


BEGIN_MESSAGE_MAP(ProcessDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID__32778, &ProcessDlg::KillProcess)
//	ON_WM_KEYDOWN()
ON_COMMAND(ID_32782, &ProcessDlg::RealTime)
ON_COMMAND(ID_32783, &ProcessDlg::high)
ON_COMMAND(ID_32784, &ProcessDlg::AboveNormal)
ON_COMMAND(ID_32785, &ProcessDlg::normal)
ON_COMMAND(ID_32786, &ProcessDlg::BelowNormal)
ON_COMMAND(ID_32787, &ProcessDlg::low)
END_MESSAGE_MAP()

//全局变量
HANDLE hProessSnap;
HANDLE hProcess;
PROCESSENTRY32 stcpe32={0};
PROCESS_MEMORY_COUNTERS pmc;//内存占用函数需要
TCHAR Pid[MAX_PATH];
TCHAR Memory[MAX_PATH];
TCHAR CpuUtilization[MAX_PATH];//cpu使用率
TCHAR Priorit[MAX_PATH]={0}; //进程优先级
MEMORYSTATUS memStatus;
//进程索引
void ProcessDlg::ProcessIndex()
{
	
	m_VecProList.clear();
	m_ProList.DeleteAllItems();
	isInitEnd = true;
	double cpu;
	stcpe32.dwSize=sizeof(PROCESSENTRY32);

	//创建一个进程相关的快照句柄
	hProessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProessSnap==INVALID_HANDLE_VALUE)
	{
		return ;
	}
	if (!Process32FirstW(hProessSnap,&stcpe32))
	{
		CloseHandle(hProessSnap);		//获取系统进程
		return ;
	}
	do 
	{
		PROCESSENTRY32	stcInfo={sizeof(PROCESSENTRY32)};
		//获取进程映像路径
		wcscpy_s(stcInfo.szExeFile,stcpe32.szExeFile);
		hProcess=OpenProcess(
			PROCESS_QUERY_INFORMATION|PROCESS_VM_READ ,
			FALSE,stcpe32.th32ProcessID);
		if (hProcess)
		{
			stcInfo.pcPriClassBase=GetPriorityClass(hProcess);

		}
		GetProcessMemoryInfo(hProcess,&pmc,sizeof(pmc));//获得工作集 workingsetsize
		cpu=Get_cpu_usage(stcpe32.th32ProcessID);//获取cpu占有率
		stcInfo.th32ProcessID=stcpe32.th32ProcessID;
		stcInfo.cntThreads=stcpe32.cntThreads;
		stcInfo.th32ParentProcessID=stcpe32.th32ParentProcessID;
		m_ProList.InsertItem(0,stcpe32.szExeFile);
		_stprintf_s(Pid,_T("%d"),stcpe32.th32ProcessID);///PID
		_stprintf_s(Memory,_T("%d %s"),pmc.WorkingSetSize/1024,_T("k"));
		_stprintf_s(CpuUtilization,_T("%d"),cpu);
		m_ProList.SetItemText(0,1,Pid);//插入PID
		m_ProList.SetItemText(0,2,Memory);//插入工作集
		m_ProList.SetItemText(0,3,CpuUtilization);//插入cpu占用率
		m_ProList.SetItemText(0,4,InitializePriority(stcInfo.pcPriClassBase));//
		//将获取的相关信息保存到向量中
		m_VecProList.push_back(stcInfo);
	} while (Process32Next(hProessSnap,&stcpe32));

	CloseHandle(hProcess);
	CloseHandle(hProessSnap);	
};
// ProcessDlg 消息处理程序


BOOL ProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect;
	m_ProList.GetClientRect(rect);
	m_ProList.SetExtendedStyle(m_ProList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//可选中风格
	CString str[] ={_T(""), _T("进程"), _T("PID"), _T("WorkingSetSize"), _T("CPU"),  _T("优先级"),_T("描述")};
	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		m_ProList.InsertColumn(i, str[i], LVCFMT_CENTER, 121);
	}
	m_ProList.DeleteColumn(0);//删除第0列 用以文字居中
	ProcessIndex();
//	m_ProList.InsertItem(0,L"123");
 //	m_ProList.SetItemText(0,1,_T("123"));
 //	m_ProList.SetItemText(0,2,_T("123"));


	SetTimer(0x1,1000,NULL);//实时刷新 每1000毫秒
	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}


void ProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(!isInitEnd){
		return;
	}
	CRect rc ;
	GetClientRect(&rc);
	rc.DeflateRect(10,0,0,0);
	ProcessIndex();//切换窗口刷新
	m_ProList.MoveWindow(rc,FALSE) ;
	
}


void ProcessDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow==FALSE)
	{
		return;
	}
}


void ProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RefreshProcess();//刷新
}

void ProcessDlg::RefreshProcess()
{
	int OriginalValue=m_VecProList.size();//原值  即刷新前的值
	m_NewVecProList.clear();
	int CountFlag=0;
	double cpu;
	stcpe32.dwSize=sizeof(PROCESSENTRY32);
	//创建一个进程相关的快照句柄
	hProessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProessSnap==INVALID_HANDLE_VALUE)
	{
		return ;
	}
	if (!Process32FirstW(hProessSnap,&stcpe32))
	{
		CloseHandle(hProessSnap);		//获取系统进程
		return ;
	}
	do 
	{
		PROCESSENTRY32	stcInfo={sizeof(PROCESSENTRY32)};
		//获取进程映像路径
		wcscpy_s(stcInfo.szExeFile,stcpe32.szExeFile);
		hProcess=OpenProcess(
			PROCESS_QUERY_INFORMATION|PROCESS_VM_READ ,
			FALSE,stcpe32.th32ProcessID);
		if (hProcess)
		{
			stcInfo.pcPriClassBase=GetPriorityClass(hProcess);
		}
		GetProcessMemoryInfo(hProcess,&pmc,sizeof(pmc));//获得工作集 workingsetsize
		cpu=Get_cpu_usage(stcpe32.th32ProcessID);//获取cpu占有率
		stcInfo.th32ProcessID=stcpe32.th32ProcessID;
		stcInfo.cntThreads=stcpe32.cntThreads;
		stcInfo.th32ParentProcessID=stcpe32.th32ParentProcessID;
		//	m_ProList.InsertItem(0,stcpe32.szExeFile);
		//必须在同一循环比较 较为恰当  多线程的原理还没弄大清楚
		if (CountFlag<OriginalValue)
		{
			if (m_VecProList[CountFlag].th32ProcessID!=stcpe32.th32ProcessID)//上次列表中同样序号上的进程不同时，删除
			{
				int num=m_VecProList.size();//可删
				m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
				m_VecProList.erase(m_VecProList.begin()+CountFlag);//被删除位置的下一个会到被删除的位置上
				UINT NowPos=CountFlag;//目前对比位置 多1 指向下一个PID进行对比
				for (;NowPos<m_VecProList.size()-1;NowPos++)
				{
					if (m_VecProList[NowPos].th32ProcessID!=stcpe32.th32ProcessID)
					{
						m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
						m_VecProList.erase(m_VecProList.begin()+CountFlag);
						NowPos--;//保持值不变  即位置不变
					}
					else
					{
						//CountFlag--
						break;
					}
				}
			}
			CountFlag++;	
		}
		else
		{
			m_ProList.InsertItem(0,stcpe32.szExeFile);
			_stprintf_s(Pid,_T("%d"),stcpe32.th32ProcessID);///PID
			_stprintf_s(Memory,_T("%d %s"),pmc.WorkingSetSize/1024,_T("K"));
			m_ProList.SetItemText(0,1,Pid);//pmc.WorkingSetSize
			m_ProList.SetItemText(0,2,Memory);
			m_ProList.SetItemText(0,3,CpuUtilization);//插入cpu占用率
			m_ProList.SetItemText(0,4,InitializePriority(stcInfo.pcPriClassBase));//

		}

		//将获取的相关信息保存到向量中
		m_NewVecProList.push_back(stcInfo);
	} while (Process32Next(hProessSnap,&stcpe32));
	//比完之后发现新的进程列表比原来的少
	if (CountFlag<OriginalValue)
	{
		for (UINT i=CountFlag;i<m_VecProList.size();i++)
		{
			m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);//删除该列
			m_VecProList.erase(m_VecProList.begin()+CountFlag);//vec列表清除该列
		}
	}
	CloseHandle(hProcess);
	CloseHandle(hProessSnap);	
	m_VecProList=m_NewVecProList;
}


void ProcessDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{

	


	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	m_ProList.GetClientRect(&rect);//可点击范围  
	CPoint p;

	//GetPhysicalCursorPos(&p);//鼠标坐标
	GetCursorPos(&p);
	rect.DeflateRect(0,23,0,0);
	m_ProList.ClientToScreen(rect);//基于屏幕坐标
	if (rect.PtInRect(p))//判断是否在客户区listctrl范围内
 	{
		CMenu Menu;
		Menu.LoadMenuW(IDR_MENU1);//菜单资源
		CMenu *SubMenu=Menu.GetSubMenu(0);
		SubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON,p.x,p.y,this);
		//SubMenu->Detach();
		//Menu.DestroyMenu();//销毁
	}

}


void ProcessDlg::KillProcess()
{
	HANDLE hProcessHandle;

	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	if (pos == NULL)//判断是否为空
	{
		return;
	}
	else
	{
		//可按ctrl+左键选中多行结束进程
		while (pos)
		{
			int nItem = m_ProList.GetNextSelectedItem(pos);
			m_ProList.DeleteItem(nItem);
			hProcessHandle = OpenProcess( PROCESS_TERMINATE, FALSE, 
				m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID);
			TerminateProcess( hProcessHandle, 4 );//
		}
	}
	// TODO: 在此添加命令处理程序代码
}

int ProcessDlg::GetCpuNumber()
{
	SYSTEM_INFO info;  
	GetSystemInfo(&info);  //得到系统信息
	return (int)info.dwNumberOfProcessors;  //返回处理器数目
}

int ProcessDlg::GetProcessTime(const FILETIME& time)
{
	ULARGE_INTEGER tt;  
	tt.LowPart = time.dwLowDateTime;  //文件低32位时间
	tt.HighPart = time.dwHighDateTime;  //文件高32位时间
	return (int)(tt.QuadPart);  
}

double ProcessDlg::Get_cpu_usage(DWORD PropPid)
{
	static int CpuNum = GetCpuNumber();//获得cpu数量 

	static int last_time_ = 0;  
	static int last_system_time_ = 0;  
	FILETIME now_time;  
	FILETIME creation_time;  
	FILETIME exit_time;  
	FILETIME kernel_time;  
	FILETIME user_time;  
	int system_time;
	int time;
	double cpu=-1;
	GetSystemTimeAsFileTime(&now_time);  //获取当前系统时间
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, PropPid);  //获取句柄
	if (!hProcess)  
	{  
		return -1;  
	} 
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  //获取进程时间
	{  
		return -1;  
	} 
	system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPU使用时间  
	time = GetProcessTime(now_time);        //现在的时间  

	last_system_time_ = system_time;  
	last_time_ = time;  
	CloseHandle( hProcess );  
	//创建内核对象  并且等待1毫秒
	/*HANDLE hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);      不要了  会降低启动及更新速度   
	WaitForSingleObject(hEvent,1);*/

	//第二次获取处理器时间
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, PropPid);  
	if (!hProcess)  
	{  
		return -1;  
	}  
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  
	{  
		return -1;  
	}  
	GetSystemTimeAsFileTime(&now_time);  
	system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPU使用时间  
	time = GetProcessTime(now_time);        //现在的时间  

	CloseHandle( hProcess );  //关闭句柄
	cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;

	return cpu;
}


//void ProcessDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	return;
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}


BOOL ProcessDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->wParam==VK_RETURN)
	{
		//Do anything what you want to
		return true;//阻塞enter消息响应
	}
	if(pMsg->wParam == VK_ESCAPE)  
	{  
		return true;  //阻塞esc消息响应
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void ProcessDlg::RealTime()//优先级实时
{
	// TODO: 在此添加命令处理程序代码
	//SetPriorityClass()
	SetProcessPriority(1);
	
}
void ProcessDlg::SetProcessPriority(int PriorityFlag)
{
	HANDLE hProcessHandle;
	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	int nItem = m_ProList.GetNextSelectedItem(pos);
	hProcessHandle = OpenProcess(PROCESS_TERMINATE, FALSE, 
		m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID);
	//GetMenu()->GetSubMenu(1)->CheckMenuItem(ID_32782,MF_BYCOMMAND | MF_CHECKED);
	TipDlg dlg;
	if (dlg.DoModal()==IDCANCEL)//取消不设置
	{
		return;
	}
//	CMenu* hMenu = GetMenu();
	switch (PriorityFlag)
	{       //可以用现成或自定义宏的 我省虐了。。。。。虐了。。。
	case 1:
		
		//CheckMenuItem (hMenu, ID_32782, MF_UNCHECKED);
	//	hMenu->CheckMenuItem (ID_32782, MF_CHECKED);
		//MessageBox(0,0,0);
		SetPriorityClass(hProcessHandle,REALTIME_PRIORITY_CLASS);//实时		
		break;
	case 2:
		SetPriorityClass(hProcessHandle,HIGH_PRIORITY_CLASS);//高
		break;
	case 3:
		SetPriorityClass(hProcessHandle,ABOVE_NORMAL_PRIORITY_CLASS);//高于标准
		break;
	case 4:
		SetPriorityClass(hProcessHandle,NORMAL_PRIORITY_CLASS);//标准
		break;
	case 5:
		SetPriorityClass(hProcessHandle,BELOW_NORMAL_PRIORITY_CLASS);//低于标准
		break;
	case 6:
		SetPriorityClass(hProcessHandle,IDLE_PRIORITY_CLASS);//低
		break;
	default:
		break;
	}
}



void ProcessDlg::high()
{
	// TODO: 在此添加命令处理程序代码
	SetProcessPriority(2);
}


void ProcessDlg::AboveNormal()
{
	// TODO: 在此添加命令处理程序代码
	SetProcessPriority(3);
}


void ProcessDlg::normal()
{
	// TODO: 在此添加命令处理程序代码
	SetProcessPriority(4);
}


void ProcessDlg::BelowNormal()
{
	// TODO: 在此添加命令处理程序代码
	SetProcessPriority(5);
}



void ProcessDlg::low()
{
	// TODO: 在此添加命令处理程序代码
	SetProcessPriority(6);
}

TCHAR* ProcessDlg::InitializePriority(DWORD dwPriority)//初始化进程优先级
{

	
	// 发出消息，为用户描述该类 
	switch(dwPriority)
	{
	case REALTIME_PRIORITY_CLASS:
		return _T("实时");
	case HIGH_PRIORITY_CLASS:
		return _T("高");
	case ABOVE_NORMAL_PRIORITY_CLASS:
		return _T("高于标准");
	case NORMAL_PRIORITY_CLASS:
		return _T("标准");
	case BELOW_NORMAL_PRIORITY_CLASS:
		return _T("低于标准");
	case IDLE_PRIORITY_CLASS:
		return _T("低");
	default:
		break;
	}
	return _T("拒绝访问");;
}
