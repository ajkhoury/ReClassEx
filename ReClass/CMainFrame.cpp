#include "stdafx.h"

#include <afxtabctrl.h>

#include "CMainFrame.h"
#include "CClassFrame.h"


#include "DialogClasses.h"
#include "DialogProcSelect.h"
#include "DialogTypes.h"

UINT BASED_CODE CMainFrame::s_StatusBarPanes[2] = { 
	ID_STATUSBAR_PANE1,
	ID_STATUSBAR_PANE2
};

// CMainFrame
IMPLEMENT_DYNAMIC( CMainFrame, CMDIFrameWndEx )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWndEx )
	ON_WM_TIMER( )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_SETTINGCHANGE( )
	ON_COMMAND( ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager )
	ON_COMMAND_RANGE( ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook )
	ON_UPDATE_COMMAND_UI_RANGE( ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook )
	//ON_COMMAND(ID_BUTTON_SHOWCLASSES, &CMainFrame::OnButtonShowclasses)
	ON_COMMAND( ID_BUTTON_CLR_BACKGROUND, &CMainFrame::OnButtonClrBackground )
	ON_COMMAND( ID_BUTTON_CLR_SELECT, &CMainFrame::OnButtonClrSelect )
	ON_COMMAND( ID_BUTTON_CLR_HIDDEN, &CMainFrame::OnButtonClrHidden )
	ON_COMMAND( ID_BUTTON_CLR_OFFSET, &CMainFrame::OnButtonClrOffset )
	ON_COMMAND( ID_BUTTON_CLR_ADDRESS, &CMainFrame::OnButtonClrAddress )
	ON_COMMAND( ID_BUTTON_CLR_HEX, &CMainFrame::OnButtonClrHex )
	ON_COMMAND( ID_BUTTON_CLR_TYPE, &CMainFrame::OnButtonClrType )
	ON_COMMAND( ID_BUTTON_CLR_NAME, &CMainFrame::OnButtonClrName )
	ON_COMMAND( ID_BUTTON_CLR_VALUE, &CMainFrame::OnButtonClrValue )
	ON_COMMAND( ID_BUTTON_CLR_INDEX, &CMainFrame::OnButtonClrIndex )
	ON_COMMAND( ID_BUTTON_CLR_COMMENT, &CMainFrame::OnButtonClrComment )
	ON_COMMAND( ID_BUTTON_CLR_TEXT, &CMainFrame::OnButtonClrText )
	ON_COMMAND( ID_BUTTON_CLR_VTABLE, &CMainFrame::OnButtonClrVtable )
	ON_COMMAND( ID_BUTTON_CLR_FUNCTION, &CMainFrame::OnButtonClrFunction )
	ON_COMMAND( ID_BUTTON_CLR_CUSTOM, &CMainFrame::OnButtonClrCustom )
	ON_COMMAND( ID_CHECK_ADDRESS, &CMainFrame::OnCheckAddress )
	ON_UPDATE_COMMAND_UI( ID_CHECK_ADDRESS, &CMainFrame::OnUpdateCheckAddress )
	ON_COMMAND( ID_CHECK_OFFSET, &CMainFrame::OnCheckOffset )
	ON_UPDATE_COMMAND_UI( ID_CHECK_OFFSET, &CMainFrame::OnUpdateCheckOffset )
	ON_COMMAND( ID_BUTTON_TYPEDEF, &CMainFrame::OnButtonTypedef )
	ON_COMMAND( ID_CHECK_TEXT, &CMainFrame::OnCheckText )
	ON_UPDATE_COMMAND_UI( ID_CHECK_TEXT, &CMainFrame::OnUpdateCheckText )
	ON_COMMAND( ID_CHECK_RTTI, &CMainFrame::OnCheckRtti )
	ON_UPDATE_COMMAND_UI( ID_CHECK_RTTI, &CMainFrame::OnUpdateCheckRtti )
	ON_COMMAND( ID_CHECK_RANDOM_NAME, &CMainFrame::OnCheckRandomWindowName )
	ON_UPDATE_COMMAND_UI( ID_CHECK_RANDOM_NAME, &CMainFrame::OnUpdateCheckRandomWindowName )
	//ON_COMMAND(ID_BUTTON_SELECT, &CMainFrame::OnButtonSelect)
	ON_COMMAND( ID_BUTTON_SELECTPROCESS, &CMainFrame::OnButtonSelectProcess )
	ON_COMMAND( ID_BUTTON_EDITCLASS, &CMainFrame::OnButtonEditClass )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_EDITCLASS, &CMainFrame::OnUpdateButtonEditClass )
	ON_COMMAND( ID_BUTTON_DELETECLASS, &CMainFrame::OnButtonDeleteClass )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_DELETECLASS, &CMainFrame::OnUpdateButtonDeleteClass )
	ON_COMMAND( ID_CHECK_TOPMOST, &CMainFrame::OnCheckTopmost )
	ON_UPDATE_COMMAND_UI( ID_CHECK_TOPMOST, &CMainFrame::OnUpdateCheckTopmost )
	ON_COMMAND( ID_CHECK_CLASSBROWSER, &CMainFrame::OnCheckClassBrowser )
	ON_UPDATE_COMMAND_UI( ID_CHECK_CLASSBROWSER, &CMainFrame::OnUpdateCheckClassBrowser )
	ON_COMMAND( ID_BUTTON_LEFT, &CMainFrame::OnButtonLeft )
	ON_COMMAND( ID_BUTTON_RIGHT, &CMainFrame::OnButtonRight )
	ON_COMMAND( ID_CHECK_FLOAT, &CMainFrame::OnCheckFloat )
	ON_UPDATE_COMMAND_UI( ID_CHECK_FLOAT, &CMainFrame::OnUpdateCheckFloat )
	ON_COMMAND( ID_CHECK_INTEGER, &CMainFrame::OnCheckInteger )
	ON_UPDATE_COMMAND_UI( ID_CHECK_INTEGER, &CMainFrame::OnUpdateCheckInteger )
	ON_COMMAND( ID_CHECK_STRING, &CMainFrame::OnCheckString )
	ON_UPDATE_COMMAND_UI( ID_CHECK_STRING, &CMainFrame::OnUpdateCheckString )
	ON_COMMAND( ID_CHECK_POINTER, &CMainFrame::OnCheckPointer )
	ON_UPDATE_COMMAND_UI( ID_CHECK_POINTER, &CMainFrame::OnUpdateCheckPointer )
	ON_COMMAND( ID_CHECK_UNSIGNEDHEX, &CMainFrame::OnCheckUnsignedHex )
	ON_UPDATE_COMMAND_UI( ID_CHECK_UNSIGNEDHEX, &CMainFrame::OnUpdateCheckUnsignedHex )
	ON_COMMAND( ID_CHECK_CLIP_COPY, &CMainFrame::OnCheckClipboardCopy )
	ON_UPDATE_COMMAND_UI( ID_CHECK_CLIP_COPY, &CMainFrame::OnUpdateCheckClipboardCopy )
	ON_COMMAND( ID_CHECK_PRIVATE_PADDING, &CMainFrame::OnCheckPrivatePadding )
	ON_UPDATE_COMMAND_UI( ID_CHECK_PRIVATE_PADDING, &CMainFrame::OnUpdateCheckPrivatePadding )
