#pragma once
#include "afxcmn.h"


// showexe �Ի���

class showexe : public CDialogEx
{
	DECLARE_DYNAMIC(showexe)

public:
	showexe(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~showexe();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl Exelist;
	virtual BOOL OnInitDialog();
	BOOL isInitEnd;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	static BOOL CALLBACK PROC1(HWND hwnd,LPARAM lparam);

	static showexe * m_myThis; //Ϊ��static�ص�����ʹ�ó�Ա������
};
