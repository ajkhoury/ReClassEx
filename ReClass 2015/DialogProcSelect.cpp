// DialogProcSelect.cpp : implementation file
//

#include "stdafx.h"
#include "DialogProcSelect.h"
#include "ReClass2015.h"
#include "afxdialogex.h"

// CDialogProcSelect dialog

IMPLEMENT_DYNAMIC(CDialogProcSelect, CDialogEx)

BEGIN_MESSAGE_MAP(CDialogProcSelect, CDialogEx)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

void CDialogProcSelect::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_PROCESS_LIST, m_ProcessList);
	CDialogEx::DoDataExchange(pDX);
}

BOOL CDialogProcSelect::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetWindowRect(&m_OriginalSize);
	ScreenToClient(&m_OriginalSize);
	return TRUE;
}

void CDialogProcSelect::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	if (!m_OriginalSize.IsRectNull())
	{
		lpMMI->ptMinTrackSize.x = m_OriginalSize.Width();
		lpMMI->ptMinTrackSize.y = m_OriginalSize.Height();
		lpMMI->ptMaxTrackSize.x = m_OriginalSize.Width();
	}

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
