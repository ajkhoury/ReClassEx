#pragma once

#include "afxwin.h"
#include "Resource.h"

// CDialogProcSelect dialog

class CDialogProcSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProcSelect)
public:
	// Dialog Data
	enum { IDD = IDD_DIALOGPROCSELECT };

	struct ProcessInfoStack
	{ 
		DWORD ProcessId; 
		CString Procname; 
	};

	// standard constructor
	CDialogProcSelect( CWnd* pParent = NULL )
		: CDialogEx( CDialogProcSelect::IDD, pParent ), m_bLoadingProcesses( false )
	{ }
	
	virtual ~CDialogProcSelect() { }

	void RefreshRunningProcesses();

protected:
	//Controls
	CListCtrl m_ProcessList;
	CRect m_OriginalSize;
	CImageList m_ProcessIcons;

	//Misc
	std::vector<ProcessInfoStack> m_ProcessInfos;
	volatile bool m_bLoadingProcesses;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnAttachButton( );
	afx_msg void OnRefreshButton( );
	afx_msg void OnDblclkListControl( NMHDR* pNMHDR, LRESULT* pResult );
};