END_MESSAGE_MAP( )

// CMainFrame construction/destruction
CMainFrame::CMainFrame( )
{
	// TODO: add member initialization code here
	g_ReClassApp.m_nAppLook = g_ReClassApp.GetInt( _T( "ApplicationLook" ), ID_VIEW_APPLOOK_OFF_2007_BLACK );
}

CMainFrame::~CMainFrame( )
{
}

//DWORD WINAPI LoadModuleSymbolsThread( LPVOID lpThreadParameter )
//{ 
//	DWORD retval = TRUE;
//	MemMapInfo* pInfo = (MemMapInfo*)lpThreadParameter;
//	if (pInfo)
//	{
//		if (!g_ReClassApp.m_pSymbolLoader->LoadSymbolsForModule( pInfo->Path, pInfo->Start, pInfo->Size ))
//		{
//			PrintOut( _T( "Failed to load symbols for %s" ), pInfo->Name.GetString( ) );
//			retval = FALSE;
//		}
//		delete pInfo;
//	}
//	return retval;
//}

void CMainFrame::OnLoadSymbols( )
{
	CProgressBar progressBar( _T( "Progress" ), 100, 100, TRUE, 0, &m_StatusBar );
	progressBar.SetStep( 1 );
	progressBar.SetText( _T( "Symbols loading: " ) );

	for (ULONG i = 0; i < g_MemMapModules.size( ); i++)
	{
		TCHAR tcsProgressText[256] = { 0 };
		MemMapInfo CurrentModule = g_MemMapModules[i];

		progressBar.SetRange32( 0, g_MemMapModules.size( ) );

		_stprintf_s( tcsProgressText, _T( "[%d/%d] %s" ), i + 1, g_MemMapModules.size( ), CurrentModule.Name.GetString( ) );
		m_StatusBar.SetPaneText( 1, tcsProgressText );

		//MemMapInfo* pCurrentModule = new MemMapInfo( CurrentModule );
		//Utils::NtCreateThread( LoadModuleSymbolsThread, pCurrentModule, 0 );

		if (!g_ReClassApp.m_pSymbolLoader->LoadSymbolsForModule( CurrentModule.Path, CurrentModule.Start, CurrentModule.Size ))
		{
			PrintOut( _T( "Failed to load symbols for %s" ), CurrentModule.Name.GetString( ) );
		}

		progressBar.StepIt( );
		
		// Peek and pump through messages to stop reclass from hanging
		MSG msg;
		while (::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ))
		{
			if (!AfxGetApp( )->PumpMessage( ))
				::PostQuitMessage( 0 );
		}
	}

	m_StatusBar.SetPaneText( 1, _T( "" ) );
}

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CMDIFrameWndEx::OnCreate( lpCreateStruct ) == -1)
		return -1;

	// Set the visual manager and style based on persisted value
	OnApplicationLook( g_ReClassApp.m_nAppLook );

	// Create tabs
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D;
	mdiTabParams.m_tabLocation = CMFCTabCtrl::LOCATION_TOP;
	mdiTabParams.m_nTabBorderSize = 2;
	mdiTabParams.m_bActiveTabCloseButton = TRUE;
	mdiTabParams.m_bTabIcons = TRUE;
	mdiTabParams.m_bAutoColor = TRUE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	mdiTabParams.m_bEnableTabSwap = TRUE;
	mdiTabParams.m_bFlatFrame = TRUE;
	EnableMDITabbedGroups( TRUE, mdiTabParams );

	// Create ribbon bar
	m_RibbonBar.Create( this );
	m_RibbonBar.LoadFromResource( IDR_RIBBON );

	// Create status bar
	m_StatusBar.Create( this );
	m_StatusBar.SetIndicators( s_StatusBarPanes, 2 );
	m_StatusBar.SetPaneInfo( 0, ID_STATUSBAR_PANE1, SBPS_NORMAL, 0 );
	m_StatusBar.SetPaneInfo( 1, ID_STATUSBAR_PANE2, SBPS_STRETCH, 0 );

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode( DT_SMART );
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes( CBRS_ALIGN_TOP );

	// create docking windows
	if (!CreateDockingWindows( ))
	{
		PrintOut( _T( "Failed to create docking windows\n" ) );
		return -1;
	}

	if (g_bRandomName)
	{
		TCHAR tcsRandom[16] = { 0 };
		Utils::GenerateRandomString( tcsRandom, 16 );
		SetTitle( tcsRandom );
	}
	else
	{
		SetTitle( _T( "ReClass 2016" ) );
	}

	// Enable enhanced windows management dialog
	EnableWindowsDialog( ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE );

	// Switch the order of document name and application name on the window title bar. 
	// This improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle( 0, FWS_PREFIXTITLE );

	// Update Colors
	CMFCRibbonColorButton* pColor = NULL;
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_BACKGROUND ));	pColor->SetColor( g_crBackground );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_SELECT ));		pColor->SetColor( g_crSelect );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_HIDDEN ));		pColor->SetColor( g_crHidden );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_OFFSET ));		pColor->SetColor( g_crOffset );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_ADDRESS ));	pColor->SetColor( g_crAddress );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_TYPE ));		pColor->SetColor( g_crType );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_NAME ));		pColor->SetColor( g_crName );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_INDEX ));		pColor->SetColor( g_crIndex );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_VALUE ));		pColor->SetColor( g_crValue );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_COMMENT ));	pColor->SetColor( g_crComment );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_VTABLE ));		pColor->SetColor( g_crVTable );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_FUNCTION ));	pColor->SetColor( g_crFunction );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_TEXT ));		pColor->SetColor( g_crChar );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_CUSTOM ));		pColor->SetColor( g_crCustom );
	pColor = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_HEX ));		pColor->SetColor( g_crHex );

	// update after 5 seconds
	SetTimer( TIMER_MEMORYMAP_UPDATE, 5000, NULL );

	return 0;
}

