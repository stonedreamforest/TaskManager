
// ���������Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "MyTabCtrl.h"


// C���������Dlg �Ի���
class C���������Dlg : public CDialogEx
{
// ����
public:
	C���������Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CMyTabCtrl m_Tab;

public:
	void AccessPrivileges();//��ȡ��Ȩ�� �ػ� ע��...

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
