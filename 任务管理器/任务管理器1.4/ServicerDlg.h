#pragma once


// ServicerDlg �Ի���

class ServicerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ServicerDlg)

public:
	ServicerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ServicerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
