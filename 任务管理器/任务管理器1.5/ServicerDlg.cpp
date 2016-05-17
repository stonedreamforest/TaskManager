// ServicerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���������.h"
#include "ServicerDlg.h"
#include "afxdialogex.h"
#include <winsvc.h>
#include <vector>

// ServicerDlg �Ի���

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


// ServicerDlg ��Ϣ�������


BOOL ServicerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CRect rect;
	m_ServiceList.GetClientRect(rect);
	m_ServiceList.SetExtendedStyle(m_ServiceList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//��ѡ�з��
	CString str[] ={_T(""), _T("����"), _T("��������"), _T("��ǰ����"), _T("��������"),  _T("�û���"), _T("������"),_T("·��")};

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
		m_ServiceList.DeleteColumn(0);//ɾ����0�� �������־���

		ServiceIndex();
	isInitEnd=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ˴������Ϣ����������
}

TCHAR * ServicerDlg::TheCurrentState(DWORD TheState)
{
	switch (TheState)
	{
	case SERVICE_CONTINUE_PENDING:
		return _T("��������");
	case SERVICE_PAUSE_PENDING:
		return _T("��ͣ����");
	case SERVICE_PAUSED:
		return _T("��ͣ");
	case SERVICE_RUNNING:
		return _T("��������");
	case SERVICE_START_PENDING:
		return _T("��������");
	case SERVICE_STOP_PENDING:
		return _T("����ֹͣ");
	default:
		break;
	}
	return _T("δ����");
}

TCHAR * ServicerDlg::TheServiceType(DWORD TheType)
{
	switch (TheType)
	{
	case SERVICE_FILE_SYSTEM_DRIVER:
		return _T("�ļ�ϵͳ��������");
	case SERVICE_KERNEL_DRIVER:
		return _T("�豸��������");
	case SERVICE_WIN32_OWN_PROCESS:
		return _T("�÷������������Լ��Ĺ�����");
	default:
		break;
	}
	return _T("�÷���������������һ������");
}

TCHAR * ServicerDlg::TheStartType(DWORD TheType)
{
	switch (TheType)
	{
	case SERVICE_AUTO_START:
		return _T("������ƹ�������ϵͳ����ʱ�Զ���������");
	case SERVICE_BOOT_START:
		return _T("��ϵͳװ�س����������豸��������,��ֵ�����������������");
	case SERVICE_DEMAND_START:
		return _T("����ʼ�ɷ�����ƹ�����,��һ�����̵���startService����");
	case SERVICE_DISABLED:
		return _T("���������ķ���.������������,���´���Ĵ���error_service_disabled");
	default:
		break;
	}
	return _T("��ioinitsystem���������豸�������򡣴�ֵ�����������������");
}


void ServicerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (bShow==FALSE)
	{
		return;
	}
	ServiceIndex();//�л�����ˢ��
	// TODO: �ڴ˴������Ϣ����������
}

void ServicerDlg::ServiceIndex()
{
	SC_HANDLE hSCM=OpenSCManager(NULL,NULL,SC_MANAGER_ENUMERATE_SERVICE);
	DWORD dwServiceNum=0;
	DWORD dwSize=0;
	EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,//���з���״̬
		NULL,0,&dwSize,&dwServiceNum,
		NULL,NULL);
	//������Ҫ���ڴ� �ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice=(LPENUM_SERVICE_STATUS_PROCESS)LocalAlloc(LPTR,dwSize);
	//�ڶ���ö��
	BOOL bStattus=EnumServicesStatusEx(hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,//���з���״̬
		(PBYTE)pEnumSerice,dwSize,&dwSize,&dwServiceNum,
		NULL,NULL);
	//������Ϣ
	for (DWORD i=0;i<dwServiceNum;i++)
	{
		//��ȡ������Ϣ

		//	pEnumSerice[i].lpDisplayName;
		//	pEnumSerice[i].lpServiceName;//������
		//pEnumSerice[i].ServiceStatusProcess.dwCurrentState;//����״̬
		//pEnumSerice[i].ServiceStatusProcess.dwServiceType;//�����ִ������
		TCHAR *ServiceType=TheServiceType(pEnumSerice[i].ServiceStatusProcess.dwServiceType);

		//	_stprintf_s(ServiceType,_T("%d"),);
		//��ȡ��ǰ����
		TCHAR* CurrentState=TheCurrentState(pEnumSerice[i].ServiceStatusProcess.dwCurrentState);
		//	_stprintf_s(CurrentState,_T("%d"),);
		m_ServiceList.InsertItem(0,pEnumSerice[i].lpServiceName);
		m_ServiceList.SetItemText(0,1,ServiceType);
		m_ServiceList.SetItemText(0,2,CurrentState);
		//��ȡ����������ϸ��Ϣ
		SC_HANDLE hService=OpenService(hSCM,pEnumSerice[i].lpServiceName,SERVICE_QUERY_CONFIG);

		//��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService,NULL,0,&dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig=(LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR,dwSize);
		//�ڶ��ε��û�ȡ��Ϣ
		QueryServiceConfig(hService,pServiceConfig,dwSize,&dwSize);
		//ͨ�������ȡ���Ľṹ����Ϣ���嵽��Ҫ��ֵ

		TCHAR *StartType=TheStartType(pServiceConfig->dwStartType);;//��ȡ��������
		//_stprintf_s(StartType,_T("%d"),);
		//	_stprintf_s(StartType,_T("%d"),pServiceConfig->dwStartType);
		m_ServiceList.SetItemText(0,3,StartType);
		m_ServiceList.SetItemText(0,4,pServiceConfig->lpLoadOrderGroup);
		m_ServiceList.SetItemText(0,5,pServiceConfig->lpServiceStartName);
		m_ServiceList.SetItemText(0,6,pServiceConfig->lpBinaryPathName);


		m_VecServiceList.push_back(pEnumSerice+i);
	}
}
