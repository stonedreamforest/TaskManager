#pragma once


// TipDlg 对话框

class TipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TipDlg)

public:
	TipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TipDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
