// DialogPlugins.cpp : implementation file
//

#include "stdafx.h"
#include "DialogPlugins.h"
#include "ReClass2015.h"
#include "afxdialogex.h"

// CDialogPlugins dialog

IMPLEMENT_DYNAMIC(CDialogPlugins, CDialogEx)

// CDialogPlugins message handlers

BEGIN_MESSAGE_MAP(CDialogPlugins, CDialogEx)
	ON_COMMAND(IDR_PLUGIN_MENU_SETTINGS, &CDialogPlugins::OnPopupMenuSettings)
	ON_COMMAND(IDR_PLUGIN_MENU_ABOUT, &CDialogPlugins::OnPopupMenuAbout)
	ON_WM_GETMINMAXINFO()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

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

	m_PluginsList.InsertColumn( 0, _T( "Plugins" ), LVCFMT_CENTER, 300 ); //TODO: Correct sizes?
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

void CDialogPlugins::OnContextMenu( CWnd *pWnd, CPoint pos )
{ 
	CDialogEx::OnContextMenu( pWnd, pos );

	if ( *pWnd == m_PluginsList )
	{
		//FUTURE: Use items rect to check if mouse pos is inside of rect when right clicked, not needed but maybe if it becomes a problem down the road...
		POSITION selected_pos = m_PluginsList.GetFirstSelectedItemPosition( );
		if ( selected_pos != nullptr && m_PluginsList.GetNextSelectedItem( selected_pos ) != -1 )
		{
			CMenu context_menu;
			context_menu.LoadMenu( IDR_PLUGIN_POPUP_MENU );
			CMenu *popup_menu = context_menu.GetSubMenu( 0 );
			popup_menu->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this );
		}
	}
}

void CDialogPlugins::OnPopupMenuSettings( )
{ 
	RECLASS_PLUGINS plugin = LoadedPlugins[m_PluginsList.GetSelectionMark( )];
	//TODO: 
}

void CDialogPlugins::OnPopupMenuAbout( )
{ 
	RECLASS_PLUGINS plugin = LoadedPlugins[m_PluginsList.GetSelectionMark( )];
		
	MessageBoxW( plugin.Info.About );
}

void CDialogPlugins::OnRightClickPluginList( NMHDR *pNotifyStruct, LRESULT *result )
{ 
	LPNMITEMACTIVATE item_active = reinterpret_cast<LPNMITEMACTIVATE>( pNotifyStruct );
	
	if ( item_active->iItem != -1 )
	{
		CRect list_size;
		GetWindowRect( &list_size );

		RECLASS_PLUGINS selected_plugin = LoadedPlugins[ item_active->iItem ];

		CString About;
		About.Format( _T( "About \"%ls\"" ), selected_plugin.Info.Name );

		CMenu context_menu;
		context_menu.CreatePopupMenu( );
		context_menu.AppendMenu( MF_STRING, 0, About );
		context_menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, 
									 item_active->ptAction.x + list_size.left, item_active->ptAction.y + list_size.top, this );
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
