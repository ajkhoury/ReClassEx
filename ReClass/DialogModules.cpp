#include "stdafx.h"

#include "DialogModules.h"
#include "DialogProgress.h"

#include "CMainFrame.h"
#include "CClassFrame.h"

#include "afxdialogex.h"

#include <algorithm>

IMPLEMENT_DYNAMIC( CDialogModules, CDialogEx )

CDialogModules::CDialogModules( CWnd* pParent )
	: CDialogEx( CDialogModules::IDD, pParent ),
	m_bSortAscendingName( false ),
	m_bSortAscendingStart( false ),
	m_bSortAscendingEnd( false ),
	m_bSortAscendingSize( false )
{
}

CDialogModules::~CDialogModules( )
{
}

void CDialogModules::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_MODULELIST, m_ModuleList );
	DDX_Control( pDX, IDC_MODULENAME, m_Edit );
	DDX_Control( pDX, IDC_MODULES_DEBUG_LOAD, m_SymbolLoad );
}

void CDialogModules::OnSize( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize( nType, cx, cy );
}

void CDialogModules::OnGetMinMaxInfo( MINMAXINFO *lpinfo )
{
	if (!m_OriginalSize.IsRectNull( ))
	{
		lpinfo->ptMinTrackSize.x = m_OriginalSize.Width( );
		lpinfo->ptMinTrackSize.y = m_OriginalSize.Height( );
	}
	CDialogEx::OnGetMinMaxInfo( lpinfo );
}

void CDialogModules::OnContextMenu( CWnd* pWnd, CPoint pos )
{
	CDialogEx::OnContextMenu( pWnd, pos );
}

BEGIN_MESSAGE_MAP( CDialogModules, CDialogEx )
	ON_NOTIFY( NM_DBLCLK, IDC_MODULELIST, CDialogModules::OnDblClkListControl )
	ON_NOTIFY( LVN_COLUMNCLICK, IDC_MODULELIST, CDialogModules::OnColumnClick )
	ON_EN_CHANGE( IDC_MODULENAME, &CDialogModules::OnEnChangeModuleName )
	ON_WM_GETMINMAXINFO( )
	ON_WM_SIZE( )
END_MESSAGE_MAP( )

void CDialogModules::BuildList( )
{
	for (UINT idx = 0; idx < g_MemMapModules.size( ); idx++)
	{
		MemMapInfo moduleInfo = g_MemMapModules[idx];

		SHFILEINFO sfi = { 0 };
		SHGetFileInfo( moduleInfo.Path, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof( SHFILEINFO ), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES );
		m_ModuleIcons.Add( sfi.hIcon );

		CString uppercase_name = CString( moduleInfo.Name ).MakeUpper( );
		if (m_Filter.GetLength( ) != 0 && uppercase_name.Find( m_Filter.MakeUpper( ) ) == -1)
			continue;

		TCHAR strStart[64] = { 0 };
		_stprintf_s( strStart, _T( "0x%IX" ), moduleInfo.Start );
		TCHAR strEnd[64] = { 0 };
		_stprintf_s( strEnd, _T( "0x%IX" ), moduleInfo.End );
		TCHAR strSize[64] = { 0 };
		_stprintf_s( strSize, _T( "0x%X" ), moduleInfo.Size );

		AddData(
			idx,
			moduleInfo.Name.GetString( ),
			moduleInfo.Path.GetString( ),
			const_cast<LPCTSTR>(strStart),
			const_cast<LPCTSTR>(strEnd),
			const_cast<LPCTSTR>(strSize),
			static_cast<LPARAM>(moduleInfo.Start)
		);
	}
}

BOOL CDialogModules::OnInitDialog( )
{
	CDialogEx::OnInitDialog( );

	SetIcon( NULL, TRUE );
	SetIcon( NULL, FALSE );

	GetWindowRect( &m_OriginalSize );
	ScreenToClient( &m_OriginalSize );

	m_SymbolLoad.EnableWindow( g_bSymbolResolution ? TRUE : FALSE );
	m_SymbolLoad.SetCheck( g_bLoadModuleSymbol ? BST_CHECKED : BST_UNCHECKED );

	m_ModuleIcons.Create( 15, 15, ILC_COLOR32, 1, 1 );
	m_ModuleIcons.SetBkColor( RGB( 255, 255, 255 ) );

	m_ModuleList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER );

	m_ModuleList.InsertColumn( COLUMN_NAME, _T( "Module" ), LVCFMT_LEFT, 140 );
	m_ModuleList.InsertColumn( COLUMN_PATH, _T( "Path" ), LVCFMT_LEFT, 200 );
	m_ModuleList.InsertColumn( COLUMN_START, _T( "Start" ), LVCFMT_LEFT, 100 );
	m_ModuleList.InsertColumn( COLUMN_END, _T( "End" ), LVCFMT_LEFT, 100 );
	m_ModuleList.InsertColumn( COLUMN_SIZE, _T( "Size" ), LVCFMT_LEFT, 80 );

	m_ModuleList.SetImageList( &m_ModuleIcons, LVSIL_SMALL );

	BuildList( );

	return TRUE;
}

