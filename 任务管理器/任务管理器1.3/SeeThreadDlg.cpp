// SeeThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���������.h"
#include "SeeThreadDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include <vector>
#include <tchar.h>

// SeeThread �Ի���

IMPLEMENT_DYNAMIC(SeeThreadDlg, CDialogEx)

SeeThreadDlg::SeeThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SeeThreadDlg::IDD, pParent)
{
	isThread=false;
}

SeeThreadDlg::~SeeThreadDlg()
{
}

void SeeThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ThreadList);
}


BEGIN_MESSAGE_MAP(SeeThreadDlg, CDialogEx)
	ON_WM_SIZE()
	ON_COMMAND(ID_32792, &SeeThreadDlg::KillThread)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

TCHAR strThreadId[MAX_PATH];
void SeeThreadDlg::initThreadList()
{
	//�������е�thread��
	//
	SetWindowText(m_StrThread);
	THREADENTRY32 thd32={sizeof(THREADENTRY32)};
	HANDLE hThreadsnap;
	hThreadsnap=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD
		,m_currPid);
	if (hThreadsnap==INVALID_HANDLE_VALUE)
	{
		return;
	}
	if (!Thread32First(hThreadsnap,&thd32))
	{
		CloseHandle(hThreadsnap);
		return;
	}
	m_VecThdList.clear();
	do 
	{
		_stprintf_s(strThreadId,_T("%d"),thd32.th32ThreadID);
		m_ThreadList.InsertItem(0,strThreadId);
		m_VecThdList.push_back(thd32);

	} while (Thread32Next(hThreadsnap,&thd32));
	CloseHandle(hThreadsnap);
}


// SeeThread ��Ϣ�������


BOOL SeeThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	
	CRect rect;
	m_ThreadList.GetClientRect(rect);
	m_ThreadList.SetExtendedStyle(m_ThreadList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//��ѡ�з��
	CString str[] ={_T(""), _T("�߳�ID")};
	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		m_ThreadList.InsertColumn(i, str[i], LVCFMT_CENTER, 442);
	}
	m_ThreadList.DeleteColumn(0);//ɾ����0�� �������־���
	isThread=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void SeeThreadDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (!isThread)
	{
		return;
	}
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(10,0,0,0);
	m_ThreadList.MoveWindow(rect,TRUE);

	// TODO: �ڴ˴������Ϣ����������
}


void SeeThreadDlg::KillThread()
{
	// TODO: �ڴ���������������
	POSITION pos = m_ThreadList.GetFirstSelectedItemPosition();
	//int nItem = m_ThreadList.GetNextSelectedItem(pos);
	HANDLE hProcessHandle;

//	POSITION pos = m_ProList.GetFirstSelectedItemPosition();
	if (pos == NULL)//�ж��Ƿ�Ϊ��
	{
		return;
	}
	else
	{
		//�ɰ�ctrl+���ѡ�ж��н�������
		while (pos)
		{
			
			int nItem = m_ThreadList.GetNextSelectedItem(pos);
			m_ThreadList.DeleteItem(nItem);
			hProcessHandle = OpenThread(PROCESS_TERMINATE,
				FALSE, 
				m_VecThdList[m_VecThdList.size()-(nItem+1)].th32ThreadID);
		//	GetExitCodeThread(hProcessHandle,&ExitCode);
			TerminateThread( hProcessHandle,0 );//
		}
	}

}


void SeeThreadDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	m_ThreadList.GetClientRect(&rect);//�ɵ����Χ  
	CPoint p;

	//GetPhysicalCursorPos(&p);//�������
	GetCursorPos(&p);
	rect.DeflateRect(0,23,0,0);
	m_ThreadList.ClientToScreen(rect);//������Ļ����
	if (rect.PtInRect(p))//�ж��Ƿ��ڿͻ���listctrl��Χ��
	{
		CMenu Menu;
		Menu.LoadMenuW(IDR_MENU2);//�˵���Դ
		CMenu *SubMenu=Menu.GetSubMenu(0);
		SubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON,p.x,p.y,this);
		//SubMenu->Detach();
		//Menu.DestroyMenu();//����
	}
}
