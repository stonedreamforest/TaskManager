// ProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���������.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <tlhelp32.h>
#include "Psapi.h"
#include "TipDlg.h"
#pragma comment(lib,"Psapi.lib")
#include <shellapi.h>

// ProcessDlg �Ի���

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

//ȫ�ֱ���
HANDLE hProessSnap;
HANDLE hProcess;
PROCESSENTRY32 stcpe32={0};
PROCESS_MEMORY_COUNTERS pmc;//�ڴ�ռ�ú�����Ҫ
TCHAR Pid[MAX_PATH];
TCHAR Memory[MAX_PATH];
TCHAR CpuUtilization[MAX_PATH];//cpuʹ����
TCHAR ProfilePath[MAX_PATH];//����·��
TCHAR NewProfilePath[MAX_PATH];//����·��
TCHAR* Priorit; //�������ȼ�
//TCHAR TempPriorit[MAX_PATH]={0}; //��ʱ�������ȼ�
MEMORYSTATUS memStatus;
//��������
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

	//����һ��������صĿ��վ��
	hProessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProessSnap==INVALID_HANDLE_VALUE)
	{
		return ;
	}
	if (!Process32FirstW(hProessSnap,&stcpe32))
	{
		CloseHandle(hProessSnap);		//��ȡϵͳ����
		return ;
	}
	do 
	{
		PROCESSENTRY32	stcInfo={sizeof(PROCESSENTRY32)};
		//��ȡ����ӳ��·��
		wcscpy_s(stcInfo.szExeFile,stcpe32.szExeFile);
		hProcess=OpenProcess(
			PROCESS_QUERY_INFORMATION|PROCESS_VM_READ ,
			FALSE,stcpe32.th32ProcessID);
		if (hProcess)
		{
			stcInfo.pcPriClassBase=GetPriorityClass(hProcess);

		}
		GetProcessMemoryInfo(hProcess,&pmc,sizeof(pmc));//��ù����� workingsetsize
		cpu=Get_cpu_usage(stcpe32.th32ProcessID);//��ȡcpuռ����
		GetProcessImageFileName(hProcess,ProfilePath,MAX_PATH);
		GetModuleFileNameEx(hProcess,NULL,ProfilePath,MAX_PATH);//��ȡ�ļ�·��
		
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
		m_ProList.SetItemText(0,1,Pid);//����PID
		m_ProList.SetItemText(0,2,Memory);//���빤����
		m_ProList.SetItemText(0,3,CpuUtilization);//����cpuռ����
		m_ProList.SetItemText(0,4,Priorit);//
		//����ȡ�������Ϣ���浽������
		m_ProList.SetItemText(0,5,NewProfilePath);
		m_VecProList.push_back(stcInfo);
		m_VecPriority.push_back(Priorit);//�������ȼ�
		m_VecMemory.push_back(pmc.WorkingSetSize/1024);//�����ڴ�
		m_VevCpu.push_back(cpu);//����CPU
	} while (Process32Next(hProessSnap,&stcpe32));

	CloseHandle(hProcess);
	CloseHandle(hProessSnap);	
};
// ProcessDlg ��Ϣ�������


BOOL ProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect;
	m_ProList.GetClientRect(rect);
	m_ProList.SetExtendedStyle(m_ProList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//��ѡ�з��
	CString str[] ={_T(""), _T("����"), _T("PID"), _T("WorkingSetSize"), _T("CPU"),  _T("���ȼ�"),_T("·��")};
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
	m_ProList.DeleteColumn(0);//ɾ����0�� �������־���
	ProcessIndex();
//	m_ProList.InsertItem(0,L"123");
 //	m_ProList.SetItemText(0,1,_T("123"));
 //	m_ProList.SetItemText(0,2,_T("123"));
	

	SetTimer(0x1,1000,NULL);//ʵʱˢ�� ÿ1000����
	return TRUE;  // return TRUE unless you set the focus to a control

	// �쳣: OCX ����ҳӦ���� FALSE
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
	ProcessIndex();//�л�����ˢ��
}


void ProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RefreshProcess();//ˢ��
}

void ProcessDlg::RefreshProcess()
{
	int OriginalValue=m_VecProList.size();//ԭֵ  ��ˢ��ǰ��ֵ
	int ListPosflag=m_VecProList.size();//�б�λ��  ���Ȳ���� ��LIST������ 
	m_NewVecProList.clear();
	m_NewVecPriority.clear();
	m_NewVecMemory.clear();
	m_NewVevCpu.clear();
	int CountFlag=0;
	double cpu;
	stcpe32.dwSize=sizeof(PROCESSENTRY32);
	//����һ��������صĿ��վ��
	hProessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProessSnap==INVALID_HANDLE_VALUE)
	{
		return ;
	}
	if (!Process32FirstW(hProessSnap,&stcpe32))
	{
		CloseHandle(hProessSnap);		//��ȡϵͳ����
		return ;
	}
	do 
	{
		PROCESSENTRY32	stcInfo={sizeof(PROCESSENTRY32)};
		//��ȡ����ӳ��·��
		wcscpy_s(stcInfo.szExeFile,stcpe32.szExeFile);
		hProcess=OpenProcess(
			PROCESS_QUERY_INFORMATION|PROCESS_VM_READ ,
			FALSE,stcpe32.th32ProcessID);
		if (hProcess)
		{
			stcInfo.pcPriClassBase=GetPriorityClass(hProcess);
		}
		GetProcessMemoryInfo(hProcess,&pmc,sizeof(pmc));//��ù����� workingsetsize
		cpu=Get_cpu_usage(stcpe32.th32ProcessID);//��ȡcpuռ����
		GetProcessImageFileName(hProcess,ProfilePath,MAX_PATH);
		GetModuleFileNameEx(hProcess,NULL,ProfilePath,MAX_PATH);//��ȡ�ļ�·��
		
		CString NewProfilePath(ProfilePath);
		NewProfilePath.Replace(_T("\\Device\\HarddiskVolume1\\"),_T("c:\\"));
		NewProfilePath.Replace(_T("\\Device\\HarddiskVolume2\\"),_T("c:\\"));
		stcInfo.th32ProcessID=stcpe32.th32ProcessID;
		stcInfo.cntThreads=stcpe32.cntThreads;
		stcInfo.th32ParentProcessID=stcpe32.th32ParentProcessID;
		 Priorit=InitializePriority(stcInfo.pcPriClassBase);
		//	m_ProList.InsertItem(0,stcpe32.szExeFile);
		//������ͬһѭ���Ƚ� ��Ϊǡ��  ���̵߳�ԭ��ûŪ�����
		if (CountFlag<OriginalValue)
		{
			if (m_VecProList[CountFlag].th32ProcessID!=stcpe32.th32ProcessID)//�ϴ��б���ͬ������ϵĽ��̲�ͬʱ��ɾ��
			{
				int num=m_VecProList.size();//��ɾ
				m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
				m_VecProList.erase(m_VecProList.begin()+CountFlag);//��ɾ��λ�õ���һ���ᵽ��ɾ����λ����
				m_VecPriority.erase(m_VecPriority.begin()+CountFlag);
				m_VecMemory.erase(m_VecMemory.begin()+CountFlag);
				m_VevCpu.erase(m_VevCpu.begin()+CountFlag);

				UINT NowPos=CountFlag;//Ŀǰ�Ա�λ�� ��1 ָ����һ��PID���жԱ�
				for (;NowPos<m_VecProList.size()-1;NowPos++)
				{
					if (m_VecProList[NowPos].th32ProcessID!=stcpe32.th32ProcessID)
					{
						m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
						m_VecProList.erase(m_VecProList.begin()+CountFlag);
						m_VecPriority.erase(m_VecPriority.begin()+CountFlag);
						m_VecMemory.erase(m_VecMemory.begin()+CountFlag);
						m_VevCpu.erase(m_VevCpu.begin()+CountFlag);
						NowPos--;//����ֵ����  ��λ�ò���
					}
					else
					{
						//CountFlag--
						break;
					}
				}
 				
			}
			if (wcscmp(Priorit,m_VecPriority[CountFlag])!=0)//�Ƚ��Ƿ�ˢ�����ȼ�
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
			m_ProList.SetItemText(0,3,CpuUtilization);//����cpuռ����
			m_ProList.SetItemText(0,4,Priorit);//
			m_ProList.SetItemText(0,5,NewProfilePath);//

		}
	//	m_ProList.SetItemText(0,4,_T(" "));//
	//	//
		//����ȡ�������Ϣ���浽������
		m_NewVecProList.push_back(stcInfo);
		m_NewVecPriority.push_back(Priorit);
		m_NewVecMemory.push_back(pmc.WorkingSetSize/1024);
		m_NewVevCpu.push_back(cpu);
	} while (Process32Next(hProessSnap,&stcpe32));
	//����֮�����µĽ����б��ԭ������
	if (CountFlag<OriginalValue)
	{
		for (UINT i=CountFlag;i<m_VecProList.size();i++)
		{
			m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);//ɾ������
			m_VecProList.erase(m_VecProList.begin()+CountFlag);//vec�б��������
			m_VecPriority.erase(m_VecPriority.begin()+CountFlag);
			m_VecMemory.erase(m_VecMemory.begin()+CountFlag);
			m_VevCpu.erase(m_VevCpu.begin()+CountFlag);
		}
	}
	CloseHandle(hProcess);
	CloseHandle(hProessSnap);	
	m_VecProList=m_NewVecProList;//���¸�ֵ  ����  ������/PID  ���ȼ�  �ڴ� cpu       
	m_VecPriority= m_NewVecPriority;
	m_VecMemory=m_NewVecMemory;
	m_VevCpu=m_NewVevCpu;
}


void ProcessDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{

	


	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	m_ProList.GetClientRect(&rect);//�ɵ����Χ  
	CPoint p;

	//GetPhysicalCursorPos(&p);//�������
	GetCursorPos(&p);
	rect.DeflateRect(0,23,0,0);
	m_ProList.ClientToScreen(rect);//������Ļ����
	if (rect.PtInRect(p))//�ж��Ƿ��ڿͻ���listctrl��Χ��
 	{
		CMenu Menu;
		Menu.LoadMenuW(IDR_MENU1);//�˵���Դ
		CMenu *SubMenu=Menu.GetSubMenu(0);
		SubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON,p.x,p.y,this);
		//SubMenu->Detach();
		//Menu.DestroyMenu();//����
	}

}


void ProcessDlg::KillProcess()
{
	HANDLE hProcessHandle;

	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	if (pos == NULL)//�ж��Ƿ�Ϊ��
	{
		return;
	}
	else
	{
		//�ɰ�ctrl+���ѡ�ж��н�������
		while (pos)
		{
			int nItem = m_ProList.GetNextSelectedItem(pos);
			m_ProList.DeleteItem(nItem);
			//m_VecProList.erase(m_VecProList.begin()+nItem);//vec�б��������
			//m_VecPriority.erase(m_VecPriority.begin()+nItem);
			//m_VecMemory.erase(m_VecMemory.begin()+nItem);
			//m_VevCpu.erase(m_VevCpu.begin()+nItem);
			hProcessHandle = OpenProcess( PROCESS_TERMINATE, FALSE, 
				m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID);
			TerminateProcess( hProcessHandle, 4 );//
		}
	}
	// TODO: �ڴ���������������
}

int ProcessDlg::GetCpuNumber()
{
	SYSTEM_INFO info;  
	GetSystemInfo(&info);  //�õ�ϵͳ��Ϣ
	return (int)info.dwNumberOfProcessors;  //���ش�������Ŀ
}

__int64 ProcessDlg::FileTimeToInt64(const FILETIME& time)
{
	ULARGE_INTEGER tt;  
	tt.LowPart = time.dwLowDateTime;  //�ļ���32λʱ��
	tt.HighPart = time.dwHighDateTime;  //�ļ���32λʱ��
	return tt.QuadPart;  
}

