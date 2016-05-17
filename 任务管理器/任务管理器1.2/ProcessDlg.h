#pragma once
#include "afxcmn.h"
#include <vector>
#include <tlhelp32.h>
using namespace std;
// ProcessDlg 对话框

class ProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessDlg)

public:
	ProcessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ProcessDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ProList;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL isInitEnd;
	vector<PROCESSENTRY32> m_VecProList;
	vector<PROCESSENTRY32> m_NewVecProList;
	PROCESSENTRY32 stcInfo;
	vector<MODULEENTRY32> m_VecModList;
	vector<THREADENTRY32> m_VecThdList;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	void ProcessIndex();//进程索引
//	void RefreshProcessAdd();//实时刷新增加进程
	void RefreshProcess();//实时刷新减少进程
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool isNeedUpdateView();

	//vector<PROCESSENTRY32> m_incVecProList;
	//vector<PROCESSENTRY32> m_decVecProList;
	//vector<int>			   m_decVecProListIndex;

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void KillProcess();
public:
	int GetCpuNumber();//获取cpu数目
	int GetProcessTime(const FILETIME& time);//获取进程时间
	double Get_cpu_usage(DWORD ProPid);//获取cpu占用率
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void RealTime();

	void SetProcessPriority(int PriorityFlag);//判断优先级并设置
	TCHAR* InitializePriority(DWORD handle);//初始化进程优先级
	afx_msg void high();
	afx_msg void AboveNormal();
	afx_msg void normal();
	afx_msg void BelowNormal();
	afx_msg void low();
};
