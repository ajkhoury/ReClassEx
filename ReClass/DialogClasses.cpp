#include "stdafx.h"
#include "ReClassEx.h"
#include "DialogClasses.h"

#include "CMainFrame.h"
#include "CClassFrame.h"

#include "afxdialogex.h"

IMPLEMENT_DYNAMIC( CDialogClasses, CDialogEx )

CDialogClasses::CDialogClasses( CWnd* pParent ) : CDialogEx( CDialogClasses::IDD, pParent )
{
}

CDialogClasses::~CDialogClasses( )
{
}

void CDialogClasses::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_CLASSLIST, m_ClassViewList );
	DDX_Control( pDX, IDC_CLASSNAME, m_Edit );
}

BEGIN_MESSAGE_MAP( CDialogClasses, CDialogEx )
	ON_EN_CHANGE( IDC_CLASSNAME, &CDialogClasses::OnEnChangeClassname )
	ON_NOTIFY( NM_DBLCLK, IDC_CLASSLIST, &CDialogClasses::OnDblclkListControl )
END_MESSAGE_MAP( )


void CDialogClasses::BuildList( )
{
	RECT listRect;
	m_ClassViewList.GetWindowRect( &listRect );
	m_ClassViewList.InsertColumn( 0, _T( "Class" ), LVCFMT_CENTER, listRect.right - listRect.left - 4 );

	m_ImageList.Add( m_hClassIcon );

	m_ClassViewList.SetImageList( &m_ImageList, LVSIL_SMALL );

	for (size_t i = 0; i < g_ReClassApp.m_Classes.size( ); i++)
	{
		CString name = g_ReClassApp.m_Classes[i]->GetName( );
		if (m_Filter.GetLength( ) != 0 && name.MakeUpper( ).Find( m_Filter.MakeUpper( ) ) == -1)
			continue;
		AddData( (int)i, 0, name );
	}
}

void CDialogClasses::OnDblclkListControl( LPNMHDR pnmhdr, LRESULT *lpresult )
{
	CDialogClasses::OnOK( );
}

BOOL CDialogClasses::OnInitDialog( )
{
	CDialogEx::OnInitDialog( );

	m_ImageList.Create( GetSystemMetrics( SM_CXSMICON ), GetSystemMetrics( SM_CYSMICON ), ILC_COLOR32, 1, 1 );
	m_ImageList.SetBkColor( RGB( 255, 255, 255 ) );
	m_hClassIcon = ::LoadIcon( AfxGetResourceHandle( ), MAKEINTRESOURCE( IDI_ICON_CLASS ) );
	m_ImageList.Add( m_hClassIcon );

	BuildList( );

	return TRUE;
}

inline int FindClassByName( const TCHAR* szName )
{
	for (size_t id = 0; id < g_ReClassApp.m_Classes.size( ); id++)
	{
		CNodeClass* pNodeClass = g_ReClassApp.m_Classes[id];
		if (!pNodeClass)
			continue;
		if (_tcsicmp( pNodeClass->GetName( ), szName ) == 0)
			return (int)id;
	}
	return -1;
};

void CDialogClasses::OnOK( )
{
	UINT numselected = m_ClassViewList.GetSelectedCount( );
	POSITION pos = m_ClassViewList.GetFirstSelectedItemPosition( );
	while (pos)
	{
		int nItem = m_ClassViewList.GetNextSelectedItem( pos );
		CString szBuffer = m_ClassViewList.GetItemText( nItem, 0 );

		#ifdef _DEBUG
		PrintOut( _T( "nitem %d" ), nItem );
		#endif

		nItem = FindClassByName( szBuffer.GetBuffer( ) );

		// Thanks timboy67678
		CMainFrame*  pFrame = STATIC_DOWNCAST( CMainFrame, AfxGetApp( )->m_pMainWnd );
		CClassFrame* pChild = g_ReClassApp.m_Classes[nItem]->pChildWindow;

		// Check if its a window first to dodge the assertion in IsWindowVisible
		if (pChild && IsWindow( pChild->GetSafeHwnd( ) ) && pChild->IsWindowVisible( ))
		{
			static_cast<CMDIChildWnd*>(pChild)->MDIActivate( );
		}
		else
		{
			CClassFrame* pNewChild = STATIC_DOWNCAST( CClassFrame, pFrame->CreateNewChild( RUNTIME_CLASS( CClassFrame ), IDR_ReClass2016TYPE, g_ReClassApp.m_hMDIMenu, g_ReClassApp.m_hMDIAccel ) );
			pNewChild->SetClass( g_ReClassApp.m_Classes[nItem] );
			pNewChild->SetTitle( g_ReClassApp.m_Classes[nItem]->GetName( ) );
			pNewChild->SetWindowText( g_ReClassApp.m_Classes[nItem]->GetName( ) );

			g_ReClassApp.m_Classes[nItem]->SetChildFrame( pNewChild );

			pFrame->UpdateFrameTitleForDocument( g_ReClassApp.m_Classes[nItem]->GetName( ) );
		}
	}

	CDialogEx::OnOK( );
}

void CDialogClasses::AddData( int row, int col, const TCHAR* str )
{
	LVITEM lv;
	ZeroMemory( &lv, sizeof( LVITEM ) );
	lv.iItem = row;
	lv.iSubItem = col;
	lv.pszText = (LPTSTR)str;

	if (col == 0)
	{
		lv.mask = LVIF_IMAGE | LVIF_TEXT;
		lv.iImage = 0;
		m_ClassViewList.InsertItem( &lv );
	}
	else
	{
		lv.mask = LVIF_TEXT;
		m_ClassViewList.SetItem( &lv );
	}
}

void CDialogClasses::OnEnChangeClassname( )
{
	m_Edit.GetWindowText( m_Filter );
	m_ClassViewList.DeleteAllItems( );
	BuildList( );
}
