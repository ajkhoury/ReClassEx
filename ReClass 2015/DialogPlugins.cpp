// DialogPlugins.cpp : implementation file
//

#include "stdafx.h"
#include "DialogPlugins.h"
#include "ReClass2015.h"
#include "afxdialogex.h"

// CDialogPlugins dialog

IMPLEMENT_DYNAMIC(CDialogPlugins, CDialogEx)

CDialogPlugins::CDialogPlugins(CWnd* pParent) 
	: CDialogEx(CDialogPlugins::IDD, pParent)
{
	
}

CDialogPlugins::~CDialogPlugins()
{
	
}

// CDialogPlugins message handlers

BEGIN_MESSAGE_MAP( CDialogPlugins, CDialogEx )
	ON_WM_GETMINMAXINFO( )
	ON_NOTIFY( NM_RCLICK, IDC_PLUGIN_LIST, &CDialogPlugins::OnRightClickList )
END_MESSAGE_MAP( )

void CDialogPlugins::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control( pDX, IDC_PLUGIN_LIST, m_PluginsList );
	CDialogEx::DoDataExchange(pDX);
}

BOOL CDialogPlugins::OnInitDialog( )
{
	CDialogEx::OnInitDialog( );
	
	GetWindowRect( &m_OriginalSize );
	ScreenToClient( &m_OriginalSize );

	m_PluginsList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER );

	m_PluginsList.InsertColumn( 0, _T( "Plugins" ), LVCFMT_CENTER, 300 ); //TODO: Correct sizes
	m_PluginsList.InsertColumn( 1, _T( "Version" ), LVCFMT_CENTER, 120 );
	
	RefreshPlugins( );

	return TRUE;
}

void CDialogPlugins::OnGetMinMaxInfo( MINMAXINFO * lpinfo )
{
	CDialogEx::OnGetMinMaxInfo( lpinfo );

	if ( !m_OriginalSize.IsRectNull( ) )
	{
		lpinfo->ptMinTrackSize.x = m_OriginalSize.Width( );
		lpinfo->ptMinTrackSize.y = m_OriginalSize.Height( );
	}
}

void CDialogPlugins::OnRightClickList( NMHDR * pNotifyStruct, LRESULT * result )
{ 
	NMITEMACTIVATE *item_active = reinterpret_cast<NMITEMACTIVATE *>( pNotifyStruct );
	
	if ( item_active->iItem != -1 )
	{
		MessageBoxW(LoadedPlugins[item_active->iItem].Info.About);
	}
}

void CDialogPlugins::RefreshPlugins( )
{
	m_PluginsList.DeleteAllItems( );

	LVITEM item;

	for ( auto plugin : LoadedPlugins )
	{
		ZeroMemory( &item, sizeof LVITEM );

		item.mask = LVIF_TEXT;
		item.pszText = plugin.Info.Name;
		item.cchTextMax = wcslen( plugin.Info.Name ) + 1;
		item.iItem = m_PluginsList.GetItemCount( );

		int pos = m_PluginsList.InsertItem( &item );

		m_PluginsList.SetItemText( pos, 1, plugin.Info.Version );
	}
}