void CMainFrame::OnSize( UINT nType, int cx, int cy )
{
	CMDIFrameWndEx::OnSize( nType, cx, cy );

	RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_STATUSBAR_PANE2 );

	if (m_StatusBar.GetSafeHwnd( ) && cx > 250)
		m_StatusBar.SetPaneInfo( 0, ID_STATUSBAR_PANE1, SBPS_NORMAL, cx - 250 );
}

void CMainFrame::OnButtonClrBackground( )
{
	g_crBackground = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_BACKGROUND ))->GetColor( );
}
void CMainFrame::OnButtonClrSelect( )
{
	g_crSelect = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_SELECT ))->GetColor( );
}
void CMainFrame::OnButtonClrHidden( )
{
	g_crHidden = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_HIDDEN ))->GetColor( );
}
void CMainFrame::OnButtonClrOffset( )
{
	g_crOffset = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_OFFSET ))->GetColor( );
}
void CMainFrame::OnButtonClrAddress( )
{
	g_crAddress = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_ADDRESS ))->GetColor( );
}
void CMainFrame::OnButtonClrHex( )
{
	g_crHex = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_HEX ))->GetColor( );
}
void CMainFrame::OnButtonClrType( )
{
	g_crType = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_TYPE ))->GetColor( );
}
void CMainFrame::OnButtonClrName( )
{
	g_crName = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_NAME ))->GetColor( );
}
void CMainFrame::OnButtonClrValue( )
{
	g_crValue = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_VALUE ))->GetColor( );
}
void CMainFrame::OnButtonClrIndex( )
{
	g_crIndex = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_INDEX ))->GetColor( );
}
void CMainFrame::OnButtonClrComment( )
{
	g_crComment = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_COMMENT ))->GetColor( );
}
void CMainFrame::OnButtonClrText( )
{
	g_crChar = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_TEXT ))->GetColor( );
}
void CMainFrame::OnButtonClrVtable( )
{
	g_crVTable = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_VTABLE ))->GetColor( );
}
void CMainFrame::OnButtonClrFunction( )
{
	g_crFunction = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_FUNCTION ))->GetColor( );
}
void CMainFrame::OnButtonClrCustom( )
{
	g_crCustom = static_cast<CMFCRibbonColorButton*>(m_RibbonBar.FindByID( ID_BUTTON_CLR_CUSTOM ))->GetColor( );
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	//cs.cy += g_FontHeight;
	if (!CMDIFrameWndEx::PreCreateWindow( cs ))
		return FALSE;
	if (g_bTop)
		cs.dwExStyle |= WS_EX_TOPMOST;
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows( )
{
	SetDockingWindowIcons( g_ReClassApp.m_bHiColorIcons );
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons( BOOL bHiColorIcons )
{
	UpdateMDITabbedBarsIcons( );
}

// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid( ) const
{
	CMDIFrameWndEx::AssertValid( );
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
	CMDIFrameWndEx::Dump( dc );
}
#endif //_DEBUG

// CMainFrame message handlers
void CMainFrame::OnWindowManager( )
{
	ShowWindowsDialog( );
}

void CMainFrame::OnApplicationLook( UINT id )
{
	g_ReClassApp.m_nAppLook = id;
	switch (g_ReClassApp.m_nAppLook)
	{

	case ID_VIEW_APPLOOK_WIN_2000:
	{
		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManager ) );
		m_RibbonBar.SetWindows7Look( FALSE );
	}
	break;

	case ID_VIEW_APPLOOK_OFF_XP:
	{
		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerOfficeXP ) );
		m_RibbonBar.SetWindows7Look( FALSE );
	}
	break;

	case ID_VIEW_APPLOOK_WIN_XP:
	{
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerWindows ) );
		m_RibbonBar.SetWindows7Look( FALSE );
	}
	break;

	case ID_VIEW_APPLOOK_OFF_2003:
	{
		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerOffice2003 ) );
		CDockingManager::SetDockingMode( DT_SMART );
		m_RibbonBar.SetWindows7Look( FALSE );
	}
	break;

	case ID_VIEW_APPLOOK_VS_2005:
	{
		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerVS2005 ) );
		CDockingManager::SetDockingMode( DT_SMART );
		m_RibbonBar.SetWindows7Look( FALSE );
	}
	break;

	case ID_VIEW_APPLOOK_VS_2008:
	{
		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerVS2008 ) );
		CDockingManager::SetDockingMode( DT_SMART );
		m_RibbonBar.SetWindows7Look( FALSE );
	}
	break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
	{
		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerWindows7 ) );
		CDockingManager::SetDockingMode( DT_SMART );
		m_RibbonBar.SetWindows7Look( TRUE );
	}
	break;

	default:
	{
		switch (g_ReClassApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_LunaBlue );
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_ObsidianBlack );
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_Silver );
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_Aqua );
			break;
		}

		CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerOffice2007 ) );
		CDockingManager::SetDockingMode( DT_SMART );
		m_RibbonBar.SetWindows7Look( FALSE );
	}
	break;

	}

	RedrawWindow( NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE );
	g_ReClassApp.WriteInt( _T( "ApplicationLook" ), g_ReClassApp.m_nAppLook );
}

