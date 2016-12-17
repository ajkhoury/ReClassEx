#include "stdafx.h"

#include "ReClass2016.h"

#include "afxwinappex.h"
#include "afxdialogex.h"

#include "CMainFrame.h"
#include "CChildFrame.h"

#include "DialogEdit.h"
#include "DialogClasses.h"
#include "DialogModules.h"
#include "DialogPlugins.h"
#include "DialogAbout.h"

// The one and only CReClass2016App object
CReClass2016App g_ReClassApp;

// CReClass2016App
BEGIN_MESSAGE_MAP( CReClass2016App, CWinAppEx )
	ON_COMMAND( ID_APP_ABOUT, &CReClass2016App::OnAppAbout )
	ON_COMMAND( ID_FILE_NEW, &CReClass2016App::OnFileNew )
	ON_COMMAND( ID_FILE_SAVE, &CReClass2016App::OnFileSave )
	ON_COMMAND( ID_FILE_SAVE_AS, &CReClass2016App::OnFileSaveAs )
	ON_COMMAND( ID_FILE_OPEN, &CReClass2016App::OnFileOpen )
	ON_COMMAND( ID_FILE_OPEN_PDB, &CReClass2016App::OnOpenPDB )
	ON_COMMAND( ID_RECLASS_PLUGINS, &CReClass2016App::OnButtonPlugins )
	ON_COMMAND( ID_BUTTON_NEWCLASS, &CReClass2016App::OnButtonNewClass )
	ON_COMMAND( ID_BUTTON_NOTES, &CReClass2016App::OnButtonNotes )
	ON_COMMAND( ID_BUTTON_SEARCH, &CReClass2016App::OnButtonSearch )
	ON_COMMAND( ID_BUTTON_CONSOLE, &CReClass2016App::OnButtonConsole )
	ON_COMMAND( ID_BUTTON_MODULES, &CReClass2016App::OnButtonModules )
	ON_COMMAND( ID_BUTTON_PARSER, &CReClass2016App::OnButtonParser )
	ON_COMMAND( ID_BUTTON_HEADER, &CReClass2016App::OnButtonHeader )
	ON_COMMAND( ID_BUTTON_FOOTER, &CReClass2016App::OnButtonFooter )
	ON_COMMAND( ID_BUTTON_RESET, &CReClass2016App::OnButtonReset )
	ON_COMMAND( ID_BUTTON_PAUSE, &CReClass2016App::OnButtonPause )
	ON_COMMAND( ID_BUTTON_RESUME, &CReClass2016App::OnButtonResume )
	ON_COMMAND( ID_BUTTON_KILL, &CReClass2016App::OnButtonKill )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_PAUSE, &CReClass2016App::OnUpdateButtonPause )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_RESUME, &CReClass2016App::OnUpdateButtonResume )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_KILL, &CReClass2016App::OnUpdateButtonKill )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_SEARCH, &CReClass2016App::OnUpdateButtonSearch )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_MODULES, &CReClass2016App::OnUpdateButtonModules )
	ON_UPDATE_COMMAND_UI( ID_RECLASS_PLUGINS, &CReClass2016App::OnUpdateButtonPlugins )
	ON_COMMAND( ID_BUTTON_GENERATE, &CReClass2016App::OnButtonGenerate )
	ON_COMMAND( ID_BUTTON_CLEAN, &CReClass2016App::OnButtonClean )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_CLEAN, &CReClass2016App::OnUpdateButtonClean )
	ON_UPDATE_COMMAND_UI( ID_FILE_OPEN_PDB, &CReClass2016App::OnUpdateOpenPDB )
END_MESSAGE_MAP( )

CReClass2016App::CReClass2016App( )
{
	TCHAR AppId[256] = { 0 };

	LoadString( NULL, AFX_IDS_APP_ID, AppId, 256 );
	SetAppID( AppId );

	m_bHiColorIcons = TRUE;
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	g_FontWidth = FONT_DEFAULT_WIDTH;
	g_FontHeight = FONT_DEFAULT_HEIGHT;
}

void CReClass2016App::ResizeMemoryFont( int font_width, int font_height )
{
	g_ViewFont.DeleteObject( );

	HMODULE hSHCoreBase = (HMODULE)Utils::GetLocalModuleBaseW( L"shcore.dll" );
	if (hSHCoreBase)
	{
		auto pfnGetProcessDpiAwareness = reinterpret_cast<decltype(&GetProcessDpiAwareness)>(Utils::GetLocalProcAddressW( hSHCoreBase, L"GetProcessDpiAwareness" ));
		auto pfnGetDpiForMonitor = reinterpret_cast<decltype(&GetDpiForMonitor)>(Utils::GetLocalProcAddressW( hSHCoreBase, L"GetDpiForMonitor" ));

		if (pfnGetProcessDpiAwareness != nullptr && pfnGetDpiForMonitor != nullptr)
		{
			PROCESS_DPI_AWARENESS dpi;
			pfnGetProcessDpiAwareness( NULL, &dpi );
			if (dpi == PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE || dpi == PROCESS_DPI_AWARENESS::PROCESS_SYSTEM_DPI_AWARE)
			{
				UINT dpiX, dpiY;
				HMONITOR monitor = ::MonitorFromWindow( m_pMainWnd->GetSafeHwnd( ), MONITOR_DEFAULTTONEAREST );
				pfnGetDpiForMonitor( monitor, MONITOR_DPI_TYPE::MDT_EFFECTIVE_DPI, &dpiX, &dpiY );
				g_FontWidth = MulDiv( font_width, MulDiv( dpiX, 100, 96 ), 100 );
				g_FontHeight = MulDiv( font_height, MulDiv( dpiY, 100, 96 ), 100 );
			}
		}

		FreeLibrary( hSHCoreBase );
	}

	g_ViewFont.CreateFont( g_FontHeight, g_FontWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH, g_ViewFontName.GetBuffer( ) );
}