double ProcessDlg::Get_cpu_usage(DWORD PropPid)
{
	//static int CpuNum = GetCpuNumber();//���cpu���� 

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
	//GetSystemTimeAsFileTime(&now_time);  //��ȡ��ǰϵͳʱ��
	//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, PropPid);  //��ȡ���
	//if (!hProcess)  
	//{  
	//	return -1;  
	//} 
	//if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  //��ȡ����ʱ��
	//{  
	//	return -1;  
	//} 
	//system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPUʹ��ʱ��  
	//time = GetProcessTime(now_time);        //���ڵ�ʱ��  

	//last_system_time_ = system_time;  
	//last_time_ = time;  
	//CloseHandle( hProcess );  
	////�����ں˶���  ���ҵȴ�1����
	////HANDLE hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);   //   ��Ҫ��  �ή�������������ٶ�   
	////WaitForSingleObject(hEvent,1);

	////�ڶ��λ�ȡ������ʱ��
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
	//system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPUʹ��ʱ��  
	//time = GetProcessTime(now_time);        //���ڵ�ʱ��  

	//CloseHandle( hProcess );  //�رվ��
	//cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;

	//return cpu;
	//cpu����  
	static int processor_count_ = -1;  
	//��һ�ε�ʱ��  
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
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPUʹ��ʱ��  
	time = FileTimeToInt64(now);        //���ڵ�ʱ��  

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
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / processor_count_;  //CPUʹ��ʱ��  
	time = FileTimeToInt64(now);        //���ڵ�ʱ��  

	CloseHandle( hProcess );  

	cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;  
	return cpu;  
}



