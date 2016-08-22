#pragma once

#include "afxwin.h"

class CDialogModules : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogModules)
public:
	CDialogModules(CWnd* pParent = NULL);
	virtual ~CDialogModules();

	enum { IDD = IDD_DIALOG_MODULES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeModuleName();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClkListControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpinfo);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);

private:
	enum
	{
		COLUMN_MODULE = 0,
		COLUMN_START,
		COLUMN_END,
		COLUMN_SIZE,
		NUM_OF_COLUMNS
	};

	int FindModuleByName(const TCHAR* szName);
	CNodeClass* GetClassByName(const TCHAR* szClassName);

	typedef struct COMPARESTRUCT {
		CListCtrl* pListCtrl;
		int iColumn;
		bool bAscending;
	} *LPCOMPARESTRUCT;

	static int CALLBACK CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

//public:
	CImageList				m_ModuleIcons;
	
	CListCtrl				m_ModuleList;
	bool					m_bSortAscendingName;
	bool					m_bSortAscendingStart;
	bool					m_bSortAscendingEnd;
	bool					m_bSortAscendingSize;

	CEdit					m_Edit;
	CString					m_Filter;
	std::vector<HICON>		m_hModuleIcons;
	CRect					m_OriginalSize;
	void SetSelected();

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual int AddData(int Index, LPTSTR ModuleName, LPTSTR StartAddress, LPTSTR EndAddress, LPTSTR ModuleSize, LPARAM lParam);
	virtual void BuildList();
};