BOOL CReClass2016App::InitInstance( )
{
	#ifdef _DEBUG
	Utils::CreateDbgConsole( _T( "dbg" ) );
	#endif

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinAppEx::InitInstance( );

	if (!ntdll::Init( ))
	{
		AfxMessageBox( IDP_NTDLL_INIT_FAILED );
		return FALSE;
	}

	if (!AfxOleInit( ))
	{
		AfxMessageBox( IDP_OLE_INIT_FAILED );
		return FALSE;
	}

	AfxEnableControlContainer( );
	EnableTaskbarInteraction( FALSE );
	InitContextMenuManager( );
	InitKeyboardManager( );
	InitTooltipManager( );

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	GetTooltipManager( )->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams );

	// Get registry entries
	SetRegistryKey( _T( "RC16" ) );

	//Typedefs
	g_tdHex				= GetProfileString( _T( "Typedefs" ), _T( "Hex" ), _T( "char" ) );
	g_tdInt64			= GetProfileString( _T( "Typedefs" ), _T( "Int64" ), _T( "__int64" ) );
	g_tdInt32			= GetProfileString( _T( "Typedefs" ), _T( "Int32" ), _T( "__int32" ) );
	g_tdInt16			= GetProfileString( _T( "Typedefs" ), _T( "Int16" ), _T( "__int16" ) );
	g_tdInt8			= GetProfileString( _T( "Typedefs" ), _T( "Int8" ), _T( "__int8" ) );
	g_tdQWORD			= GetProfileString( _T( "Typedefs" ), _T( "QWORD" ), _T( "DWORD64" ) );
	g_tdDWORD			= GetProfileString( _T( "Typedefs" ), _T( "DWORD" ), _T( "DWORD" ) );
	g_tdWORD			= GetProfileString( _T( "Typedefs" ), _T( "WORD" ), _T( "WORD" ) );
	g_tdBYTE			= GetProfileString( _T( "Typedefs" ), _T( "BYTE" ), _T( "unsigned char" ) );
	g_tdFloat			= GetProfileString( _T( "Typedefs" ), _T( "Float" ), _T( "float" ) );
	g_tdDouble			= GetProfileString( _T( "Typedefs" ), _T( "Double" ), _T( "double" ) );
	g_tdVec2			= GetProfileString( _T( "Typedefs" ), _T( "Vec2" ), _T( "Vector2" ) );
	g_tdVec3			= GetProfileString( _T( "Typedefs" ), _T( "Vec3" ), _T( "Vector3" ) );
	g_tdQuat			= GetProfileString( _T( "Typedefs" ), _T( "Quat" ), _T( "Vector4" ) );
	g_tdMatrix			= GetProfileString( _T( "Typedefs" ), _T( "Matrix" ), _T( "matrix3x4_t" ) );
	g_tdPChar			= GetProfileString( _T( "Typedefs" ), _T( "PChar" ), _T( "char*" ) );
	g_tdPWChar			= GetProfileString( _T( "Typedefs" ), _T( "PWChar" ), _T( "wchar_t*" ) );

	g_crBackground		= GetProfileInt( _T( "Colors" ), _T( "Background" ),	g_crBackground );
	g_crSelect			= GetProfileInt( _T( "Colors" ), _T( "Select" ),		g_crSelect );
	g_crHidden			= GetProfileInt( _T( "Colors" ), _T( "Hidden" ),		g_crHidden );
	g_crOffset			= GetProfileInt( _T( "Colors" ), _T( "Offset" ),		g_crOffset );
	g_crAddress			= GetProfileInt( _T( "Colors" ), _T( "Address" ),		g_crAddress );
	g_crType			= GetProfileInt( _T( "Colors" ), _T( "Type" ),			g_crType );
	g_crName			= GetProfileInt( _T( "Colors" ), _T( "Name" ),			g_crName );
	g_crIndex			= GetProfileInt( _T( "Colors" ), _T( "Index" ),			g_crIndex );
	g_crValue			= GetProfileInt( _T( "Colors" ), _T( "Value" ),			g_crValue );
	g_crComment			= GetProfileInt( _T( "Colors" ), _T( "Comment" ),		g_crComment );
	g_crVTable			= GetProfileInt( _T( "Colors" ), _T( "VTable" ),		g_crVTable );
	g_crFunction		= GetProfileInt( _T( "Colors" ), _T( "Function" ),		g_crFunction );
	g_crChar			= GetProfileInt( _T( "Colors" ), _T( "Char" ),			g_crChar );
	g_crCustom			= GetProfileInt( _T( "Colors" ), _T( "Custom" ),		g_crCustom );
	g_crHex				= GetProfileInt( _T( "Colors" ), _T( "Hex" ),			g_crHex );

	g_bOffset			= GetProfileInt( _T( "Display" ), _T( "Offset" ),		g_bOffset ) > 0 ? true : false;
	g_bAddress			= GetProfileInt( _T( "Display" ), _T( "Address" ),		g_bAddress ) > 0 ? true : false;
	g_bText				= GetProfileInt( _T( "Display" ), _T( "Text" ),			g_bText ) > 0 ? true : false;
	g_bFloat			= GetProfileInt( _T( "Display" ), _T( "Float" ),		g_bFloat ) > 0 ? true : false;
	g_bInt				= GetProfileInt( _T( "Display" ), _T( "Int" ),			g_bInt ) > 0 ? true : false;
	g_bString			= GetProfileInt( _T( "Display" ), _T( "String" ),		g_bString ) > 0 ? true : false;
	g_bPointers			= GetProfileInt( _T( "Display" ), _T( "Pointers" ),		g_bPointers ) > 0 ? true : false;
	g_bClassBrowser		= GetProfileInt( _T( "Display" ), _T( "ClassBrowser" ), g_bClassBrowser ) > 0 ? true : false;
	g_bFilterProcesses	= GetProfileInt( _T( "Display" ), _T( "FilterProcesses" ), g_bFilterProcesses ) > 0 ? true : false;

	g_bRTTI				= GetProfileInt( _T( "Misc" ), _T( "RTTI" ),			g_bRTTI ) > 0 ? true : false;
	g_bRandomName		= GetProfileInt( _T( "Misc" ), _T( "RandomName" ),		g_bRandomName ) > 0 ? true : false;
	g_bLoadModuleSymbol = GetProfileInt( _T( "Misc" ), _T( "LoadModuleSymbols" ), g_bLoadModuleSymbol ) > 0 ? true : false;

	g_bPrivatePadding	= GetProfileInt( _T( "Class Generation" ), _T( "PrivatePadding" ), g_bPrivatePadding ) > 0 ? true : false;
	g_bClipboardCopy	= GetProfileInt( _T( "Class Generation" ), _T( "ClipboardCopy" ), g_bClipboardCopy ) > 0 ? true : false;

	g_bTop = false; //GetProfileInt("Display", "g_bTop", g_bTop) > 0 ? true : false;

	g_ViewFontName = _T( "Terminal" );

	HINSTANCE hInst = AfxGetInstanceHandle( );
	m_hMDIMenu = ::LoadMenu( hInst, MAKEINTRESOURCE( IDR_ReClass2016TYPE ) );
	m_hMDIAccel = ::LoadAccelerators( hInst, MAKEINTRESOURCE( IDR_ReClass2016TYPE ) );

	#define PushIcon(id) g_Icons.emplace_back(::LoadIcon(hInst, MAKEINTRESOURCE(id)));
	PushIcon( IDI_ICON_OPEN );
	PushIcon( IDI_ICON_CLOSED );
	PushIcon( IDI_ICON_CLASS );
	PushIcon( IDI_ICON_METHOD );
	PushIcon( IDI_ICON_VTABLE );
	PushIcon( IDI_ICON_DELETE );
	PushIcon( IDI_ICON_ADD );
	PushIcon( IDI_ICON_RANDOM );
	PushIcon( IDI_ICON_DROPARROW );
	PushIcon( IDI_ICON_POINTER );
	PushIcon( IDI_ICON_ARRAY );
	PushIcon( IDI_ICON_CUSTOM );
	PushIcon( IDI_ICON_ENUM );
	PushIcon( IDI_ICON_FLOAT );
	PushIcon( IDI_ICON_LEFT );
	PushIcon( IDI_ICON_RIGHT );
	PushIcon( IDI_ICON_MATRIX );
	PushIcon( IDI_ICON_INTEGER );
	PushIcon( IDI_ICON_TEXT );
	PushIcon( IDI_ICON_UNSIGNED );
	PushIcon( IDI_ICON_VECTOR );
	PushIcon( IDI_ICON_CHANGE );
	PushIcon( IDI_ICON_CAMERA );
	#undef PushIcon

	CMainFrame* pFrame = new CMainFrame( );
	m_pMainWnd = pFrame;
	if (!pFrame)
		return FALSE;
	if (!pFrame->LoadFrame( IDR_MAINFRAME ))
		return FALSE;

	pFrame->m_hMenuDefault = m_hMDIMenu;
	pFrame->m_hAccelTable = m_hMDIAccel;

	pFrame->ShowWindow( m_nCmdShow );
	pFrame->UpdateWindow( );

	// Fix for 4k monitors
	ResizeMemoryFont( g_FontWidth, g_FontHeight );

	g_hProcess = NULL;
	g_ProcessID = NULL;
	g_AttachedProcessAddress = NULL;

	// Initialize the editor
	if (!Scintilla_RegisterClasses( m_hInstance ))
	{
		AfxMessageBox( _T( "Scintilla failed to initiailze" ) );
		return FALSE;
	}

	m_pConsole = new CDialogConsole( _T( "Console" ) );
	if (m_pConsole->Create( CDialogConsole::IDD, CWnd::GetDesktopWindow( ) ))
		m_pConsole->ShowWindow( SW_HIDE );

	m_pSymbolLoader = new (std::nothrow) Symbols;
	if (m_pSymbolLoader != nullptr)
	{
		PrintOut( _T( "Symbol resolution enabled" ) );
		g_bSymbolResolution = true;
	}
	else
	{
		PrintOut( _T( "Failed to init symbol loader, disabling globally" ) );
		g_bSymbolResolution = false;
		g_bSymbolResolution = false;
	}

	LoadPlugins( );

	// Get debug privilege
	//Utils::SetDebugPrivilege(TRUE);

	return TRUE;
}

