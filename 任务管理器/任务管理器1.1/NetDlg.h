#pragma once


// NetDlg �Ի���

class NetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NetDlg)

public:
	NetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
