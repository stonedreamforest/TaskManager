#pragma once
#include "afxcmn.h"
#include <vector>
#include <tlhelp32.h>
using namespace std;
// ProcessDlg 对话框
#include "SeeThreadDlg.h"
#include "SeeMoudleDlg.h"
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
	
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//以下可以用结构体 保存的  同样地 沃森虐了 森虐了。。。。。。。
	vector<unsigned long> m_VecMemory;//内存 //用于对比刷新 下同
	vector<unsigned long> m_NewVecMemory;
	vector<TCHAR*> m_VecPriority;//游戏级
	vector<TCHAR*> m_NewVecPriority;//游戏级
	vector<double> m_VevCpu;
	vector<double> m_NewVevCpu;
public:
	void ProcessIndex();//进程索引
//	void RefreshProcessAdd();//实时刷新增加进程
	void RefreshProcess();//实时刷新进程
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//vector<PROCESSENTRY32> m_incVecProList;
	//vector<PROCESSENTRY32> m_decVecProList;
	//vector<int>			   m_decVecProListIndex;

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void KillProcess();
public:
	int GetCpuNumber();//获取cpu数目
	__int64 FileTimeToInt64(const FILETIME& time);//获取进程时间
	double  Get_cpu_usage(DWORD ProPid);//获取cpu占用率
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
	afx_msg void SeeThread();



	SeeThreadDlg ThreadDLG;//线程
	bool isFirstShow;
	
	SeeMoudleDlg MoudleDLG;//模块
	bool isFirstModShow;
	afx_msg void SeeMoudle();
	afx_msg void SearchFileInformation();
	afx_msg void OpenFilePath();
	//afx_msg void CopyFileName();
};