int CReClass2016App::ExitInstance( )
{
	//
	// Free resources
	if (m_hMDIMenu != NULL)
		FreeResource( m_hMDIMenu );

	if (m_hMDIAccel != NULL)
		FreeResource( m_hMDIAccel );

	if (m_pConsole)
	{
		delete m_pConsole;
		m_pConsole = NULL;
	}

	if (m_pSymbolLoader) 
	{ 
		delete m_pSymbolLoader;
		m_pSymbolLoader = NULL;
	}

	AfxOleTerm( FALSE );

	//
	// Release Scintilla
	Scintilla_ReleaseResources( );

	//
	// Write settings to profile
	WriteProfileString( _T( "Typedefs" ), _T( "Hex" ),			g_tdHex );
	WriteProfileString( _T( "Typedefs" ), _T( "Int64" ),		g_tdInt64 );
	WriteProfileString( _T( "Typedefs" ), _T( "Int32" ),		g_tdInt32 );
	WriteProfileString( _T( "Typedefs" ), _T( "Int16" ),		g_tdInt16 );
	WriteProfileString( _T( "Typedefs" ), _T( "Int8" ),			g_tdInt8 );
	WriteProfileString( _T( "Typedefs" ), _T( "QWORD" ),		g_tdQWORD );
	WriteProfileString( _T( "Typedefs" ), _T( "DWORD" ),		g_tdDWORD );
	WriteProfileString( _T( "Typedefs" ), _T( "WORD" ),			g_tdWORD );
	WriteProfileString( _T( "Typedefs" ), _T( "BYTE" ),			g_tdBYTE );
	WriteProfileString( _T( "Typedefs" ), _T( "Float" ),		g_tdFloat );
	WriteProfileString( _T( "Typedefs" ), _T( "Double" ),		g_tdDouble );
	WriteProfileString( _T( "Typedefs" ), _T( "Vec2" ),			g_tdVec2 );
	WriteProfileString( _T( "Typedefs" ), _T( "Vec3" ),			g_tdVec3 );
	WriteProfileString( _T( "Typedefs" ), _T( "Quat" ),			g_tdQuat );
	WriteProfileString( _T( "Typedefs" ), _T( "Matrix" ),		g_tdMatrix );
	WriteProfileString( _T( "Typedefs" ), _T( "PChar" ),		g_tdPChar );
	WriteProfileString( _T( "Typedefs" ), _T( "PWChar" ),		g_tdPWChar );

	WriteProfileInt( _T( "Colors" ), _T( "Background" ),		g_crBackground );
	WriteProfileInt( _T( "Colors" ), _T( "Select" ),			g_crSelect );
	WriteProfileInt( _T( "Colors" ), _T( "Hidden" ),			g_crHidden );
	WriteProfileInt( _T( "Colors" ), _T( "Offset" ),			g_crOffset );
	WriteProfileInt( _T( "Colors" ), _T( "Address" ),			g_crAddress );
	WriteProfileInt( _T( "Colors" ), _T( "Type" ),				g_crType );
	WriteProfileInt( _T( "Colors" ), _T( "Name" ),				g_crName );
	WriteProfileInt( _T( "Colors" ), _T( "Index" ),				g_crIndex );
	WriteProfileInt( _T( "Colors" ), _T( "Value" ),				g_crValue );
	WriteProfileInt( _T( "Colors" ), _T( "Comment" ),			g_crComment );
	WriteProfileInt( _T( "Colors" ), _T( "VTable" ),			g_crVTable );
	WriteProfileInt( _T( "Colors" ), _T( "Function" ),			g_crFunction );
	WriteProfileInt( _T( "Colors" ), _T( "Char" ),				g_crChar );
	WriteProfileInt( _T( "Colors" ), _T( "Custom" ),			g_crCustom );
	WriteProfileInt( _T( "Colors" ), _T( "Hex" ),				g_crHex );

	WriteProfileInt( _T( "Display" ), _T( "Offset" ),			g_bOffset );
	WriteProfileInt( _T( "Display" ), _T( "Address" ),			g_bAddress );
	WriteProfileInt( _T( "Display" ), _T( "Text" ),				g_bText );
	WriteProfileInt( _T( "Display" ), _T( "Float" ),			g_bFloat );
	WriteProfileInt( _T( "Display" ), _T( "Int" ),				g_bInt );
	WriteProfileInt( _T( "Display" ), _T( "String" ),			g_bString );
	WriteProfileInt( _T( "Display" ), _T( "Pointers" ),			g_bPointers );
	WriteProfileInt( _T( "Display" ), _T( "Top" ),				g_bTop );
	WriteProfileInt( _T( "Display" ), _T( "ClassBrowser" ),		g_bClassBrowser );
	WriteProfileInt( _T( "Display" ), _T( "FilterProcesses" ),	g_bFilterProcesses );

	WriteProfileInt( _T( "Misc" ), _T( "RTTI" ),				g_bRTTI );
	WriteProfileInt( _T( "Misc" ), _T( "RandomName" ),			g_bRandomName );
	WriteProfileInt( _T( "Misc" ), _T( "LoadModuleSymbols" ),	g_bLoadModuleSymbol );

	WriteProfileInt( _T( "Class Generation" ), _T( "PrivatePadding" ), g_bPrivatePadding );
	WriteProfileInt( _T( "Class Generation" ), _T( "ClipboardCopy" ), g_bClipboardCopy );

	//
	// Unload any loaded plugins
	UnloadPlugins( );

	return CWinAppEx::ExitInstance( );
}

void CReClass2016App::OnButtonReset( )
{
	CloseHandle( g_hProcess );

	g_hProcess = NULL;
	g_ProcessID = 0;
	g_AttachedProcessAddress = NULL;

	CMDIFrameWnd* pFrame = STATIC_DOWNCAST( CMDIFrameWnd, m_pMainWnd );
	CMDIChildWnd* pChildWnd = pFrame->MDIGetActive( );

	while (pChildWnd)
	{
		pChildWnd->SendMessage( WM_CLOSE, 0, 0 );
		pChildWnd = pFrame->MDIGetActive( );
	}

	m_Classes.clear( );

	m_strHeader = _T( "" );
	m_strFooter = _T( "" );
	m_strNotes = _T( "" );

	m_strCurrentFilePath = "";
}

void CReClass2016App::OnButtonPause( )
{
	PauseResumeThreadList( FALSE );
}

void CReClass2016App::OnUpdateButtonPause( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( g_hProcess != NULL );
}

void CReClass2016App::OnButtonResume( )
{
	PauseResumeThreadList( TRUE );
}

void CReClass2016App::OnUpdateButtonResume( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( g_hProcess != NULL );
}

void CReClass2016App::OnButtonKill( )
{
	TerminateProcess( g_hProcess, 0 );
	g_hProcess = NULL;
}

void CReClass2016App::OnUpdateButtonKill( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( g_hProcess != NULL );
}

void CReClass2016App::CalcOffsets( CNodeClass* pClass )
{
	size_t offset = 0;
	for (UINT i = 0; i < pClass->NodeCount( ); i++)
	{
		pClass->GetNode( i )->SetOffset( offset );
		offset += pClass->GetNode( i )->GetMemorySize( );
	}
}

