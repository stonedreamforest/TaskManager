// showexe.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���������.h"
#include "showexe.h"
#include "afxdialogex.h"
#include "vector"
using namespace std;
// showexe �Ի���

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
	ON_WM_TIMER()
END_MESSAGE_MAP()





BOOL CALLBACK showexe::PROC1(HWND hwnd,LPARAM lparam)	
{
	TCHAR exeName[MAX_PATH];
 	if (!::IsWindowVisible(hwnd))
 	{
 		return TRUE;
 	}
 	if (!::GetWindowText(hwnd,exeName,MAX_PATH))
 	{
 		return TRUE;
 	}
 	m_myThis->Exelist.InsertItem(0,exeName);//pmc.WorkingSetSize
  	m_myThis->Exelist.SetItemText(0,1,L"��������");
	//m_myThis->m_VecExeList.push_back(exeName);
 	//_tprintf(L"%s\n",str);
 	//GetClassName(hwnd,str1,MAX_PATH);
	return TRUE;
 }	
// showexe ��Ϣ�������


BOOL showexe::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect;
	Exelist.GetClientRect(rect);
	Exelist.SetExtendedStyle(Exelist.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//��ѡ�з��
	CString str[] ={ _T("������"), _T("״̬")};
	for(int i=0; i<sizeof(str)/sizeof(str[0]); i++)
	{
		if (i==0)
		{
			Exelist.InsertColumn(i, str[i], LVCFMT_LEFT, 515);
		}
		else
		{
			Exelist.InsertColumn(i, str[i], LVCFMT_LEFT, rect.Width()/2);
		}
		
	}
//	Exelist.DeleteColumn(0);//ɾ����0�� �������־���
	m_VecExeList.clear();
	EnumWindows(PROC1,0);

	m_myThis->isInitEnd=TRUE;
	SetTimer(0x2,5000,NULL);//ʵʱˢ�� ÿ1000����
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ˴������Ϣ����������
}

//BOOL CALLBACK showexe::PROCNew(HWND hwnd,LPARAM lparam)	
//{
//	TCHAR exeName[MAX_PATH];
//	if (!::IsWindowVisible(hwnd))
//	{
//		return TRUE;
//	}
//	if (!::GetWindowText(hwnd,exeName,MAX_PATH))
//	{
//		return TRUE;
//	}
//	m_myThis->Exelist.InsertItem(0,exeName);//pmc.WorkingSetSize
//	m_myThis->Exelist.SetItemText(0,1,L"��������");
////	m_myThis->m_NewVecExeList.push_back(exeName);
////	m_myThis->m_NumFlag--;
// 	//if (m_myThis->m_VecExeList[m_myThis->m_NumFlag]!=exeName)
// 	//{
//
// 	//}
//	//_tprintf(L"%s\n",str);
//	//GetClassName(hwnd,str1,MAX_PATH);
//	
//	return TRUE;
//	
//}
// int showexe::m_Num=0;
void showexe::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CDialogEx::OnTimer(nIDEvent);
	//m_NumFlag=m_VecExeList.size();;
	Exelist.DeleteAllItems();
	//m_NewVecExeList.clear();//���vec
	EnumWindows(PROC1,0);
//	m_VecExeList=m_NewVecExeList;//���¸�ֵ
}

