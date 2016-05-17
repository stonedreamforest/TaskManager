
// ���������Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���������.h"
#include "���������Dlg.h"
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// C���������Dlg �Ի���



C���������Dlg::C���������Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C���������Dlg::IDD, pParent)
	, m_IsWindowHide(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_IsWindowHide=false;
}

void C���������Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
//	DDX_Control(pDX, IDC_STATIC1, m_StaticText);
}

BEGIN_MESSAGE_MAP(C���������Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &C���������Dlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON3, &C���������Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &C���������Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C���������Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &C���������Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C���������Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &C���������Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &C���������Dlg::OnBnClickedButton7)
	ON_WM_HOTKEY()
	//ON_UPDATE_COMMAND_UI(AFX_IDS_UPDATE_MENU, &C���������Dlg::OnUpdateAfxIdsUpdateMenu)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &C���������Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &C���������Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &C���������Dlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// C���������Dlg ��Ϣ�������

BOOL C���������Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	::RegisterHotKey(this->GetSafeHwnd(),0xa001,MOD_CONTROL|MOD_SHIFT,'P');//ע���ȼ� 
	m_Tab.InsertItem(0,_T("��ʾ����"));
	m_Tab.InsertItem(1,_T("���̹���"));
	m_Tab.InsertItem(2,_T("����"));
	m_Tab.addDlg(3,IDD_DIALOG5,new showexe, IDD_DIALOG2, new ProcessDlg,IDD_DIALOG8,new ServicerDlg);
	m_Tab.setshow(0);
	
	SetTimer(0x4,1000,NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C���������Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

//void C���������Dlg::OnPaint()
//{
//	
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// ʹͼ���ڹ����������о���
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// ����ͼ��
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialogEx::OnPaint();
//	}
//}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C���������Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C���������Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void C���������Dlg::AccessPrivileges()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = {0};
	TOKEN_PRIVILEGES stToken_Privileges = {0};
	hProcess = GetCurrentProcess();  //��ȡ��ǰӦ�ó�����̾��
	if(!OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken))//�򿪽��̷�������
	{
		return;
	}
	if (!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&uID))//��ȡϵͳȨ�޵���Ȩֵ
	{
		return;
	}
	stToken_Privileges.PrivilegeCount=1;////��������Ȩ�޸���
	stToken_Privileges.Privileges[0].Luid=uID; //Ȩ�޵�LUID
	stToken_Privileges.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;//Ȩ�޵�����,SE_PRIVILEGE_ENABLEDΪʹ�ܸ�Ȩ��
	if (!AdjustTokenPrivileges(hToken,FALSE,&stToken_Privileges,sizeof(stToken_Privileges),NULL,NULL))
	{
		return;
	}
	CloseHandle(hToken);
	return;
}



void C���������Dlg::OnBnClickedButton3()//�ػ�
{
	AccessPrivileges();
	ExitWindowsEx(EWX_SHUTDOWN,0);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void C���������Dlg::OnBnClickedButton1()//ע��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AccessPrivileges();
	ExitWindowsEx(EWX_LOGOFF,0);
}



void C���������Dlg::OnBnClickedButton2()//˯��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AccessPrivileges();
	SetSuspendState(TRUE,FALSE,FALSE);
}


void C���������Dlg::OnBnClickedButton4()//����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AccessPrivileges();
	ExitWindowsEx(EWX_REBOOT, 0); //����
}


void C���������Dlg::OnBnClickedButton5()//ǿ��ע�� ���ܻᶪʧ����
{
	AccessPrivileges();
	ExitWindowsEx(EWX_LOGOFF|EWX_FORCE,0);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void C���������Dlg::OnBnClickedButton6()//ǿ������  ���ܻᶪʧ����
{
	AccessPrivileges();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
}


void C���������Dlg::OnBnClickedButton7()//ǿ�ƹػ�  ���ܻᶪʧ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AccessPrivileges();
	ExitWindowsEx(EWX_SHUTDOWN|EWX_FORCE,0);
}


// BOOL C���������Dlg::PreTranslateMessage(MSG* pMsg)
// {
// 	// TODO: �ڴ����ר�ô����/����û���
// 	
// 	if ((pMsg->message==WM_HOTKEY)&&(pMsg->wParam==0xa001))
// 	{
// 		//���ش���
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


void C���������Dlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nHotKeyId==0xa001)
	{
		//���ش���
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



//void C���������Dlg::OnUpdateAfxIdsUpdateMenu(CCmdUI *pCmdUI)
//{
//	// TODO: �ڴ������������û����洦��������
//
//}


BOOL C���������Dlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CDialogEx::PreCreateWindow(cs);
}
//__int64 C���������Dlg::CompareFileTime ( FILETIME time1, FILETIME time2 )//ת��Ϊ����ʱ��
//{
//	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;
//	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime ;
//
//	return   (b - a);
//}
//TCHAR strCpu[MAX_PATH];
//void C���������Dlg::CpuOccupation()
//{
//	HANDLE hEvent;
//	BOOL res ;
//
//	FILETIME preidleTime;
//	FILETIME prekernelTime;		//������ û�и��õķ������ ����  
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
//	hEvent = CreateEvent (NULL,FALSE,FALSE,NULL);// ��ʼֵΪ nonsignaled ������ÿ�δ������Զ�����Ϊnonsignaled
//
//	//WaitForSingleObject( hEvent,1000 ); //�ȴ�
//	res = GetSystemTimes( &idleTime, &kernelTime, &userTime );
//
//	int idle = CompareFileTime( preidleTime,idleTime);
//	int kernel = CompareFileTime( prekernelTime, kernelTime);
//	int user = CompareFileTime(preuserTime, userTime);
//
//	float cpu = (kernel +user - idle) *100.0/(kernel+user);
//	float cpuidle = ( idle) *100.0/(kernel+user);
//	_stprintf_s(strCpu,_T("CPU������%d"),cpu);
//	//	cout << "CPU������:" << cpu << "%" << "      CPU������:" <<cpuidle << "%" <<endl;
//	m_StaticText.SetWindowText(strCpu);
//
//	preidleTime = idleTime;
//	prekernelTime = kernelTime;
//	preuserTime = userTime ;
//
//}
void C���������Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
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

void C���������Dlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ishookkey=true;
	CreateThread(0,0,(LPTHREAD_START_ROUTINE)ThreadProc,0,0,0);
}


void C���������Dlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ishookkey=false;
	UninstallHook();
}


void C���������Dlg::OnBnClickedButton10()
{
	
	CPoint   point;
	GetCursorPos(&point);
	CRect rect(point.x,point.y,point.x,point.y);
	ClipCursor(rect);
}
