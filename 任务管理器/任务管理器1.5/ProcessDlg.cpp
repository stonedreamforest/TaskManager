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
#include <shellapi.h>

// ProcessDlg 对话框

IMPLEMENT_DYNAMIC(ProcessDlg, CDialogEx)

ProcessDlg::ProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ProcessDlg::IDD, pParent)
{
	isInitEnd = FALSE;
	isFirstShow=false;
	isFirstModShow=false;
}

ProcessDlg::~ProcessDlg()
{
}

void ProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProList);
//	DDX_Control(pDX, IDC_LIST2, m_ServiceList);
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
ON_COMMAND(ID__32788, &ProcessDlg::SeeThread)
ON_COMMAND(ID__32789, &ProcessDlg::SeeMoudle)
ON_COMMAND(ID__32780, &ProcessDlg::SearchFileInformation)
ON_COMMAND(ID__32781, &ProcessDlg::OpenFilePath)
//ON_COMMAND(ID__32790, &ProcessDlg::CopyFileName)
END_MESSAGE_MAP()

//全局变量
HANDLE hProessSnap;
HANDLE hProcess;
PROCESSENTRY32 stcpe32={0};
PROCESS_MEMORY_COUNTERS pmc;//内存占用函数需要
TCHAR Pid[MAX_PATH];
TCHAR Memory[MAX_PATH];
TCHAR CpuUtilization[MAX_PATH];//cpu使用率
TCHAR ProfilePath[MAX_PATH];//进程路径
TCHAR NewProfilePath[MAX_PATH];//进程路径
TCHAR* Priorit; //进程优先级
//TCHAR TempPriorit[MAX_PATH]={0}; //零时进程优先级
MEMORYSTATUS memStatus;
//进程索引
void ProcessDlg::ProcessIndex()
{
	
	m_VecProList.clear();
	m_VecPriority.clear();
	m_VecMemory.clear();
	m_VevCpu.clear();
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
		GetProcessImageFileName(hProcess,ProfilePath,MAX_PATH);
		GetModuleFileNameEx(hProcess,NULL,ProfilePath,MAX_PATH);//获取文件路径
		
		CString NewProfilePath(ProfilePath);
		NewProfilePath.Replace(_T("\\Device\\HarddiskVolume1\\"),_T("c:\\"));
		NewProfilePath.Replace(_T("\\Device\\HarddiskVolume2\\"),_T("c:\\"));
		stcInfo.th32ProcessID=stcpe32.th32ProcessID;
		stcInfo.cntThreads=stcpe32.cntThreads;
		stcInfo.th32ParentProcessID=stcpe32.th32ParentProcessID;
		m_ProList.InsertItem(0,stcpe32.szExeFile);
		_stprintf_s(Pid,_T("%d"),stcpe32.th32ProcessID);///PID
		_stprintf_s(Memory,_T("%d %s"),pmc.WorkingSetSize/1024,_T("k"));
		_stprintf_s(CpuUtilization,_T("%d"),cpu);
		Priorit=InitializePriority(stcInfo.pcPriClassBase);
		m_ProList.SetItemText(0,1,Pid);//插入PID
		m_ProList.SetItemText(0,2,Memory);//插入工作集
		m_ProList.SetItemText(0,3,CpuUtilization);//插入cpu占用率
		m_ProList.SetItemText(0,4,Priorit);//
		//将获取的相关信息保存到向量中
		m_ProList.SetItemText(0,5,NewProfilePath);
		m_VecProList.push_back(stcInfo);
		m_VecPriority.push_back(Priorit);//保存优先级
		m_VecMemory.push_back(pmc.WorkingSetSize/1024);//保存内存
		m_VevCpu.push_back(cpu);//保存CPU
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
	CString str[] ={_T(""), _T("进程"), _T("PID"), _T("WorkingSetSize"), _T("CPU"),  _T("优先级"),_T("路径")};
	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		if (i==6)
		{
			m_ProList.InsertColumn(i, str[i], LVCFMT_LEFT, 600);
		}
		else
		{
				m_ProList.InsertColumn(i, str[i], LVCFMT_CENTER, 121);
		}
	
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
	m_ProList.MoveWindow(rc,FALSE) ;
	
}


void ProcessDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow==FALSE)
	{
		return;
	}
	ProcessIndex();//切换窗口刷新
}


void ProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RefreshProcess();//刷新
}