void CMainFrame::OnUpdateApplicationLook( CCmdUI* pCmdUI )
{
	pCmdUI->SetRadio( g_ReClassApp.m_nAppLook == pCmdUI->m_nID );
}

void CMainFrame::OnSettingChange( UINT uFlags, LPCTSTR lpszSection )
{
	CMDIFrameWndEx::OnSettingChange( uFlags, lpszSection );
}

BOOL CMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if (nCode == CN_UPDATE_COMMAND_UI)
	{
		CCmdUI* pCmdUI = (CCmdUI*)pExtra;
		if (nID >= WM_CLASSMENU && nID < (WM_CLASSMENU + WM_MAXITEMS))
		{
			pCmdUI->Enable( TRUE );
			return TRUE;
		}
		else if (nID >= WM_PROCESSMENU && nID < (WM_PROCESSMENU + WM_MAXITEMS))
		{
			pCmdUI->Enable( TRUE );
			return TRUE;
		}
		else if (nID >= WM_DELETECLASSMENU && nID < (WM_DELETECLASSMENU + WM_MAXITEMS))
		{
			pCmdUI->Enable( TRUE );
			return TRUE;
		}
	}

	if (nCode == CN_COMMAND)
	{
		if ((nID >= WM_CLASSMENU) && (nID < (WM_CLASSMENU + WM_MAXITEMS)))
		{
			UINT idx = nID - WM_CLASSMENU;
			
			CClassFrame* pChild = STATIC_DOWNCAST( CClassFrame, CreateNewChild( RUNTIME_CLASS( CClassFrame ), IDR_ReClass2016TYPE, g_ReClassApp.m_hMDIMenu, g_ReClassApp.m_hMDIAccel ) );
			CNodeClass* pClass = g_ReClassApp.m_Classes[idx];

			pChild->SetTitle( pClass->GetName( ) );
			pChild->SetWindowText( pClass->GetName( ) );
			pChild->SetClass( pClass );

			pClass->SetChildFrame( pChild );

			UpdateFrameTitleForDocument( pClass->GetName( ) );

			return TRUE;
		}

		if (nID >= WM_DELETECLASSMENU && nID < (WM_DELETECLASSMENU + WM_MAXITEMS))
		{
			UINT idx = nID - WM_DELETECLASSMENU;
			g_ReClassApp.DeleteClass( g_ReClassApp.m_Classes[idx] );
			return TRUE;
		}
	}

	return CMDIFrameWndEx::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void CMainFrame::OnCheckAddress( )
{
	g_bAddress = !g_bAddress;
}

