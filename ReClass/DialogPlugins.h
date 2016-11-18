#pragma once

#include "afxwin.h"
#include "Resource.h"
#include "PluginAPI.h"

// CDialogPlugins dialog

class CDialogPlugins : public CDialogEx
{
	DECLARE_DYNAMIC( CDialogPlugins )

public:
	// standard constructor
	CDialogPlugins( CWnd* pParent = NULL ) : CDialogEx( CDialogPlugins::IDD, pParent ) { }
	// virtual deconstructor
	virtual ~CDialogPlugins( ) { }

	// Dialog Data
	enum { IDD = IDD_DIALOG_PLUGINS, MENU_SETTINGS = 10010, MENU_ABOUT = 10011, MENU_STATECHANGE = 10012 };

protected:
	virtual void DoDataExchange( CDataExchange *pDX );    // DDX/DDV support
	virtual BOOL OnInitDialog( );
	virtual void OnContextMenu( CWnd *pWnd, CPoint pos );

	DECLARE_MESSAGE_MAP( )

	afx_msg void OnPopupMenuSettings( );
	afx_msg void OnPopupMenuAbout( );
	afx_msg void OnPopupMenuChangeState( );

private:

	PRECLASS_PLUGIN GetSelectedPlugin( ) { return g_LoadedPlugins.at(m_PluginsList.GetSelectionMark( )); }
	VOID RefreshPlugins( );

	CListCtrl m_PluginsList;
};