void ProcessDlg::RefreshProcess()
{
	int OriginalValue=m_VecProList.size();//原值  即刷新前的值
	int ListPosflag=m_VecProList.size();//列表位置  最先插入的 在LIST最下面 
	m_NewVecProList.clear();
	m_NewVecPriority.clear();
	m_NewVecMemory.clear();
	m_NewVevCpu.clear();
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
		GetProcessImageFileName(hProcess,ProfilePath,MAX_PATH);
		GetModuleFileNameEx(hProcess,NULL,ProfilePath,MAX_PATH);//获取文件路径
		
		CString NewProfilePath(ProfilePath);
		NewProfilePath.Replace(_T("\\Device\\HarddiskVolume1\\"),_T("c:\\"));
		NewProfilePath.Replace(_T("\\Device\\HarddiskVolume2\\"),_T("c:\\"));
		stcInfo.th32ProcessID=stcpe32.th32ProcessID;
		stcInfo.cntThreads=stcpe32.cntThreads;
		stcInfo.th32ParentProcessID=stcpe32.th32ParentProcessID;
		 Priorit=InitializePriority(stcInfo.pcPriClassBase);
		//	m_ProList.InsertItem(0,stcpe32.szExeFile);
		//必须在同一循环比较 较为恰当  多线程的原理还没弄大清楚
		if (CountFlag<OriginalValue)
		{
			if (m_VecProList[CountFlag].th32ProcessID!=stcpe32.th32ProcessID)//上次列表中同样序号上的进程不同时，删除
			{
				int num=m_VecProList.size();//可删
				m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
				m_VecProList.erase(m_VecProList.begin()+CountFlag);//被删除位置的下一个会到被删除的位置上
				m_VecPriority.erase(m_VecPriority.begin()+CountFlag);
				m_VecMemory.erase(m_VecMemory.begin()+CountFlag);
				m_VevCpu.erase(m_VevCpu.begin()+CountFlag);

				UINT NowPos=CountFlag;//目前对比位置 多1 指向下一个PID进行对比
				for (;NowPos<m_VecProList.size()-1;NowPos++)
				{
					if (m_VecProList[NowPos].th32ProcessID!=stcpe32.th32ProcessID)
					{
						m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
						m_VecProList.erase(m_VecProList.begin()+CountFlag);
						m_VecPriority.erase(m_VecPriority.begin()+CountFlag);
						m_VecMemory.erase(m_VecMemory.begin()+CountFlag);
						m_VevCpu.erase(m_VevCpu.begin()+CountFlag);
						NowPos--;//保持值不变  即位置不变
					}
					else
					{
						//CountFlag--
						break;
					}
				}
 				
			}
			if (wcscmp(Priorit,m_VecPriority[CountFlag])!=0)//比较是否刷新优先级
			{
				m_ProList.SetItemText(ListPosflag-1,4,Priorit);
			}
			if (m_VecMemory[CountFlag]!=pmc.WorkingSetSize/1024)
			{
				_stprintf_s(Memory,_T("%d %s"),pmc.WorkingSetSize/1024,_T("K"));
				m_ProList.SetItemText(ListPosflag-1,2,Memory);
			}
			if (cpu!=m_VevCpu[CountFlag])
			{
				/*_stprintf_s(CpuUtilization,_T("%d"),cpu);
				m_ProList.SetItemText(ListPosflag-1,3,CpuUtilization);*/
			}
			ListPosflag--;
			CountFlag++;	
		}
		else
		{
			m_ProList.InsertItem(0,stcpe32.szExeFile);
			_stprintf_s(Pid,_T("%d"),stcpe32.th32ProcessID);///PID
			_stprintf_s(Memory,_T("%d %s"),pmc.WorkingSetSize/1024,_T("K"));
			_stprintf_s(CpuUtilization,_T("%d"),cpu);
			m_ProList.SetItemText(0,1,Pid);//pmc.WorkingSetSize
			m_ProList.SetItemText(0,2,Memory);
			m_ProList.SetItemText(0,3,CpuUtilization);//插入cpu占用率
			m_ProList.SetItemText(0,4,Priorit);//
			m_ProList.SetItemText(0,5,NewProfilePath);//

		}
	//	m_ProList.SetItemText(0,4,_T(" "));//
	//	//
		//将获取的相关信息保存到向量中
		m_NewVecProList.push_back(stcInfo);
		m_NewVecPriority.push_back(Priorit);
		m_NewVecMemory.push_back(pmc.WorkingSetSize/1024);
		m_NewVevCpu.push_back(cpu);
	} while (Process32Next(hProessSnap,&stcpe32));
	//比完之后发现新的进程列表比原来的少
	if (CountFlag<OriginalValue)
	{
		for (UINT i=CountFlag;i<m_VecProList.size();i++)
		{
			m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);//删除该列
			m_VecProList.erase(m_VecProList.begin()+CountFlag);//vec列表清除该列
			m_VecPriority.erase(m_VecPriority.begin()+CountFlag);
			m_VecMemory.erase(m_VecMemory.begin()+CountFlag);
			m_VevCpu.erase(m_VevCpu.begin()+CountFlag);
		}
	}
	CloseHandle(hProcess);
	CloseHandle(hProessSnap);	
	m_VecProList=m_NewVecProList;//重新赋值  依次  进程名/PID  优先级  内存 cpu       
	m_VecPriority= m_NewVecPriority;
	m_VecMemory=m_NewVecMemory;
	m_VevCpu=m_NewVevCpu;
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
			//m_VecProList.erase(m_VecProList.begin()+nItem);//vec列表清除该列
			//m_VecPriority.erase(m_VecPriority.begin()+nItem);
			//m_VecMemory.erase(m_VecMemory.begin()+nItem);
			//m_VevCpu.erase(m_VevCpu.begin()+nItem);
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

