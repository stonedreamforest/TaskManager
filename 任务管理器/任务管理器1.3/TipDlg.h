#pragma once


// TipDlg �Ի���

class TipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TipDlg)

public:
	TipDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TipDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
