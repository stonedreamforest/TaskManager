// SeeMoudleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���������.h"
#include "SeeMoudleDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>
#include <vector>
#include <delayimp.h>

// SeeMoudleDlg �Ի���

IMPLEMENT_DYNAMIC(SeeMoudleDlg, CDialogEx)

SeeMoudleDlg::SeeMoudleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SeeMoudleDlg::IDD, pParent)
{
	isInitEnd = FALSE;
}

SeeMoudleDlg::~SeeMoudleDlg()
{
}

void SeeMoudleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST1, m_VecModList);
	DDX_Control(pDX, IDC_LIST1, m_MoudleList);
}


BEGIN_MESSAGE_MAP(SeeMoudleDlg, CDialogEx)
	ON_WM_SIZE()
	ON_COMMAND(ID__32793, &SeeMoudleDlg::KillMoudle)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

TCHAR MoudleId[MAX_PATH];
TCHAR MoudleBaseAdd[MAX_PATH];

void SeeMoudleDlg::initMoudleList()
{
	SetWindowText(m_StrMoudle);
	MODULEENTRY32 mod32={sizeof(MODULEENTRY32)};
	HANDLE hMoudleSnap=INVALID_HANDLE_VALUE;
	hMoudleSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,m_ProCurrPid);//TH32CS_SNAPMODULE
	if (hMoudleSnap==INVALID_HANDLE_VALUE)
	{
		return;
	}
	if (!Module32First(hMoudleSnap,&mod32))
	{
		CloseHandle(hMoudleSnap);
		return;
	}
	m_VecModList.clear();
	do 
	{
		_stprintf_s(MoudleId,_T("%d"),mod32.modBaseSize);
		_stprintf_s(MoudleBaseAdd,_T("%x"),mod32.modBaseAddr);
		m_MoudleList.InsertItem(0,MoudleId);
		m_MoudleList.SetItemText(0,1,MoudleBaseAdd);
		m_MoudleList.SetItemText(0,2,mod32.szExePath);
		m_VecModList.push_back(mod32);
	} while (Module32Next(hMoudleSnap,&mod32));
}


// SeeMoudleDlg ��Ϣ�������


BOOL SeeMoudleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CRect rect;
	m_MoudleList.GetClientRect(rect);
	m_MoudleList.SetExtendedStyle(m_MoudleList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//��ѡ�з��
	CString str[] ={_T(""), _T("ģ���С"), _T("ģ���ַ"), _T("ģ��·��")};
	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		if (i==3)
		{
			m_MoudleList.InsertColumn(i, str[i], LVCFMT_LEFT, 700);
		}
		else
		{
				m_MoudleList.InsertColumn(i, str[i], LVCFMT_CENTER, 120);
		}
		
	}
	m_MoudleList.DeleteColumn(0);//ɾ����0�� �������־���
	isInitEnd = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void SeeMoudleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);


	if(!isInitEnd){
		return;
	}
	CRect rc ;
	GetClientRect(&rc);
	rc.DeflateRect(0,0,0,0);
	m_MoudleList.MoveWindow(rc,TRUE) ;
}

typedef ULONG (WINAPI *PFNNtUnmapViewOfSection)( IN HANDLE ProcessHandle,IN PVOID BaseAddress );
void SeeMoudleDlg::KillMoudle()
{
	// TODO: �ڴ���������������
	POSITION pos = m_MoudleList.GetFirstSelectedItemPosition();
	//int nItem = m_ThreadList.GetNextSelectedItem(pos);
	HMODULE hProcessHandle;

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

			int nItem = m_MoudleList.GetNextSelectedItem(pos);
			m_MoudleList.DeleteItem(nItem);
			CString MoudleName(m_VecModList[m_VecModList.size()-(nItem+1)].szExePath);
			//	MoudleName.Replace(_T("\\Device\\HarddiskVolume1\\"),_T("c:\\"));
			int index =MoudleName.ReverseFind(_T('\\'));//��ȴģ����
			MoudleName = MoudleName.Right(MoudleName.GetLength()-index-1);
			hProcessHandle = LoadLibrary(MoudleName);





			//	GetExitCodeThread(hProcessHandle,&ExitCode);
			// PFNNtUnmapViewOfSection pfnNtUnmapViewOfSection = 
			//	 (PFNNtUnmapViewOfSection)GetProcAddress ( hProcessHandle, "NtUnmapViewOfSection" ) ;
			//  HANDLE hProcess = OpenProcess ( PROCESS_ALL_ACCESS, TRUE, m_ProCurrPid ) ;
			// ULONG    ret = pfnNtUnmapViewOfSection (hProcess, m_VecModList[m_VecModList.size()-(nItem+1)].modBaseAddr );
			 //__FUnloadDelayLoadedDLL2()
			// FreeLibraryAndExitThread(hProcessHandle,0);
			FreeLibrary(hProcessHandle);  
		//	 CloseHandle ( hProcess ) ;
		//	NtUnmapViewOfSection()
		}
	}
}


void SeeMoudleDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	m_MoudleList.GetClientRect(&rect);//�ɵ����Χ  
	CPoint p;

	//GetPhysicalCursorPos(&p);//�������
	GetCursorPos(&p);
	rect.DeflateRect(0,23,0,0);
	m_MoudleList.ClientToScreen(rect);//������Ļ����
	if (rect.PtInRect(p))//�ж��Ƿ��ڿͻ���listctrl��Χ��
	{
		CMenu Menu;
		Menu.LoadMenuW(IDR_MENU3);//�˵���Դ
		CMenu *SubMenu=Menu.GetSubMenu(0);
		SubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON,p.x,p.y,this);
		//SubMenu->Detach();
		//Menu.DestroyMenu();//����
	}
}
