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

private:
	enum
	{
		COLUMN_MODULE = 0,
		COLUMN_START,
		COLUMN_END,
		COLUMN_SIZE,
		NUM_OF_COLUMNS
	};
	static TCHAR* s_ColumnInfo[4];

public:
	CImageList				m_ImageList;
	CListCtrl				m_ModuleViewList;
	CEdit					m_Edit;
	CString					m_Filter;
	std::vector<HICON>		m_hModuleIcons;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual int AddData(int Index, LPTSTR ModuleName, LPTSTR StartAddress, LPTSTR EndAddress, LPTSTR ModuleSize, LPARAM lParam);
	virtual void BuildList();

	afx_msg void OnEnChangeModuleName();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};