__int64 ProcessDlg::FileTimeToInt64(const FILETIME& time)
{
	ULARGE_INTEGER tt;  
	tt.LowPart = time.dwLowDateTime;  //文件低32位时间
	tt.HighPart = time.dwHighDateTime;  //文件高32位时间
	return tt.QuadPart;  
}

double ProcessDlg::Get_cpu_usage(DWORD PropPid)
{
	//static int CpuNum = GetCpuNumber();//获得cpu数量 

	//static __int64 last_time_ = 0;  
	//static __int64 last_system_time_ = 0;  
	//FILETIME now_time;  
	//FILETIME creation_time;  
	//FILETIME exit_time;  
	//FILETIME kernel_time;  
	//FILETIME user_time;  
	//__int64 system_time;
	//__int64 time;
	//double cpu=-1;
	//GetSystemTimeAsFileTime(&now_time);  //获取当前系统时间
	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, PropPid);  //获取句柄
	//if (!hProcess)  
	//{  
	//	return -1;  
	//} 
	//if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  //获取进程时间
	//{  
	//	return -1;  
	//} 
	//system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPU使用时间  
	//time = GetProcessTime(now_time);        //现在的时间  

	//last_system_time_ = system_time;  
	//last_time_ = time;  
	//CloseHandle( hProcess );  
	////创建内核对象  并且等待1毫秒
	////HANDLE hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);   //   不要了  会降低启动及更新速度   
	////WaitForSingleObject(hEvent,1);

	////第二次获取处理器时间
	//hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, PropPid);  
	//if (!hProcess)  
	//{  
	//	return -1;  
	//}  
	//if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  
	//{  
	//	return -1;  
	//}  
	//GetSystemTimeAsFileTime(&now_time);  
	//system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPU使用时间  
	//time = GetProcessTime(now_time);        //现在的时间  

	//CloseHandle( hProcess );  //关闭句柄
	//cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;

	//return cpu;
	//cpu数量  
	static int processor_count_ = -1;  
	//上一次的时间  
	static __int64 last_time_ = 0;  
	static __int64 last_system_time_ = 0;  

	FILETIME now;  
	FILETIME creation_time;  
	FILETIME exit_time;  
	FILETIME kernel_time;  
	FILETIME user_time;  
	__int64 system_time;  
	__int64 time;  
	//  __int64 system_time_delta;  
	//  __int64 time_delta;  

	double cpu = -1;  

	if(processor_count_ == -1)  
	{  
		processor_count_ = GetCpuNumber();  
	}  	

	GetSystemTimeAsFileTime(&now);  

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, PropPid);  
	if (!hProcess)  
	{  
		return -1;  
	}  
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  
	{  
		return -1;  
	}  
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPU使用时间  
	time = FileTimeToInt64(now);        //现在的时间  

	last_system_time_ = system_time;  
	last_time_ = time;  
	CloseHandle( hProcess );  

//	Sleep(1000);  

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, false, PropPid);  
	if (!hProcess)  
	{  
		return -1;  
	}  
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  
	{  
		return -1;  
	}  
	GetSystemTimeAsFileTime(&now);  
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPU使用时间  
	time = FileTimeToInt64(now);        //现在的时间  

	CloseHandle( hProcess );  

	cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;  
	return cpu;  
}



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
//	int j = m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID;
	hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 
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