void CReClass2016App::CalcAllOffsets( )
{
	for (UINT i = 0; i < m_Classes.size( ); i++)
		CalcOffsets( m_Classes[i] );
}

void CReClass2016App::OnFileNew( )
{
	CMainFrame* pFrame = STATIC_DOWNCAST( CMainFrame, m_pMainWnd );
	CChildFrame* pChild = STATIC_DOWNCAST( CChildFrame, pFrame->CreateNewChild( RUNTIME_CLASS( CChildFrame ), IDR_ReClass2016TYPE, m_hMDIMenu, m_hMDIAccel ) );
	CNodeClass* pClass = new CNodeClass;

	pClass->pChildWindow = pChild;
	pChild->SetClass( pClass );
	g_ReClassApp.m_Classes.push_back( pClass );
	
	//CNodeCustom* pCust = new CNodeCustom;
	//pCust->memsize = 18;
	//pCust->pParent = pClass;
	//pClass->Nodes.push_back(pCust);

	for (int i = 0; i < 64 / sizeof( size_t ); i++)
	{
		CNodeHex* pNode = new CNodeHex;
		pNode->SetParent( pClass );
		pClass->AddNode( pNode );
	}

	CalcOffsets( pClass );
}

// App command to run the dialog
void CReClass2016App::OnAppAbout( )
{
	CDialogAbout aboutDlg;
	aboutDlg.DoModal( );
}

void CReClass2016App::PreLoadState( )
{
	CString strName;
	ASSERT( strName.LoadString( IDS_EDIT_MENU ) );
	GetContextMenuManager( )->AddMenu( strName, IDR_POPUP_EDIT );
}

void CReClass2016App::LoadCustomState( )
{
}

void CReClass2016App::SaveCustomState( )
{
}

// TODO: Actually finish this
void CReClass2016App::OnFileImport( )
{
	return;
}

void CReClass2016App::ClearSelection( )
{
	for (UINT i = 0; i < m_Classes.size( ); i++)
	{
		m_Classes[i]->Unselect( );
		for (UINT n = 0; n < m_Classes[i]->NodeCount( ); n++)
		{
			CNodeBase* pNode = m_Classes[i]->GetNode( n );
			pNode->Unselect( );

			NodeType nt = pNode->GetType( );
			if (nt == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->NodeCount( ); f++)
				{
					pVTable->GetNode( f )->Unselect( );
				}
			}
			if (nt == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				pArray->Unselect( );
			}
			if (nt == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				pPtr->Unselect( );
			}
		}
	}
}

void CReClass2016App::ClearHidden( )
{
	for (UINT i = 0; i < m_Classes.size( ); i++)
	{
		m_Classes[i]->Show( );
		for (UINT n = 0; n < m_Classes[i]->NodeCount( ); n++)
		{
			CNodeBase* pNode = m_Classes[i]->GetNode( n );
			pNode->Show( );

			NodeType nt = pNode->GetType( );
			if (nt == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->NodeCount( ); f++)
					pVTable->GetNode( f )->Show( );
			}
			if (nt == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				pArray->Show( );
			}
			if (nt == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				pPtr->Show( );
			}
		}
	}
}

bool CReClass2016App::IsNodeValid( CNodeBase* pCheckNode )
{
	for (UINT i = 0; i < m_Classes.size( ); i++)
	{
		for (UINT n = 0; n < m_Classes[i]->NodeCount( ); n++)
		{
			CNodeBase* pNode = m_Classes[i]->GetNode( n );
			if (pNode == pCheckNode)
				return true;

			NodeType nt = pNode->GetType( );
			if (nt == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->NodeCount( ); f++)
				{
					if (pVTable->GetNode( f ) == pCheckNode)
						return true;
				}
			}
			if (nt == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				if (pArray->pNode == pCheckNode)
					return true;
			}
			if (nt == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				if (pPtr->pNode == pCheckNode)
					return true;
			}
		}
	}
	return false;
}


//////////////// OnButtonNewClass /////////////////
void CReClass2016App::OnButtonNewClass( )
{
	CMainFrame* pFrame = STATIC_DOWNCAST( CMainFrame, m_pMainWnd );
	CChildFrame* pChild = STATIC_DOWNCAST( CChildFrame, pFrame->CreateNewChild( RUNTIME_CLASS( CChildFrame ), IDR_ReClass2016TYPE, m_hMDIMenu, m_hMDIAccel ) );
	CNodeClass* pClass = new CNodeClass;

	pClass->SetChildFrame( pChild );
	pClass->idx = (int)g_ReClassApp.m_Classes.size( );
	pChild->SetClass( pClass );

	g_ReClassApp.m_Classes.push_back( pClass );

	for (int i = 0; i < 64 / sizeof( size_t ); i++)
	{
		CNodeHex* pNode = new CNodeHex;
		pNode->SetParent( pClass );
		pClass->AddNode( pNode );
	}

	CalcOffsets( pClass );
}

void CReClass2016App::OnButtonSearch( )
{
	GetMainWnd( )->MessageBox( _T( "Coming Soon!" ), _T( "WubbaLubbaDubDub" ) );
}

void CReClass2016App::OnUpdateButtonSearch( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( g_hProcess != NULL );
}

void CReClass2016App::OnButtonConsole( )
{
	m_pConsole->ShowWindow( SW_SHOW );
	m_pConsole->SetForegroundWindow( );
}

void CReClass2016App::OnButtonModules( )
{
	CDialogModules dlg;
	dlg.DoModal( );
}

void CReClass2016App::OnUpdateButtonModules( CCmdUI * pCmdU )
{
	pCmdU->Enable( g_hProcess != NULL );
}

void CReClass2016App::OnButtonNotes( )
{
	CDialogEdit dlg;
	dlg.Title = _T( "Notes" );
	dlg.Text = m_strNotes;
	dlg.DoModal( );
	m_strNotes = dlg.Text;
}

void CReClass2016App::OnButtonParser( )
{
	CDialogClasses dlg;
	dlg.DoModal( );
}

void CReClass2016App::OnButtonHeader( )
{
	CDialogEdit dlg;
	dlg.Title = _T( "Header" );
	dlg.Text = m_strHeader;
	dlg.DoModal( );
	m_strHeader = dlg.Text;
}

void CReClass2016App::OnButtonFooter( )
{
	CDialogEdit dlg;
	dlg.Title = _T( "Footer" );
	dlg.Text = m_strFooter;
	dlg.DoModal( );
	m_strFooter = dlg.Text;
}

CMainFrame* CReClass2016App::GetMainFrame( )
{
	return STATIC_DOWNCAST( CMainFrame, g_ReClassApp.m_pMainWnd );
}

CMFCRibbonBar* CReClass2016App::GetRibbonBar( )
{
	return &GetMainFrame( )->m_RibbonBar;
}

CNodeBase* CReClass2016App::CreateNewNode( NodeType Type )
{
	if (Type == nt_class) return new CNodeClass;

	if (Type == nt_hex64) return new CNodeHex64;
	if (Type == nt_hex32) return new CNodeHex32;
	if (Type == nt_hex16) return new CNodeHex16;
	if (Type == nt_hex8) return new CNodeHex8;

	if (Type == nt_bits) return new CNodeBits;

	if (Type == nt_int64) return new CNodeInt64;
	if (Type == nt_int32) return new CNodeInt32;
	if (Type == nt_int16) return new CNodeInt16;
	if (Type == nt_int8) return new CNodeInt8;

	if (Type == nt_uint64) return new CNodeQWORD;
	if (Type == nt_uint32) return new CNodeDWORD;
	if (Type == nt_uint16) return new CNodeWORD;
	if (Type == nt_uint8) return new CNodeByte;

	if (Type == nt_vec2) return new CNodeVec2;
	if (Type == nt_vec3) return new CNodeVec3;
	if (Type == nt_quat) return new CNodeQuat;
	if (Type == nt_matrix) return new CNodeMatrix;

	if (Type == nt_float) return new CNodeFloat;
	if (Type == nt_double) return new CNodeDouble;

	if (Type == nt_custom) return new CNodeCustom;
	if (Type == nt_text) return new CNodeText;
	if (Type == nt_pchar) return new CNodeCharPtr;
	if (Type == nt_pwchar) return new CNodeWCharPtr;
	if (Type == nt_unicode) return new CNodeUnicode;

	if (Type == nt_vtable) return new CNodeVTable;
	if (Type == nt_functionptr) return new CNodeFunctionPtr;
	if (Type == nt_function) return new CNodeFunction;

	if (Type == nt_pointer) return new CNodePtr;
	if (Type == nt_array) return new CNodeArray;
	if (Type == nt_instance) return new CNodeClassInstance;

	return NULL;
}

