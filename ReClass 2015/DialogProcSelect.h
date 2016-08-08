#pragma once

#include "afxwin.h"
#include "Resource.h"

// CDialogProcSelect dialog

class CDialogProcSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProcSelect)
public:
	CDialogProcSelect(CWnd* pParent = NULL);
	virtual ~CDialogProcSelect();

	// Dialog Data
	enum { IDD = IDD_DIALOGPROCSELECT };

	void ListRunningProcs( );

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnClose( );
	afx_msg void OnAttachButton();
	afx_msg void OnRefreshButton();
	afx_msg void OnDblClkListControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);

private:
	enum
	{
		COLUMN_PROCESSNAME = 0,
		COLUMN_PROCESSID,
		NUM_OF_COLUMNS
	};

	struct ProcessInfoStack
	{
		DWORD ProcessId;
		CString Procname;
	};

	typedef struct COMPARESTRUCT 
	{
		CListCtrl* pListCtrl;
		int iColumn;
		bool bAscending;
	} *LPCOMPARESTRUCT;
	
	static int CALLBACK CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	//Controls
	CListCtrl m_ProcessList;
	CButton m_FilterCheck;
	bool m_bSortAscendingName;
	bool m_bSortAscendingId;

	CRect m_OriginalSize;
	CImageList m_ProcessIcons;

	//Misc
	std::vector<ProcessInfoStack> m_ProcessInfos;
	volatile bool m_bLoadingProcesses;

	// Annoying processes to filter out that you probably won't be looking in the memory of
	// Used unicode because its faster to compare process name from UNICODE_STRING buffer without converting to multibyte
	static std::vector<const wchar_t*> CommonProcesses;
};