void CMainFrame::OnUpdateCheckAddress( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bAddress );
}

void CMainFrame::OnCheckOffset( )
{
	g_bOffset = !g_bOffset;
}

void CMainFrame::OnUpdateCheckOffset( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bOffset );
}

void CMainFrame::OnCheckText( )
{
	g_bText = !g_bText;
}

void CMainFrame::OnUpdateCheckText( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bText );
}

void CMainFrame::OnCheckRtti( )
{
	g_bRTTI = !g_bRTTI;
}

void CMainFrame::OnUpdateCheckRtti( CCmdUI *pCmdUI )
{
	if (!g_bPointers) {
		pCmdUI->Enable( FALSE );
	} else {
		pCmdUI->Enable( TRUE );
		pCmdUI->SetCheck( g_bRTTI );
	}
}

void CMainFrame::OnCheckRandomWindowName( )
{
	g_bRandomName = !g_bRandomName;

	if (g_bRandomName) {
		TCHAR tcsRandom[16] = { 0 };
		Utils::GenerateRandomString( tcsRandom, 16 );
		SetTitle( tcsRandom );
	} else {
		SetTitle( _T( "ReClass 2016" ) );
	}

	// Invalidate window to force a change to the window text
	Invalidate( );
}

void CMainFrame::OnUpdateCheckRandomWindowName( CCmdUI * pCmdUI )
{
	pCmdUI->SetCheck( g_bRandomName );
}

