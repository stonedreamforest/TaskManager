#pragma once
#include "afxcmn.h"
#include <tlhelp32.h>
#include <vector>
using namespace std;

// SeeThread �Ի���

class SeeThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SeeThreadDlg)

public:
	SeeThreadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SeeThreadDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP();
public:
	UINT m_currPid;
	TCHAR *m_StrThread;
	void initThreadList();
	vector<THREADENTRY32> m_VecThdList;
	virtual BOOL OnInitDialog();
	CListCtrl m_ThreadList;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	bool isThread;
	afx_msg void KillThread();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