void ProcessDlg::SeeThread()
{
	// TODO: 在此添加命令处理程序代码
	if (!isFirstShow)
	{
		ThreadDLG.Create(IDD_DIALOG6);
		isFirstShow=true;
	}	
	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	int nItem = m_ProList.GetNextSelectedItem(pos);
	ThreadDLG.m_StrThread=m_VecProList[m_VecProList.size()-(nItem+1)].szExeFile;
	ThreadDLG.m_currPid = m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID;

	ThreadDLG.initThreadList();
	ThreadDLG.ShowWindow(SW_SHOW);
	//HANDLE hProcessHandle;
}




void ProcessDlg::SeeMoudle()
{
	
	// TODO: 在此添加命令处理程序代码
	if (!isFirstModShow)
	{
		MoudleDLG.Create(IDD_DIALOG7);
		isFirstModShow=true;
	}
	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	int nItem = m_ProList.GetNextSelectedItem(pos);
	MoudleDLG.m_StrMoudle=m_VecProList[m_VecProList.size()-(nItem+1)].szExeFile;
	MoudleDLG.m_ProCurrPid = m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID;

	MoudleDLG.initMoudleList();
	MoudleDLG.ShowWindow(SW_SHOW);
}

TCHAR http[MAX_PATH];
void ProcessDlg::SearchFileInformation()
{
	// TODO: 在此添加命令处理程序代码
	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	int nItem = m_ProList.GetNextSelectedItem(pos);
	_stprintf_s(http,_T("http://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&rsv_idx=1&ch=&tn=baiduerr&bar=&wd=%s&rn=&rsv_enter=1"),m_VecProList[m_VecProList.size()-(nItem+1)].szExeFile);
	ShellExecute(NULL,_T ("open"),http, NULL, NULL, SW_SHOWNORMAL);
}

void ProcessDlg::OpenFilePath()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR filePath[MAX_PATH];
	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	int nItem = m_ProList.GetNextSelectedItem(pos);
	HANDLE handle=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ ,
		FALSE,m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID);
	GetProcessImageFileName(handle,filePath,MAX_PATH);
	GetModuleFileNameEx(handle,NULL,filePath,MAX_PATH);//获取文件路径
	//CString csStr=_T("\\Device\\HarddiskVolume1\\123");
	
	CString NewfilePath(filePath);
	NewfilePath.Replace(_T("\\Device\\HarddiskVolume1\\"),_T("c:\\"));//将 \\Device\\HarddiskVolume1\\  替换为 c:\\       1111
	NewfilePath.Replace(_T("\\Device\\HarddiskVolume2\\"),_T("c:\\"));
	int index =NewfilePath.ReverseFind(_T('\\'));//截断EXE文件名
	NewfilePath = NewfilePath.Left(index);
	ShellExecute(NULL,_T ("open"),
		NewfilePath, NULL, NULL, SW_SHOWNORMAL);//打开所在文件夹
//	GetFileVersionString
}

//void ProcessDlg::CopyFileName()
//{
//	// TODO: 在此添加命令处理程序代码
//	//OpenClipboard();
//	//EmptyClipboard();
//	//SetClipboardData(CF_UNICODETEXT,NULL);
//	//CloseClipboard ();
//
//
//	HGLOBAL hMen;   
//	TCHAR strText[256] = _T("我被拷贝到剪切板！/n");   
//
//	// 分配全局内存    
//	hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(strText)+1)*sizeof(TCHAR)));    
//
//	if (!hMen)   
//	{   
//		printf("分配全局内存出错！/n");    
//		// 关闭剪切板    
//		CloseClipboard();   
//		return;         
//	}   
//
//	// 把数据拷贝考全局内存中   
//	// 锁住内存区    
//	LPSTR lpStr = (LPSTR)GlobalLock(hMen);    
//
//	// 内存复制   
//	memcpy(lpStr, strText, ((strlen(strText))*sizeof(TCHAR)));    
//	// 字符结束符    
//	lpStr[strlen(strText)] = (TCHAR)0;   
//	// 释放锁    
//	GlobalUnlock(hMen);   
//
//	// 把内存中的数据放到剪切板上   
//	SetClipboardData(CF_TEXT, hMen);   
//	CloseClipboard();  
//}
