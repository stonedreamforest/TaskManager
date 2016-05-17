#pragma once


// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CDialogEx *m_processDlg[10];
	void addDlg(DWORD count,...);
	void setshow(int sel);
	DWORD m_dlgCount;
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


