// MyTabCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���������.h"
#include "MyTabCtrl.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{
	m_dlgCount = 0;
}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()

void CMyTabCtrl::addDlg(DWORD count,...)
{
	va_list vl;
	va_start(vl,count);
	for (DWORD i=0;i<count;i++)
	{
		UINT uid=va_arg(vl,UINT);
		m_processDlg[i]=va_arg(vl,CDialogEx*);
		m_processDlg[i]->Create(uid,this);
		m_dlgCount++;
	}
	va_end(vl);
}

void CMyTabCtrl::setshow(int sel)
{
 	CRect rect;
	GetClientRect(&rect);	
 	rect.DeflateRect(-11,22,1,1);//��У����

	m_processDlg[sel]->MoveWindow(rect);
	m_processDlg[sel]->ShowWindow(SW_SHOW);
}



// CMyTabCtrl ��Ϣ�������



void CMyTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD sel=this->GetCurSel();
	for (DWORD i=0;i<m_dlgCount;i++)
	{
		if (sel==i)
		{
			setshow(i);
		}
		else
		{
			m_processDlg[i]->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
