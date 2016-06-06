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
	ON_COMMAND(MENU_SETTINGS, &CDialogPlugins::OnPopupMenuSettings)
	ON_COMMAND(MENU_ABOUT, &CDialogPlugins::OnPopupMenuAbout)
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

	m_PluginsList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER );

	m_PluginsList.InsertColumn( 0, _T( "Plugins" ), LVCFMT_CENTER, 300 ); //TODO: Correct sizes?
	m_PluginsList.InsertColumn( 1, _T( "Version" ), LVCFMT_CENTER, 120 );
	
	RefreshPlugins( );

	return TRUE;
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
			RECLASS_PLUGINS plugin = LoadedPlugins[m_PluginsList.GetSelectionMark()];
			
			CString About;
			About.Format( _T( "&About \"%ls\"" ), plugin.Info.Name );

			CMenu context_menu;
			context_menu.CreatePopupMenu( );
			context_menu.AppendMenu( MF_STRING, MENU_SETTINGS, _T( "Settings" ) );
			context_menu.AppendMenu( MF_STRING, MENU_ABOUT, About );
			context_menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this );
		}
	}
}

void CDialogPlugins::OnPopupMenuSettings( )
{ 
	RECLASS_PLUGINS plugin = LoadedPlugins[m_PluginsList.GetSelectionMark()];
	//TODO: 
}

void CDialogPlugins::OnPopupMenuAbout( )
{ 
	RECLASS_PLUGINS plugin = LoadedPlugins[m_PluginsList.GetSelectionMark()];
	//TODO: make cool about dialog maybe??
	MessageBoxW( plugin.Info.About, plugin.Info.Name, MB_OK | MB_ICONINFORMATION);
}

void CDialogPlugins::RefreshPlugins( )
{
	m_PluginsList.DeleteAllItems( );

	LVITEM item;

	for ( RECLASS_PLUGINS plugin : LoadedPlugins )
	{
		ZeroMemory( &item, sizeof LVITEM );

		item.mask = LVIF_TEXT;
		item.pszText = plugin.Info.Name;
		item.cchTextMax = (int)wcslen( plugin.Info.Name ) + 1;
		item.iItem = m_PluginsList.GetItemCount( );

		int pos = m_PluginsList.InsertItem( &item );

		m_PluginsList.SetItemText( pos, 1, plugin.Info.Version );
	}
}