void CReClass2016App::SaveXML( TCHAR* FileName )
{
	PrintOut( _T( "SaveXML(\"%s\") called" ), FileName );

	TiXMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration(/*"xml version = \"1.0\" encoding=\"UTF-8\""*/ );
	doc.LinkEndChild( decl );

	XMLElement* root = doc.NewElement( "ReClass" );
	doc.LinkEndChild( root );

	XMLComment* comment = doc.NewComment( "Reclass 2016" );
	root->LinkEndChild( comment );
	//---------------------------------------------
	XMLElement* settings = doc.NewElement( "TypeDef" );
	#ifdef UNICODE
	settings->SetAttribute( "tdHex",	CW2A( g_tdHex ) );
	settings->SetAttribute( "tdInt64",	CW2A( g_tdInt64 ) );
	settings->SetAttribute( "tdInt32",	CW2A( g_tdInt32 ) );
	settings->SetAttribute( "tdInt16",	CW2A( g_tdInt16 ) );
	settings->SetAttribute( "tdInt8",	CW2A( g_tdInt8 ) );
	settings->SetAttribute( "tdQWORD",	CW2A( g_tdQWORD ) );
	settings->SetAttribute( "tdDWORD",	CW2A( g_tdDWORD ) );
	settings->SetAttribute( "tdWORD",	CW2A( g_tdWORD ) );
	settings->SetAttribute( "tdBYTE",	CW2A( g_tdBYTE ) );
	settings->SetAttribute( "tdFloat",	CW2A( g_tdFloat ) );
	settings->SetAttribute( "tdDouble", CW2A( g_tdDouble ) );
	settings->SetAttribute( "tdVec2",	CW2A( g_tdVec2 ) );
	settings->SetAttribute( "tdVec3",	CW2A( g_tdVec3 ) );
	settings->SetAttribute( "tdQuat",	CW2A( g_tdQuat ) );
	settings->SetAttribute( "tdMatrix", CW2A( g_tdMatrix ) );
	settings->SetAttribute( "tdPChar",	CW2A( g_tdPChar ) );
	settings->SetAttribute( "tdPWChar", CW2A( g_tdPWChar ) );
	#else
	settings->SetAttribute( "tdHex",	g_tdHex );
	settings->SetAttribute( "tdInt64",	g_tdInt64 );
	settings->SetAttribute( "tdInt32",	g_tdInt32 );
	settings->SetAttribute( "tdInt16",	g_tdInt16 );
	settings->SetAttribute( "tdInt8",	g_tdInt8 );
	settings->SetAttribute( "tdQWORD",	g_tdQWORD );
	settings->SetAttribute( "tdDWORD",	g_tdDWORD );
	settings->SetAttribute( "tdWORD",	g_tdWORD );
	settings->SetAttribute( "tdBYTE",	g_tdBYTE );
	settings->SetAttribute( "tdFloat",	g_tdFloat );
	settings->SetAttribute( "tdDouble", g_tdDouble );
	settings->SetAttribute( "tdVec2",	g_tdVec2 );
	settings->SetAttribute( "tdVec3",	g_tdVec3 );
	settings->SetAttribute( "tdQuat",	g_tdQuat );
	settings->SetAttribute( "tdMatrix", g_tdMatrix );
	settings->SetAttribute( "tdPChar",	g_tdPChar );
	settings->SetAttribute( "tdPWChar", g_tdPWChar );
	#endif
	root->LinkEndChild( settings );

	settings = doc.NewElement( "Header" );
	#ifdef UNICODE
	settings->SetAttribute( "Text", CW2A( m_strHeader ) );
	root->LinkEndChild( settings );

	settings = doc.NewElement( "Footer" );
	settings->SetAttribute( "Text", CW2A( m_strFooter ) );
	root->LinkEndChild( settings );

	settings = doc.NewElement( "Notes" );
	settings->SetAttribute( "Text", CW2A( m_strNotes ) );
	root->LinkEndChild( settings );
	#else
	settings->SetAttribute( "Text", Header );
	root->LinkEndChild( settings );

	settings = doc.NewElement( "Footer" );
	settings->SetAttribute( "Text", Footer );
	root->LinkEndChild( settings );

	settings = doc.NewElement( "Notes" );
	settings->SetAttribute( "Text", Notes );
	root->LinkEndChild( settings );
	#endif

	for (UINT i = 0; i < m_Classes.size( ); i++)
	{
		CNodeClass* pClass = m_Classes[i];

		#ifdef UNICODE
		CStringA strClassName = CW2A( pClass->GetName( ) );
		CStringA strClassComment = CW2A( pClass->GetComment( ) );
		CStringA strClassOffset = CW2A( pClass->GetOffsetString( ) );
		CStringA strClassCode = CW2A( pClass->Code );
		#else
		CStringA strClassName = pClass->Name;
		CStringA strClassComment = pClass->Comment;
		CStringA strClassOffset = pClass->strOffset;
		CStringA strClassCode = pClass->Code;
		#endif

		XMLElement* classNode = doc.NewElement( "Class" );
		classNode->SetAttribute( "Name", strClassName );
		classNode->SetAttribute( "Type", pClass->GetType( ) );
		classNode->SetAttribute( "Comment", strClassComment );
		classNode->SetAttribute( "Offset", (int)pClass->GetOffset( ) );
		classNode->SetAttribute( "strOffset", strClassOffset );
		classNode->SetAttribute( "Code", strClassCode );
		root->LinkEndChild( classNode );

		for (UINT n = 0; n < pClass->NodeCount( ); n++)
		{
			CNodeBase* pNode = pClass->GetNode( n );
			if (!pNode)
				continue;

			#ifdef UNICODE
			CStringA strNodeName = CW2A( pNode->GetName( ) );
			CStringA strNodeComment = CW2A( pNode->GetComment( ) );
			#else
			CStringA strNodeName = pNode->Name;
			CStringA strNodeComment = pNode->Comment;
			#endif

			XMLElement* node = doc.NewElement( "Node" );
			node->SetAttribute( "Name", strNodeName );
			node->SetAttribute( "Type", pNode->GetType( ) );
			node->SetAttribute( "Size", pNode->GetMemorySize( ) );
			node->SetAttribute( "bHidden", pNode->IsHidden( ) );
			node->SetAttribute( "Comment", strNodeComment );

			classNode->LinkEndChild( node );

			if (pNode->GetType( ) == nt_array)
			{
				CNodeArray* pptr = (CNodeArray*)pNode;
				node->SetAttribute( "Total", (UINT)pptr->Total );

				#ifdef UNICODE
				CStringA strArrayNodeName = CW2A( pptr->pNode->GetName( ) );
				CStringA strArrayNodeComment = CW2A( pptr->pNode->GetComment( ) );
				#else
				CStringA strArrayNodeName = pptr->pNode->Name;
				CStringA strArrayNodeComment = pptr->pNode->Comment;
				#endif

				XMLElement *item = doc.NewElement( "Array" );
				item->SetAttribute( "Name", strArrayNodeName );
				item->SetAttribute( "Type", pptr->pNode->GetType( ) );
				item->SetAttribute( "Size", pptr->pNode->GetMemorySize( ) );
				item->SetAttribute( "Comment", strArrayNodeComment );
				node->LinkEndChild( item );
			}
			else if (pNode->GetType( ) == nt_pointer)
			{
				CNodePtr* pptr = (CNodePtr*)pNode;
				#ifdef UNICODE
				CStringA strPtrNodeName = CW2A( pptr->pNode->GetName( ) );
				#else
				CStringA strPtrNodeName = pptr->pNode->Name;
				#endif

				node->SetAttribute( "Pointer", strPtrNodeName );
			}
			else if (pNode->GetType( ) == nt_instance)
			{
				CNodeClassInstance* pptr = (CNodeClassInstance*)pNode;
				#ifdef UNICODE
				CStringA strInstanceNodeName = CW2A( pptr->pNode->GetName( ) );
				#else
				CStringA strInstanceNodeName = pptr->pNode->Name;
				#endif
				node->SetAttribute( "Instance", strInstanceNodeName );
			}
			else if (pNode->GetType( ) == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->NodeCount( ); f++)
				{
					CNodeFunctionPtr* pNodefun = (CNodeFunctionPtr*)pVTable->GetNode( f );
					#ifdef UNICODE
					CStringA strFunctionNodeName = CW2A( pNodefun->GetName( ) );
					CStringA strFunctionNodeComment = CW2A( pNodefun->GetComment( ) );
					#else
					CStringA strFunctionNodeName = pNodefun->Name;
					CStringA strFunctionNodeComment = pNodefun->Comment;
					#endif

					XMLElement *fun = doc.NewElement( "Function" );
					fun->SetAttribute( "Name", strFunctionNodeName );
					fun->SetAttribute( "Comment", strFunctionNodeComment );
					fun->SetAttribute( "bHidden", pNodefun->IsHidden( ) );
					node->LinkEndChild( fun );
					for (UINT as = 0; as < pNodefun->Assembly.size( ); as++)
					{
						XMLElement *pCode = doc.NewElement( "Code" );
						CStringA strFunctionNodeAssembler = pNodefun->Assembly[as];
						pCode->SetAttribute( "Assembly", strFunctionNodeAssembler );
						fun->LinkEndChild( pCode );
					}
				}
			}
		}
	}

	char szFilename[MAX_PATH] = { 0 };
	#ifdef UNICODE
	size_t converted = 0;
	wcstombs_s( &converted, szFilename, FileName, MAX_PATH );
	#else
	strcpy_s( szFilename, FileName );
	#endif

	XMLError err = doc.SaveFile( szFilename );
	if (err == XML_NO_ERROR)
	{
		PrintOut( _T( "ReClass files saved successfully to \"%s\"" ), FileName );
		return;
	}

	PrintOut( _T( "Failed to save file to \"%s\". Error %d" ), FileName, err );
}

