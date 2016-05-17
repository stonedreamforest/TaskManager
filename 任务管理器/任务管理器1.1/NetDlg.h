#pragma once


// NetDlg 对话框

class NetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NetDlg)

public:
	NetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
