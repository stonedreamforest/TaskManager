#pragma once
#include "afxcmn.h"
#include <vector>
#include <tlhelp32.h>
using namespace std;
// ProcessDlg �Ի���

class ProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessDlg)

public:
	ProcessDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ProcessDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	void ProcessIndex();//��������
//	void RefreshProcessAdd();//ʵʱˢ�����ӽ���
	void RefreshProcess();//ʵʱˢ�¼��ٽ���
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool isNeedUpdateView();

	//vector<PROCESSENTRY32> m_incVecProList;
	//vector<PROCESSENTRY32> m_decVecProList;
	//vector<int>			   m_decVecProListIndex;

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void KillProcess();
public:
	int GetCpuNumber();//��ȡcpu��Ŀ
	int GetProcessTime(const FILETIME& time);//��ȡ����ʱ��
	double Get_cpu_usage(DWORD ProPid);//��ȡcpuռ����
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void RealTime();

	void SetProcessPriority(int PriorityFlag);//�ж����ȼ�������
	TCHAR* InitializePriority(DWORD handle);//��ʼ���������ȼ�
	afx_msg void high();
	afx_msg void AboveNormal();
	afx_msg void normal();
	afx_msg void BelowNormal();
	afx_msg void low();
};