void CReClass2016App::OnFileSave( )
{
	if (m_strCurrentFilePath.IsEmpty( ))
	{
		OnFileSaveAs( );
	}
	else
	{
		SaveXML( m_strCurrentFilePath.GetBuffer( ) );
	}
}

void CReClass2016App::OnFileSaveAs( )
{
	TCHAR Filters[] = _T( "ReClass (*.reclass)|*.reclass|All Files (*.*)|*.*||" );
	CFileDialog fileDlg( FALSE, _T( "reclass" ), _T( "" ), OFN_HIDEREADONLY, Filters, NULL );
	if (fileDlg.DoModal( ) != IDOK)
		return;

	CString pathName = fileDlg.GetPathName( );
	m_strCurrentFilePath = pathName;
	SaveXML( pathName.GetBuffer( ) );
}

void CReClass2016App::OnFileOpen( )
{
	TCHAR Filters[] = _T( "ReClass (*.reclass)|*.reclass|All Files (*.*)|*.*||" );
	CFileDialog fileDlg( TRUE, _T( "reclass" ), _T( "" ), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, Filters, NULL );
	if (fileDlg.DoModal( ) != IDOK)
		return;

	CString pathName = fileDlg.GetPathName( );

	OnButtonReset( );

	TiXMLDocument doc;

	#ifdef UNICODE
	#define _CA2W(psz) CA2W(psz)
		// Convert path to mbs in unicode mode
	char szFilename[MAX_PATH] = { 0 };
	size_t converted = 0;
	wcstombs_s( &converted, szFilename, pathName, MAX_PATH );
	#else
	#define _CA2W(psz) (psz)
	char* szFilename = pathName.GetBuffer( );
	#endif

	XMLError ret = doc.LoadFile( szFilename );
	if (ret != XML_NO_ERROR)
		return;

	m_strCurrentFilePath = pathName;

	XMLHandle hDoc( &doc );
	XMLHandle hRoot( 0 );
	XMLElement* pElem;
	typedef std::pair<CString, CNodeBase*> Link;
	typedef std::vector<Link> Links;
	Links links;

	pElem = hDoc.FirstChildElement( ).ToElement( );
	if (!pElem)
		return;

	const char* v = pElem->Value( );
	if (_stricmp( v, "ReClass" ) != 0) // the root element value is 'ReClass'
		return; // Not a Reclass file

	hRoot = XMLHandle( pElem );

	pElem = hRoot.FirstChildElement( "Header" ).ToElement( );
	if (pElem)
	{
		m_strHeader = pElem->Attribute( "Text" );
	}

	pElem = hRoot.FirstChildElement( "Footer" ).ToElement( );
	if (pElem)
	{
		m_strFooter = pElem->Attribute( "Text" );
	}

	pElem = hRoot.FirstChildElement( "Notes" ).ToElement( );
	if (pElem)
	{
		m_strNotes = pElem->Attribute( "Text" );
	}

	pElem = hRoot.FirstChildElement( "Class" ).ToElement( );
	while (pElem)
	{
		CNodeClass* pClass = new CNodeClass;
		pClass->SetName( _CA2W( pElem->Attribute( "Name" ) ) );
		pClass->SetComment( _CA2W( pElem->Attribute( "Comment" ) ) );
		pClass->SetOffset( atoi( pElem->Attribute( "Offset" ) ) );
		pClass->SetOffsetString( _CA2W( pElem->Attribute( "strOffset" ) ) );
		pClass->Code = _CA2W( pElem->Attribute( "Code" ) );

		if (pClass->GetOffsetString( ) == "")
			pClass->SetOffsetString( _CA2W( pElem->Attribute( "Offset" ) ) );

		XMLElement* pClassElem = pElem->FirstChildElement( );
		while (pClassElem)
		{
			int Type = nt_none;
			pClassElem->QueryIntAttribute( "Type", &Type );

			if (Type != nt_none)
			{
				CNodeBase* pNode = CreateNewNode( (NodeType)Type );
				int Size = -1;
				
				pNode->SetName( _CA2W( pClassElem->Attribute( "Name" ) ) );
				pNode->SetComment( _CA2W( pClassElem->Attribute( "Comment" ) ) );
				pNode->SetHidden( atoi( pClassElem->Attribute( "bHidden" ) ) > 0 ? true : false );
				pNode->SetParent( pClass );
				pClass->AddNode( pNode );

				pClassElem->QueryIntAttribute( "Size", &Size );

				if (Type == nt_custom)
				{
					((CNodeCustom*)pNode)->memsize = Size;
				}
				else if (Type == nt_text)
				{
					((CNodeText*)pNode)->memsize = Size;
				}
				else if (Type == nt_unicode)
				{
					((CNodeText*)pNode)->memsize = Size;
				}
				else if (Type == nt_vtable)
				{
					XMLElement* pVTableElem = pClassElem->FirstChildElement( );
					while (pVTableElem)
					{
						CNodeFunctionPtr* pFun = new CNodeFunctionPtr;
						pFun->SetName( _CA2W( pVTableElem->Attribute( "Name" ) ) );
						pFun->SetComment( _CA2W( pVTableElem->Attribute( "Comment" ) ) );
						pFun->SetHidden( atoi( pVTableElem->Attribute( "bHidden" ) ) > 0 ? true : false );
						pFun->SetParent( pNode );
						pNode->AddNode( pFun );

						XMLElement* pCode = pVTableElem->FirstChildElement( );
						while (pCode)
						{
							CStringA disassembly = pCode->Attribute( "Assembly" );
							pFun->Assembly.push_back( disassembly );
							pCode = pCode->NextSiblingElement( );
						}
						pVTableElem = pVTableElem->NextSiblingElement( );
					}
				}
				else if (Type == nt_array)
				{
					//<Node Name="N4823" Type="23" Size="64" bHidden="0" Comment="" Total="1">
					//<Array Name="N12DB" Type="24" Size="64" Comment="" />
					CNodeArray* pArray = (CNodeArray*)pNode;
					pArray->Total = (DWORD)atoi( pClassElem->Attribute( "Total" ) );

					XMLElement* pArrayElem = pClassElem->FirstChildElement( );
					if (pArrayElem)
					{
						CString Name = _CA2W( pArrayElem->Attribute( "Name" ) );
						CString Comment = _CA2W( pArrayElem->Attribute( "Comment" ) );
						int ArrayType = nt_none, ArraySize = 0;
						pArrayElem->QueryIntAttribute( "Type", &ArrayType );
						pClassElem->QueryIntAttribute( "Size", &ArraySize );

						if (ArrayType == nt_class)
						{
							links.push_back( Link( Name, pNode ) );
						}
						//Handle other type of arrays....
					}
				}
				else if (Type == nt_pointer)
				{
					CString PointerStr = _CA2W( pClassElem->Attribute( "Pointer" ) );
					links.push_back( Link( PointerStr, pNode ) );
				}
				else if (Type == nt_instance)
				{
					CString InstanceStr = _CA2W( pClassElem->Attribute( "Instance" ) );
					links.push_back( Link( InstanceStr, pNode ) );
				}
			}

			pClassElem = pClassElem->NextSiblingElement( );
		}


		m_Classes.push_back( pClass );
		pElem = pElem->NextSiblingElement( "Class" );
	}

	//Fix Links... very ghetto this whole thing is just fucked
	//for (UINT i = 0; i < Links.size(); i++)
	for (auto it = links.begin( ); it != links.end( ); it++)
	{
		for (UINT i = 0; i < m_Classes.size( ); i++)
		{
			if (it->first == m_Classes[i]->GetName( ))
			{
				NodeType Type = it->second->GetType( );
				if (Type == nt_pointer)
				{
					static_cast<CNodePtr*>(it->second)->pNode = m_Classes[i];
				}
				if (Type == nt_instance)
				{
					static_cast<CNodeClassInstance*>(it->second)->pNode = m_Classes[i];
				}
				if (Type == nt_array)
				{
					static_cast<CNodeArray*>(it->second)->pNode = m_Classes[i];
				}
			}
		}
	}

	CalcAllOffsets( );
}

