#pragma once
#include "afxcmn.h"
#include <vector>
#include <tlhelp32.h>
using namespace std;

// SeeMoudleDlg 对话框

class SeeMoudleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SeeMoudleDlg)

public:
	SeeMoudleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SeeMoudleDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void initMoudleList();
	vector<MODULEENTRY32> m_VecModList;
//	CListCtrl m_VecModList;
	CListCtrl m_MoudleList;
	UINT m_ProCurrPid;
	TCHAR *m_StrMoudle;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	bool isInitEnd;
	afx_msg void KillMoudle();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
