#pragma once
#include "afxcmn.h"
#include <tlhelp32.h>
#include <vector>
using namespace std;

// SeeThread 对话框

class SeeThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SeeThreadDlg)

public:
	SeeThreadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SeeThreadDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