void CReClass2016App::OnButtonGenerate( )
{
	PrintOut( _T( "OnButtonGenerate() called" ) );

	CString strGeneratedText, t;

	strGeneratedText += _T( "// Generated using ReClass 2016\r\n\r\n" );

	if (!m_strHeader.IsEmpty( ))
		strGeneratedText += m_strHeader + _T( "\r\n\r\n" );

	for (UINT i = 0; i < m_Classes.size( ); i++)
	{
		t.Format( _T( "class %s;\r\n" ), m_Classes[i]->GetName( ) );
		strGeneratedText += t;
	}

	strGeneratedText += _T( "\r\n" );

	std::vector<CString> vfun;
	std::vector<CString> var;

	CString ClassName;

	for (UINT c = 0; c < m_Classes.size( ); c++)
	{
		CNodeClass* pClass = m_Classes[c];

		CalcOffsets( pClass );

		vfun.clear( );
		var.clear( );

		ClassName.Format( _T( "class %s" ), pClass->GetName( ) );

		int fill = 0;
		int fillStart = 0;

		for (UINT n = 0; n < pClass->NodeCount( ); n++)
		{
			CNodeBase* pNode = (CNodeBase*)pClass->GetNode( n );
			NodeType Type = pNode->GetType( );

			if ((Type == nt_hex64) || (Type == nt_hex32) || (Type == nt_hex16) || (Type == nt_hex8))
			{
				if (fill == 0)
					fillStart = (int)pNode->GetOffset( );
				fill += pNode->GetMemorySize( );
			}
			else
			{
				if (fill > 0)
				{
					if (g_bPrivatePadding)
						t.Format( _T( "private:\r\n\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\npublic:\r\n" ), g_tdHex, fillStart, fill, fillStart );
					else
						t.Format( _T( "\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\n" ), g_tdHex, fillStart, fill, fillStart );
					var.push_back( t );
				}
				fill = 0;
			}

			if (Type == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->NodeCount( ); f++)
				{
					CString fn( pVTable->GetNode( f )->GetName( ) );
					if (fn.GetLength( ) == 0)
						fn.Format( _T( "void Function%i()" ), f );
					t.Format( _T( "\tvirtual %s; //%s\r\n" ), fn, pVTable->GetNode( f )->GetComment( ) );
					vfun.push_back( t );
				}
			}

			if (Type == nt_int64)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdInt64, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_int32)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdInt32, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_int16)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdInt16, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_int8)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdInt8, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_uint64)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdQWORD, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_uint32)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdDWORD, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_uint16)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdWORD, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_uint8)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdBYTE, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}

			if (Type == nt_vec2)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdVec2, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_vec3)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdVec3, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_quat)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdQuat, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_matrix)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdMatrix, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}

			if (Type == nt_pchar)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdPChar, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_pwchar)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdPWChar, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_text)
			{
				CNodeText* pText = (CNodeText*)pNode;
				t.Format( _T( "\tchar %s[%i]; //0x%0.4X %s\r\n" ), pText->GetName( ), pText->memsize, pText->GetComment( ), pText->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_unicode)
			{
				CNodeUnicode* pText = (CNodeUnicode*)pNode;
				t.Format( _T( "\twchar_t %s[%i]; //0x%0.4X %s\r\n" ), pText->GetName( ), pText->memsize / sizeof( wchar_t ), pText->GetOffset( ), pText->GetComment( ) );
				var.push_back( t );
			}

			if (Type == nt_float)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdFloat, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}
			if (Type == nt_double)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), g_tdDouble, pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}

			if (Type == nt_custom)
			{
				t.Format( _T( "\t%s; //0x%0.4X %s\r\n" ), pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}

			if (Type == nt_functionptr)
			{
				t.Format( _T( "\t%s; //0x%0.4X %s\r\n" ), pNode->GetName( ), pNode->GetOffset( ), pNode->GetComment( ) );
				var.push_back( t );
			}

			if (Type == nt_pointer)
			{
				CNodePtr* pPointer = (CNodePtr*)pNode;
				t.Format( _T( "\t%s* %s; //0x%0.4X %s\r\n" ), pPointer->pNode->GetName( ), pPointer->GetName( ), pPointer->GetOffset( ), pPointer->GetComment( ) );
				var.push_back( t );
			}

			if (Type == nt_instance)
			{
				CNodeClassInstance* pCls = (CNodeClassInstance*)pNode;
				if (pCls->GetOffset( ) == 0)
				{
					t.Format( _T( " : public %s" ), pCls->pNode->GetName( ) ); // Inheritance
					ClassName += t;
				}
				else
				{
					t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), pCls->pNode->GetName( ), pCls->GetName( ), pCls->GetOffset( ), pCls->GetComment( ) );
					var.push_back( t );
				}
			}

			if (Type == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				t.Format( _T( "\t%s %s[%i]; //0x%0.4X %s\r\n" ), pArray->pNode->GetName( ), pArray->GetName( ), pArray->Total, pArray->GetOffset( ), pArray->GetComment( ) );
				var.push_back( t );
			}

		}

		if (fill > 0)
		{
			if (g_bPrivatePadding)
				t.Format( _T( "private:\r\n\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\n" ), g_tdHex, fillStart, fill, fillStart ); //Maybe add public at the end for user impl of class inline functions?: public:\r\n
			else
				t.Format( _T( "\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\n" ), g_tdHex, fillStart, fill, fillStart );

			var.push_back( t );
		}

		t.Format( _T( "%s\r\n{\r\npublic:\r\n" ), ClassName );
		strGeneratedText += t;

		for (UINT i = 0; i < vfun.size( ); i++)
			strGeneratedText += vfun[i];

		if (vfun.size( ) > 0)
			strGeneratedText += _T( "\r\n" );

		for (UINT i = 0; i < var.size( ); i++)
			strGeneratedText += var[i];

		if (var.size( ) > 0)
			strGeneratedText += _T( "\r\n" );

		if (pClass->Code.GetLength( ) > 0)
		{
			strGeneratedText += pClass->Code;
			strGeneratedText += _T( "\r\n" );
		}

		t.Format( _T( "}; //Size=0x%0.4X\r\n\r\n" ), pClass->GetMemorySize( ) );
		strGeneratedText += t;
	}

	if (!m_strFooter.IsEmpty( ))
	{
		strGeneratedText += (m_strFooter + _T( "\r\n" ));
	}

	if (g_bClipboardCopy)
	{
		int stringSize = 0;
		HGLOBAL MemoryBlob = NULL;

		::OpenClipboard( NULL );
		::EmptyClipboard( );

		stringSize = strGeneratedText.GetLength( ) * sizeof( CString::StrTraits::XCHAR );
		MemoryBlob = ::GlobalAlloc( GMEM_FIXED, stringSize );
		memcpy( MemoryBlob, strGeneratedText.GetBuffer( ), stringSize );

		#ifdef UNICODE
		::SetClipboardData( CF_UNICODETEXT, MemoryBlob );
		#else
		::SetClipboardData( CF_TEXT, hMemBlob );
		#endif

		::CloseClipboard( );
		
		GetMainWnd( )->MessageBox( _T( "Copied generated code to clipboard" ), _T( "ReClass 2016" ), MB_OK | MB_ICONINFORMATION );
	}
	else
	{
		CDialogEdit dlg;
		dlg.Title = _T( "Class Code Generated" );
		dlg.Text = strGeneratedText;
		dlg.DoModal( );
	}
}

