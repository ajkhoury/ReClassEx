#pragma once

#include "afxwin.h"
#include "Resource.h"

// CDialogPlugins dialog

class CDialogPlugins : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPlugins)

public:
	CDialogPlugins(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogPlugins();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLUGINS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnGetMinMaxInfo( MINMAXINFO *lpinfo );

	DECLARE_MESSAGE_MAP( )

	afx_msg void OnRightClickList( NMHDR * pNotifyStruct, LRESULT * result );
private:
	void RefreshPlugins( );

	CRect m_OriginalSize;
	CListCtrl m_PluginsList;
};