inline int CDialogModules::FindModuleByName( const TCHAR* szName )
{
	for (UINT id = 0; id < g_MemMapModules.size( ); id++)
	{
		MemMapInfo moduleInfo = g_MemMapModules[id];
		if (_tcsicmp( moduleInfo.Name, szName ) == 0)
			return id;
	}
	return -1;
}

inline CNodeClass* CDialogModules::GetClassByName( const TCHAR* szClassName )
{
	auto iter = std::find_if( g_ReClassApp.m_Classes.begin( ), g_ReClassApp.m_Classes.end( ), 
							  [szClassName] ( const CNodeClass* pNode ) -> bool { return (pNode->GetName( ).CompareNoCase( szClassName ) == 0); } );
	return (iter != g_ReClassApp.m_Classes.end( )) ? *iter : NULL;
}

void CDialogModules::SetSelected( )
{
	POSITION pos = m_ModuleList.GetFirstSelectedItemPosition( );
	while (pos)
	{
		int nItem = m_ModuleList.GetNextSelectedItem( pos );
		nItem = FindModuleByName( m_ModuleList.GetItemText( nItem, 0 ) );

		MemMapInfo mod = g_MemMapModules[nItem];

		if (g_bSymbolResolution && m_SymbolLoad.GetCheck( ) == BST_CHECKED)
			g_ReClassApp.m_pSymbolLoader->LoadSymbolsForModule( mod.Path, mod.Start, mod.Size );

		int extension_size = mod.Name.ReverseFind( '.' );
		extension_size = (extension_size == -1) ? 0 : (mod.Name.GetLength( ) - extension_size);

		CString ClassName = mod.Name.Left( mod.Name.GetLength( ) - extension_size ) + _T( "_base" );

		CNodeClass* pNewClass = GetClassByName( ClassName );

		if (pNewClass != NULL)
		{
			CMDIFrameWnd* pMainFrame = STATIC_DOWNCAST( CMDIFrameWnd, AfxGetApp( )->m_pMainWnd );
			CClassFrame* pChildClassFrame = pNewClass->m_pChildClassFrame;

			// Check if its a window first to dodge the assertion in IsWindowVisible
			if (pChildClassFrame != nullptr &&
                IsWindow( pChildClassFrame->GetSafeHwnd( ) ) &&
                pChildClassFrame->IsWindowVisible( ))
			{
				static_cast<CMDIChildWnd*>(pChildClassFrame)->MDIActivate( );
			}
			else
			{
				CClassFrame* pNewChildClassFrame = STATIC_DOWNCAST( CClassFrame, 
                    pMainFrame->CreateNewChild( RUNTIME_CLASS( CClassFrame ), IDR_ReClassExTYPE, g_ReClassApp.m_hMdiMenu, g_ReClassApp.m_hMdiAccel ) );

                pNewChildClassFrame->SetClass( pNewClass );
                pNewChildClassFrame->SetTitle( pNewClass->GetName( ) );
                pNewChildClassFrame->SetWindowText( pNewClass->GetName( ) );

				pNewClass->SetChildClassFrame( pNewChildClassFrame );

                pMainFrame->UpdateFrameTitleForDocument( pNewClass->GetName( ) );
			}
		}
		else
		{
			CMainFrame* pMainFrame = static_cast<CMainFrame*>(AfxGetApp( )->m_pMainWnd);
			CClassFrame* pChildClassFrame = STATIC_DOWNCAST( CClassFrame, 
                pMainFrame->CreateNewChild( RUNTIME_CLASS( CClassFrame ), IDR_ReClassExTYPE, g_ReClassApp.m_hMdiMenu, g_ReClassApp.m_hMdiAccel ));

			pNewClass = new CNodeClass;
			pNewClass->SetName( ClassName );

			TCHAR strStart[64];
			_stprintf_s( strStart, 64, _T( "%IX" ), mod.Start );
			pNewClass->SetOffsetString( strStart );
			pNewClass->SetOffset( mod.Start );
			pNewClass->m_pChildClassFrame = pChildClassFrame;
			pNewClass->m_Idx = g_ReClassApp.m_Classes.size( );

			g_ReClassApp.m_Classes.push_back( pNewClass );

			DWORD offset = 0;
			for (int i = 0; i < 64 / sizeof( size_t ); i++)
			{
				CNodeHex* pNode = new CNodeHex;
				pNode->SetParent( pNewClass );
				pNode->SetOffset( offset );
				offset += pNode->GetMemorySize( );
				pNewClass->AddNode( pNode );
			}

			pChildClassFrame->SetClass( pNewClass );
			pChildClassFrame->SetTitle( pNewClass->GetName( ) );
			pChildClassFrame->SetWindowText( pNewClass->GetName( ) );

			pMainFrame->UpdateFrameTitleForDocument( pNewClass->GetName( ) );
		}
	}
}