void CReClass2016App::OnButtonPlugins( )
{
	CDialogPlugins plugin_dlg;
	plugin_dlg.DoModal( );
}

void CReClass2016App::OnUpdateButtonPlugins( CCmdUI * pCmdUI )
{
	pCmdUI->Enable( !g_LoadedPlugins.empty( ) );
}

void CReClass2016App::OnOpenPDB( )
{
	CString strConcatProcessName = g_ProcessName;
	if (strConcatProcessName.ReverseFind( '.' ) != -1)
		strConcatProcessName.Truncate( strConcatProcessName.ReverseFind( '.' ) );

	CFileDialog fileDlg( TRUE, _T( "pdb" ), strConcatProcessName, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "PDB (*.pdb)|*.pdb|All Files (*.*)|*.*||" ), NULL );
	if (fileDlg.DoModal( ) != IDOK)
		return;

	m_pSymbolLoader->LoadSymbolsForPdb( fileDlg.GetPathName( ) );
}

void CReClass2016App::OnUpdateOpenPDB( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( (g_ProcessID != NULL) );
}

void CReClass2016App::DeleteClass( CNodeClass* pClass )
{
	PrintOut( _T( "DeleteClass(\"%s\") called" ), pClass->GetName( ).GetString( ) );

	CNodeBase* pNode = IsNodeRef( pClass );
	if (pNode)
	{
		PrintOut( _T( "Class still has a reference in %s.%s" ), pNode->GetParent( )->GetName( ).GetString( ), pNode->GetName( ).GetString( ) );
		CString msg;
		msg.Format( _T( "Class still has a reference in %s.%s" ), pNode->GetParent( )->GetName( ).GetString( ), pNode->GetName( ).GetString( ) );
		GetMainWnd( )->MessageBox( msg );
		return;
	}

	for (UINT i = 0; i < m_Classes.size( ); i++)
	{
		if (m_Classes[i] == pClass)
		{
			m_Classes.erase( m_Classes.begin( ) + i );
			return;
		}
	}
}

CNodeBase* CReClass2016App::IsNodeRef( CNodeBase* pTestNode )
{
	for (UINT c = 0; c < m_Classes.size( ); c++)
	{
		CNodeClass* pClass = (CNodeClass*)m_Classes[c];
		for (UINT n = 0; n < pClass->NodeCount( ); n++)
		{
			CNodeBase* pNode = pClass->GetNode( n );
			if (!pNode)
				continue;

			NodeType nt = pNode->GetType( );
			if (nt == nt_instance)
			{
				CNodeClassInstance* pInstance = (CNodeClassInstance*)pNode;
				if (pInstance->pNode == pTestNode)
					return pInstance;
			}
			else if (nt == nt_pointer)
			{
				CNodePtr* pInstance = (CNodePtr*)pNode;
				if (pInstance->pNode == pTestNode)
					return pInstance;
			}
			else if (nt == nt_array)
			{
				CNodeArray* pInstance = (CNodeArray*)pNode;
				if (pInstance->pNode == pTestNode)
					return pInstance;
			}
		}
	}
	return NULL;
}

void CReClass2016App::OnButtonClean( )
{
	CMDIFrameWnd* pFrame = STATIC_DOWNCAST( CMDIFrameWnd, m_pMainWnd );
	CMDIChildWnd* wnd = pFrame->MDIGetActive( );
	while (wnd)
	{
		wnd->SendMessage( WM_CLOSE, 0, 0 );
		wnd = pFrame->MDIGetActive( );
	}

	std::vector<CNodeClass*> ClassesToCheck;
	for (UINT i = 0; i < m_Classes.size( ); i++)
	{
		if (IsNodeRef( m_Classes[i] ) == NULL)
			ClassesToCheck.push_back( m_Classes[i] );
	}

	int count = 0;
	for (UINT i = 0; i < ClassesToCheck.size( ); i++)
	{
		CNodeClass* pClass = ClassesToCheck[i];
		bool bCanDelete = true;

		for (UINT n = 0; n < pClass->NodeCount( ); n++)
		{
			CNodeBase* pNode = pClass->GetNode( n );
			NodeType Type = pNode->GetType( );

			if (Type == nt_hex64 || Type == nt_hex32 || Type == nt_hex16 || Type == nt_hex8)
				continue;

			bCanDelete = false;

			break;
		}

		if (bCanDelete)
		{
			count++;
			DeleteClass( pClass );
		}
	}

	PrintOut( _T( "Unused Classes removed: %i" ), count );
	CString msg; msg.Format( _T( "Unused Classes removed: %i" ), count );
	MessageBox( GetMainWnd( )->GetSafeHwnd( ), msg.GetString( ), _T( "Cleaner" ), MB_OK );
}

void CReClass2016App::OnUpdateButtonClean( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( (g_ReClassApp.m_Classes.size( ) > 0) );
}

