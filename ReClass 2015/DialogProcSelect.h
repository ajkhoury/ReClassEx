#pragma once

#include "afxwin.h"
#include "Resource.h"

// CDialogProcSelect dialog

class CDialogProcSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProcSelect)

public:
	// standard constructor
	CDialogProcSelect(CWnd* pParent = NULL)
		: CDialogEx(CDialogProcSelect::IDD, pParent)
	{ }
	
	virtual ~CDialogProcSelect() { }

// Dialog Data
	enum { IDD = IDD_DIALOGPROCSELECT };

protected:
	CListCtrl m_ProcessList;
	CRect m_OriginalSize;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
