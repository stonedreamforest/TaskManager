#pragma once
#include "afxcmn.h"
#include <vector>
#include <winsvc.h>
using namespace std;

// ServicerDlg �Ի���

class ServicerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ServicerDlg)

public:
	ServicerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ServicerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ServiceList;
	vector<LPENUM_SERVICE_STATUS_PROCESS> m_VecServiceList;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	bool isInitEnd;
	TCHAR *TheCurrentState(DWORD TheState);//״̬
	TCHAR *TheServiceType(DWORD TheType);//��ǰ����
	TCHAR *TheStartType(DWORD TheType);//��������
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ServiceIndex();
};