void CMainFrame::OnButtonTypedef( )
{
	CDialogTypes dlg( this );
	dlg.DoModal( );
}

void CMainFrame::OnButtonSelectProcess( )
{
	CDialogProcSelect proc( this );
	proc.DoModal( );
}

void CMainFrame::OnButtonEditClass( )
{
	if (g_bClassBrowser)
	{
		CDialogClasses dlg( this );
		dlg.DoModal( );
	}
	else
	{
		CMFCRibbonBaseElement* pButton = m_RibbonBar.FindByID( ID_BUTTON_EDITCLASS );
		CRect pos = pButton->GetRect( );
		ClientToScreen( &pos );

		CMenu menu;
		menu.CreatePopupMenu( );

		for (size_t i = 0; i < g_ReClassApp.m_Classes.size( ); i++)
		{
			CString MenuItem;
			MenuItem.Format( _T( "%i - %s" ), i, g_ReClassApp.m_Classes[i]->GetName( ).GetString( ) );
			menu.AppendMenu( MF_STRING | MF_ENABLED, WM_CLASSMENU + i, MenuItem );
		}

		menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this );
	}
}

void CMainFrame::OnUpdateButtonEditClass( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( (g_ReClassApp.m_Classes.size( ) > 0) );
}

void CMainFrame::OnButtonDeleteClass( )
{
	CMenu menu;
	CMFCRibbonBaseElement* pButton = m_RibbonBar.FindByID( ID_BUTTON_DELETECLASS );
	CRect pos = pButton->GetRect( );
	
	ClientToScreen( &pos );

	menu.CreatePopupMenu( );

	for (size_t i = 0; i < g_ReClassApp.m_Classes.size( ); i++)
	{
		menu.AppendMenu( MF_STRING | MF_ENABLED, WM_DELETECLASSMENU + i, g_ReClassApp.m_Classes[i]->GetName( ) );
	}

	menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this );
}