int CALLBACK CDialogModules::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	LPCOMPARESTRUCT Compare = (LPCOMPARESTRUCT)lParamSort;
	if (Compare)
	{
		CListCtrl* pListCtrl = (CListCtrl*)Compare->pListCtrl;
		int column = Compare->iColumn;
		bool ascending = Compare->bAscending;

		int item1 = ascending ? static_cast<int>(lParam1) : static_cast<int>(lParam2);
		int item2 = ascending ? static_cast<int>(lParam2) : static_cast<int>(lParam1);

		if (column == COLUMN_START || column == COLUMN_END || column == COLUMN_SIZE)
		{
			CString strValue1 = pListCtrl->GetItemText( item1, column );
			CString strValue2 = pListCtrl->GetItemText( item2, column );

			size_t value1 = (size_t)_tcstoui64( strValue1.GetBuffer( ), NULL, 16 );
			size_t value2 = (size_t)_tcstoui64( strValue2.GetBuffer( ), NULL, 16 );

			return (int)(value2 - value1);
		}
		else if (column == COLUMN_NAME)
		{
			CString strModuleName1 = pListCtrl->GetItemText( item1, column );
			CString strModuleName2 = pListCtrl->GetItemText( item2, column );

			return _tcsicmp( strModuleName1.GetBuffer( ), strModuleName2.GetBuffer( ) );
		}
		else if (column == COLUMN_PATH)
		{
			CString strModulePath1 = pListCtrl->GetItemText( item1, column );
			CString strModulePath2 = pListCtrl->GetItemText( item2, column );

			int idxSlash1 = strModulePath1.ReverseFind( _T( '\\' ) );
			if (!idxSlash1)
				idxSlash1 = strModulePath1.ReverseFind( _T( '/' ) );
			CString strModuleFile1 = strModulePath1.Mid( ++idxSlash1 );

			int idxSlash2 = strModulePath2.ReverseFind( _T( '\\' ) );
			if (!idxSlash2)
				idxSlash2 = strModulePath2.ReverseFind( _T( '/' ) );
			CString strModuleFile2 = strModulePath2.Mid( ++idxSlash2 );

			return _tcsicmp( strModuleFile1.GetBuffer( ), strModuleFile2.GetBuffer( ) );
		}
	}
	return 0;
}

void CDialogModules::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LPCOMPARESTRUCT compare = new COMPARESTRUCT;
	compare->pListCtrl = &m_ModuleList;
	compare->iColumn = pNMListView->iSubItem;

	switch (compare->iColumn)
	{
	case COLUMN_NAME:
		m_bSortAscendingName = !m_bSortAscendingName;
		compare->bAscending = m_bSortAscendingName;
		break;
	case COLUMN_PATH:
		m_bSortAscendingPath = !m_bSortAscendingPath;
		compare->bAscending = m_bSortAscendingPath;
		break;
	case COLUMN_START:
		m_bSortAscendingStart = !m_bSortAscendingStart;
		compare->bAscending = m_bSortAscendingStart;
		break;
	case COLUMN_END:
		m_bSortAscendingEnd = !m_bSortAscendingEnd;
		compare->bAscending = m_bSortAscendingEnd;
		break;
	case COLUMN_SIZE:
		m_bSortAscendingSize = !m_bSortAscendingSize;
		compare->bAscending = m_bSortAscendingSize;
		break;
	default:
		compare->bAscending = false;
		break;
	}

	m_ModuleList.SortItemsEx( CompareFunction, (LPARAM)compare );

	delete compare;

	*pResult = 0;
}

void CDialogModules::OnDblClkListControl( NMHDR* pNMHDR, LRESULT* pResult )
{
	OnOK( );
}

void CDialogModules::OnOK( )
{
	SetSelected( );
	if (m_SymbolLoad.GetCheck( ) == BST_CHECKED)
		g_bSymbolResolution = true;
	CDialogEx::OnOK( );
}

int CDialogModules::AddData( int Index, LPCTSTR ModuleName, LPCTSTR ModulePath, LPCTSTR StartAddress, LPCTSTR EndAddress, LPCTSTR ModuleSize, LPARAM lParam )
{
	LVITEM lvi;
	int pos = 0;
	
	ZeroMemory( &lvi, sizeof( LVITEM ) );
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.pszText = (LPTSTR)ModuleName;
	lvi.cchTextMax = static_cast<int>(_tcslen( ModuleName )) + 1;
	lvi.iImage = Index;
	lvi.lParam = lParam;
	lvi.iItem = m_ModuleList.GetItemCount( );

	pos = m_ModuleList.InsertItem( &lvi );

	m_ModuleList.SetItemText( pos, COLUMN_PATH, (LPCTSTR)ModulePath );
	m_ModuleList.SetItemText( pos, COLUMN_START, (LPCTSTR)StartAddress );
	m_ModuleList.SetItemText( pos, COLUMN_END, (LPCTSTR)EndAddress );
	m_ModuleList.SetItemText( pos, COLUMN_SIZE, (LPCTSTR)ModuleSize );

	return pos;
}

void CDialogModules::OnEnChangeModuleName( )
{
	m_Edit.GetWindowText( m_Filter );
	m_ModuleList.DeleteAllItems( );
	BuildList( );
}
