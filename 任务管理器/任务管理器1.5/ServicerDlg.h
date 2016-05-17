#pragma once
#include "afxcmn.h"
#include <vector>
#include <winsvc.h>
using namespace std;

// ServicerDlg 对话框

class ServicerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ServicerDlg)

public:
	ServicerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ServicerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ServiceList;
	vector<LPENUM_SERVICE_STATUS_PROCESS> m_VecServiceList;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	bool isInitEnd;
	TCHAR *TheCurrentState(DWORD TheState);//状态
	TCHAR *TheServiceType(DWORD TheType);//当前类型
	TCHAR *TheStartType(DWORD TheType);//启动类型
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ServiceIndex();
};