BOOL ProcessDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->wParam==VK_RETURN)
	{
		//Do anything what you want to
		return true;//����enter��Ϣ��Ӧ
	}
	if(pMsg->wParam == VK_ESCAPE)  
	{  
		return true;  //����esc��Ϣ��Ӧ
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void ProcessDlg::RealTime()//���ȼ�ʵʱ
{
	// TODO: �ڴ���������������
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
	if (dlg.DoModal()==IDCANCEL)//ȡ��������
	{
		return;
	}
//	CMenu* hMenu = GetMenu();
	switch (PriorityFlag)
	{       //�������ֳɻ��Զ����� ��ʡŰ�ˡ���������Ű�ˡ�����
	case 1:
		
		//CheckMenuItem (hMenu, ID_32782, MF_UNCHECKED);
	//	hMenu->CheckMenuItem (ID_32782, MF_CHECKED);
		//MessageBox(0,0,0);
		SetPriorityClass(hProcessHandle,REALTIME_PRIORITY_CLASS);//ʵʱ		
		break;
	case 2:
		SetPriorityClass(hProcessHandle,HIGH_PRIORITY_CLASS);//��
		break;
	case 3:
		SetPriorityClass(hProcessHandle,ABOVE_NORMAL_PRIORITY_CLASS);//���ڱ�׼
		break;
	case 4:
		SetPriorityClass(hProcessHandle,NORMAL_PRIORITY_CLASS);//��׼
		break;
	case 5:
		SetPriorityClass(hProcessHandle,BELOW_NORMAL_PRIORITY_CLASS);//���ڱ�׼
		break;
	case 6:
		SetPriorityClass(hProcessHandle,IDLE_PRIORITY_CLASS);//��
		break;
	default:
		break;
	}
}



void ProcessDlg::high()
{
	// TODO: �ڴ���������������
	SetProcessPriority(2);
}


void ProcessDlg::AboveNormal()
{
	// TODO: �ڴ���������������
	SetProcessPriority(3);
}


void ProcessDlg::normal()
{
	// TODO: �ڴ���������������
	SetProcessPriority(4);
}


void ProcessDlg::BelowNormal()
{
	// TODO: �ڴ���������������
	SetProcessPriority(5);
}



void ProcessDlg::low()
{
	// TODO: �ڴ���������������
	SetProcessPriority(6);
}

TCHAR* ProcessDlg::InitializePriority(DWORD dwPriority)//��ʼ���������ȼ�
{

	
	// ������Ϣ��Ϊ�û��������� 
	switch(dwPriority)
	{
	case REALTIME_PRIORITY_CLASS:
		return _T("ʵʱ");
	case HIGH_PRIORITY_CLASS:
		return _T("��");
	case ABOVE_NORMAL_PRIORITY_CLASS:
		return _T("���ڱ�׼");
	case NORMAL_PRIORITY_CLASS:
		return _T("��׼");
	case BELOW_NORMAL_PRIORITY_CLASS:
		return _T("���ڱ�׼");
	case IDLE_PRIORITY_CLASS:
		return _T("��");
	default:
		break;
	}
	return _T("�ܾ�����");;
}



void ProcessDlg::SeeThread()
{
	// TODO: �ڴ���������������
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
	
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	int nItem = m_ProList.GetNextSelectedItem(pos);
	_stprintf_s(http,_T("http://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&rsv_idx=1&ch=&tn=baiduerr&bar=&wd=%s&rn=&rsv_enter=1"),m_VecProList[m_VecProList.size()-(nItem+1)].szExeFile);
	ShellExecute(NULL,_T ("open"),http, NULL, NULL, SW_SHOWNORMAL);
}

void ProcessDlg::OpenFilePath()
{
	// TODO: �ڴ���������������
	TCHAR filePath[MAX_PATH];
	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	int nItem = m_ProList.GetNextSelectedItem(pos);
	HANDLE handle=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ ,
		FALSE,m_VecProList[m_VecProList.size()-(nItem+1)].th32ProcessID);
	GetProcessImageFileName(handle,filePath,MAX_PATH);
	GetModuleFileNameEx(handle,NULL,filePath,MAX_PATH);//��ȡ�ļ�·��
	//CString csStr=_T("\\Device\\HarddiskVolume1\\123");
	
	CString NewfilePath(filePath);
	NewfilePath.Replace(_T("\\Device\\HarddiskVolume1\\"),_T("c:\\"));//�� \\Device\\HarddiskVolume1\\  �滻Ϊ c:\\       1111
	NewfilePath.Replace(_T("\\Device\\HarddiskVolume2\\"),_T("c:\\"));
	int index =NewfilePath.ReverseFind(_T('\\'));//�ض�EXE�ļ���
	NewfilePath = NewfilePath.Left(index);
	ShellExecute(NULL,_T ("open"),
		NewfilePath, NULL, NULL, SW_SHOWNORMAL);//�������ļ���
//	GetFileVersionString
}

//void ProcessDlg::CopyFileName()
//{
//	// TODO: �ڴ���������������
//	//OpenClipboard();
//	//EmptyClipboard();
//	//SetClipboardData(CF_UNICODETEXT,NULL);
//	//CloseClipboard ();
//
//
//	HGLOBAL hMen;   
//	TCHAR strText[256] = _T("�ұ����������а壡/n");   
//
//	// ����ȫ���ڴ�    
//	hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(strText)+1)*sizeof(TCHAR)));    
//
//	if (!hMen)   
//	{   
//		printf("����ȫ���ڴ����/n");    
//		// �رռ��а�    
//		CloseClipboard();   
//		return;         
//	}   
//
//	// �����ݿ�����ȫ���ڴ���   
//	// ��ס�ڴ���    
//	LPSTR lpStr = (LPSTR)GlobalLock(hMen);    
//
//	// �ڴ渴��   
//	memcpy(lpStr, strText, ((strlen(strText))*sizeof(TCHAR)));    
//	// �ַ�������    
//	lpStr[strlen(strText)] = (TCHAR)0;   
//	// �ͷ���    
//	GlobalUnlock(hMen);   
//
//	// ���ڴ��е����ݷŵ����а���   
//	SetClipboardData(CF_TEXT, hMen);   
//	CloseClipboard();  
//}
