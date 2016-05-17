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
// ProcessDlg �Ի���

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

//ȫ�ֱ���
HANDLE hProessSnap;
HANDLE hProcess;
PROCESSENTRY32 stcpe32={0};
PROCESS_MEMORY_COUNTERS pmc;//�ڴ�ռ�ú�����Ҫ
TCHAR Pid[MAX_PATH];
TCHAR Memory[MAX_PATH];
TCHAR CpuUtilization[MAX_PATH];//cpuʹ����
TCHAR Priorit[MAX_PATH]={0}; //�������ȼ�
MEMORYSTATUS memStatus;
//��������
void ProcessDlg::ProcessIndex()
{
	
	m_VecProList.clear();
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
		stcInfo.th32ProcessID=stcpe32.th32ProcessID;
		stcInfo.cntThreads=stcpe32.cntThreads;
		stcInfo.th32ParentProcessID=stcpe32.th32ParentProcessID;
		m_ProList.InsertItem(0,stcpe32.szExeFile);
		_stprintf_s(Pid,_T("%d"),stcpe32.th32ProcessID);///PID
		_stprintf_s(Memory,_T("%d %s"),pmc.WorkingSetSize/1024,_T("k"));
		_stprintf_s(CpuUtilization,_T("%d"),cpu);
		m_ProList.SetItemText(0,1,Pid);//����PID
		m_ProList.SetItemText(0,2,Memory);//���빤����
		m_ProList.SetItemText(0,3,CpuUtilization);//����cpuռ����
		m_ProList.SetItemText(0,4,InitializePriority(stcInfo.pcPriClassBase));//
		//����ȡ�������Ϣ���浽������
		m_VecProList.push_back(stcInfo);
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
	CString str[] ={_T(""), _T("����"), _T("PID"), _T("WorkingSetSize"), _T("CPU"),  _T("���ȼ�"),_T("����")};
	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		m_ProList.InsertColumn(i, str[i], LVCFMT_CENTER, 121);
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
	ProcessIndex();//�л�����ˢ��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RefreshProcess();//ˢ��
}

void ProcessDlg::RefreshProcess()
{
	int OriginalValue=m_VecProList.size();//ԭֵ  ��ˢ��ǰ��ֵ
	m_NewVecProList.clear();
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
		stcInfo.th32ProcessID=stcpe32.th32ProcessID;
		stcInfo.cntThreads=stcpe32.cntThreads;
		stcInfo.th32ParentProcessID=stcpe32.th32ParentProcessID;
		//	m_ProList.InsertItem(0,stcpe32.szExeFile);
		//������ͬһѭ���Ƚ� ��Ϊǡ��  ���̵߳�ԭ��ûŪ�����
		if (CountFlag<OriginalValue)
		{
			if (m_VecProList[CountFlag].th32ProcessID!=stcpe32.th32ProcessID)//�ϴ��б���ͬ������ϵĽ��̲�ͬʱ��ɾ��
			{
				int num=m_VecProList.size();//��ɾ
				m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
				m_VecProList.erase(m_VecProList.begin()+CountFlag);//��ɾ��λ�õ���һ���ᵽ��ɾ����λ����
				UINT NowPos=CountFlag;//Ŀǰ�Ա�λ�� ��1 ָ����һ��PID���жԱ�
				for (;NowPos<m_VecProList.size()-1;NowPos++)
				{
					if (m_VecProList[NowPos].th32ProcessID!=stcpe32.th32ProcessID)
					{
						m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);
						m_VecProList.erase(m_VecProList.begin()+CountFlag);
						NowPos--;//����ֵ����  ��λ�ò���
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
			m_ProList.SetItemText(0,3,CpuUtilization);//����cpuռ����
			m_ProList.SetItemText(0,4,InitializePriority(stcInfo.pcPriClassBase));//

		}

		//����ȡ�������Ϣ���浽������
		m_NewVecProList.push_back(stcInfo);
	} while (Process32Next(hProessSnap,&stcpe32));
	//����֮�����µĽ����б��ԭ������
	if (CountFlag<OriginalValue)
	{
		for (UINT i=CountFlag;i<m_VecProList.size();i++)
		{
			m_ProList.DeleteItem(m_VecProList.size()-CountFlag-1);//ɾ������
			m_VecProList.erase(m_VecProList.begin()+CountFlag);//vec�б��������
		}
	}
	CloseHandle(hProcess);
	CloseHandle(hProessSnap);	
	m_VecProList=m_NewVecProList;
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

int ProcessDlg::GetProcessTime(const FILETIME& time)
{
	ULARGE_INTEGER tt;  
	tt.LowPart = time.dwLowDateTime;  //�ļ���32λʱ��
	tt.HighPart = time.dwHighDateTime;  //�ļ���32λʱ��
	return (int)(tt.QuadPart);  
}

double ProcessDlg::Get_cpu_usage(DWORD PropPid)
{
	static int CpuNum = GetCpuNumber();//���cpu���� 

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
	GetSystemTimeAsFileTime(&now_time);  //��ȡ��ǰϵͳʱ��
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, PropPid);  //��ȡ���
	if (!hProcess)  
	{  
		return -1;  
	} 
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))  //��ȡ����ʱ��
	{  
		return -1;  
	} 
	system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPUʹ��ʱ��  
	time = GetProcessTime(now_time);        //���ڵ�ʱ��  

	last_system_time_ = system_time;  
	last_time_ = time;  
	CloseHandle( hProcess );  
	//�����ں˶���  ���ҵȴ�1����
	/*HANDLE hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);      ��Ҫ��  �ή�������������ٶ�   
	WaitForSingleObject(hEvent,1);*/

	//�ڶ��λ�ȡ������ʱ��
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
	system_time = (GetProcessTime(kernel_time) + GetProcessTime(user_time)) / CpuNum;  //CPUʹ��ʱ��  
	time = GetProcessTime(now_time);        //���ڵ�ʱ��  

	CloseHandle( hProcess );  //�رվ��
	cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;

	return cpu;
}


//void ProcessDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	return;
//	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
//}


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
	hProcessHandle = OpenProcess(PROCESS_TERMINATE, FALSE, 
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
