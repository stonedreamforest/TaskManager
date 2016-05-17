
// 任务管理器Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "MyTabCtrl.h"


// C任务管理器Dlg 对话框
class C任务管理器Dlg : public CDialogEx
{
// 构造
public:
	C任务管理器Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CMyTabCtrl m_Tab;

public:
	void AccessPrivileges();//获取特权如 关机 注销...

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	bool m_IsWindowHide;
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
//	afx_msg void OnUpdateAfxIdsUpdateMenu(CCmdUI *pCmdUI);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
