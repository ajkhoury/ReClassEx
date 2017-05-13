#pragma once

#include <afxwin.h>

#include "CProgressBar.h"

#define TIMER_MEMORYMAP_UPDATE 0xDEADF00D

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC( CMainFrame )
public:
	CMainFrame( );

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

// Implementation
public:
	virtual ~CMainFrame( );

	#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
	#endif

public:  // control bar embedded members
	CMFCRibbonBar m_RibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;

	static UINT BASED_CODE s_StatusBarPanes[2];
	CStatusBar m_StatusBar;

	void OnLoadSymbols( );


// Generated message map functions
protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnWindowManager( );
	afx_msg void OnApplicationLook( UINT id );
	afx_msg void OnUpdateApplicationLook( CCmdUI* pCmdUI );
	afx_msg void OnSettingChange( UINT uFlags, LPCTSTR lpszSection );
	DECLARE_MESSAGE_MAP( )

	BOOL CreateDockingWindows( );
	void SetDockingWindowIcons( BOOL bHiColorIcons );

public:
	//afx_msg void OnButtonShowclasses();
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
	afx_msg void OnButtonClrBackground( );
	afx_msg void OnButtonClrSelect( );
	afx_msg void OnButtonClrHidden( );
	afx_msg void OnButtonClrOffset( );
	afx_msg void OnButtonClrAddress( );
	afx_msg void OnButtonClrHex( );
	afx_msg void OnButtonClrType( );
	afx_msg void OnButtonClrName( );
	afx_msg void OnButtonClrValue( );
	afx_msg void OnButtonClrIndex( );
	afx_msg void OnButtonClrComment( );
	afx_msg void OnButtonClrText( );
	afx_msg void OnButtonClrVtable( );
	afx_msg void OnButtonClrFunction( );
	afx_msg void OnButtonClrCustom( );
	afx_msg void OnCheckAddress( );
	afx_msg void OnUpdateCheckAddress( CCmdUI *pCmdUI );
	afx_msg void OnCheckOffset( );
	afx_msg void OnUpdateCheckOffset( CCmdUI *pCmdUI );
	afx_msg void OnCheckText( );
	afx_msg void OnUpdateCheckText( CCmdUI *pCmdUI );
	afx_msg void OnCheckRtti( );
	afx_msg void OnUpdateCheckRtti( CCmdUI *pCmdUI );
	afx_msg void OnCheckRandomWindowName( );
	afx_msg void OnUpdateCheckRandomWindowName( CCmdUI *pCmdUI );
	afx_msg void OnButtonTypedef( );
	afx_msg void OnButtonSelectProcess( );
	afx_msg void OnButtonEditClass( );
	afx_msg void OnUpdateButtonEditClass( CCmdUI *pCmdUI );
	afx_msg void OnButtonDeleteClass( );
	afx_msg void OnUpdateButtonDeleteClass( CCmdUI *pCmdUI );
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnCheckTopmost( );
	afx_msg void OnUpdateCheckTopmost( CCmdUI *pCmdUI );
	afx_msg void OnCheckClassBrowser( );
	afx_msg void OnUpdateCheckClassBrowser( CCmdUI *pCmdUI );
	afx_msg void OnCheckPrivatePadding( );
	afx_msg void OnUpdateCheckPrivatePadding( CCmdUI *pCmdUI );
	afx_msg void OnCheckClipboardCopy( );
	afx_msg void OnUpdateCheckClipboardCopy( CCmdUI *pCmdUI );
	afx_msg void OnButtonLeft( );
	afx_msg void OnButtonRight( );
	afx_msg void OnCheckFloat( );
	afx_msg void OnUpdateCheckFloat( CCmdUI *pCmdUI );
	afx_msg void OnCheckInteger( );
	afx_msg void OnUpdateCheckInteger( CCmdUI *pCmdUI );
	afx_msg void OnCheckString( );
	afx_msg void OnUpdateCheckString( CCmdUI *pCmdUI );
	afx_msg void OnCheckPointer( );
	afx_msg void OnUpdateCheckPointer( CCmdUI *pCmdUI );
	afx_msg void OnCheckUnsignedHex( );
	afx_msg void OnUpdateCheckUnsignedHex( CCmdUI *pCmdUI );
};