void CMainFrame::OnUpdateButtonDeleteClass( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( (g_ReClassApp.m_Classes.size( ) > 0) ? TRUE : FALSE );
}

void CMainFrame::OnTimer( UINT_PTR nIDEvent )
{
	if (nIDEvent == TIMER_MEMORYMAP_UPDATE)
		UpdateMemoryMap( );

	CMDIFrameWndEx::OnTimer( nIDEvent );
}

void CMainFrame::OnCheckTopmost( )
{
	g_bTop = !g_bTop;
	SetWindowPos( g_bTop ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
}

void CMainFrame::OnUpdateCheckTopmost( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bTop );
}

void CMainFrame::OnCheckClassBrowser( )
{
	g_bClassBrowser = !g_bClassBrowser;
}

void CMainFrame::OnUpdateCheckClassBrowser( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bClassBrowser );
}

void CMainFrame::OnCheckPrivatePadding( )
{
	g_bPrivatePadding = !g_bPrivatePadding;
}

void CMainFrame::OnUpdateCheckPrivatePadding( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bPrivatePadding );
}

void CMainFrame::OnCheckClipboardCopy( )
{
	g_bClipboardCopy = !g_bClipboardCopy;
}

void CMainFrame::OnUpdateCheckClipboardCopy( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bClipboardCopy );
}

// Multi monitor support. Thanks timboy67678
void CMainFrame::OnButtonLeft( )
{
	RECT rc;
	LONG nWidth, nHeight;
	HMONITOR hMon;
	MONITORINFO mi;

	ZeroMemory( &mi, sizeof( MONITORINFO ) );
	mi.cbSize = sizeof( MONITORINFO );

	::GetWindowRect( GetSafeHwnd( ), &rc );

	hMon = ::MonitorFromRect( &rc, MONITOR_DEFAULTTONEAREST );
	::GetMonitorInfo( hMon, &mi );

	nWidth = mi.rcWork.right - mi.rcWork.left;
	nHeight = mi.rcWork.bottom - mi.rcWork.top;

	SetWindowPos( g_bTop ? &wndTopMost : &wndNoTopMost, mi.rcMonitor.left, mi.rcMonitor.top, nWidth / 2, nHeight, SWP_NOZORDER );
}

void CMainFrame::OnButtonRight( )
{
	RECT rc;
	LONG nWidth, nHeight;
	HMONITOR hMon;
	MONITORINFO mi;

	ZeroMemory( &mi, sizeof( MONITORINFO ) );
	mi.cbSize = sizeof( MONITORINFO );

	::GetWindowRect( GetSafeHwnd( ), &rc );

	hMon = ::MonitorFromRect( &rc, MONITOR_DEFAULTTONEAREST );
	::GetMonitorInfo( hMon, &mi );

	nWidth = mi.rcWork.right - mi.rcWork.left;
	nHeight = mi.rcWork.bottom - mi.rcWork.top;

	SetWindowPos( g_bTop ? &wndTopMost : &wndNoTopMost, mi.rcMonitor.left + (nWidth / 2), 0, nWidth / 2, nHeight, SWP_NOZORDER );
}

void CMainFrame::OnCheckFloat( )
{
	g_bFloat = !g_bFloat;
}

void CMainFrame::OnUpdateCheckFloat( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bFloat );
}

void CMainFrame::OnCheckInteger( )
{
	g_bInt = !g_bInt;
}

void CMainFrame::OnUpdateCheckInteger( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bInt );
}

void CMainFrame::OnCheckString( )
{
	g_bString = !g_bString;
}

void CMainFrame::OnUpdateCheckString( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bString );
}

void CMainFrame::OnCheckPointer( )
{
	g_bPointers = !g_bPointers;
}

void CMainFrame::OnUpdateCheckPointer( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bPointers );
}

void CMainFrame::OnCheckUnsignedHex( )
{
	g_bUnsignedHex = !g_bUnsignedHex;
}

void CMainFrame::OnUpdateCheckUnsignedHex( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( g_bUnsignedHex );
}

