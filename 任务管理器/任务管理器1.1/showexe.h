#pragma once
#include "afxcmn.h"


// showexe 对话框

class showexe : public CDialogEx
{
	DECLARE_DYNAMIC(showexe)

public:
	showexe(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~showexe();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl Exelist;
	virtual BOOL OnInitDialog();
	BOOL isInitEnd;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	static BOOL CALLBACK PROC1(HWND hwnd,LPARAM lparam);

	static showexe * m_myThis; //为了static回调函数使用成员变量。
};
