// DialogPlugins.cpp : implementation file
//

#include "stdafx.h"
#include "DialogPlugins.h"
#include "ReClassEx.h"
#include "afxdialogex.h"

// CDialogPlugins dialog
IMPLEMENT_DYNAMIC( CDialogPlugins, CDialogEx )

// CDialogPlugins message handlers
BEGIN_MESSAGE_MAP( CDialogPlugins, CDialogEx )
    ON_COMMAND( MENU_SETTINGS, &CDialogPlugins::OnPopupMenuSettings )
    ON_COMMAND( MENU_ABOUT, &CDialogPlugins::OnPopupMenuAbout )
    ON_COMMAND( MENU_STATECHANGE, &CDialogPlugins::OnPopupMenuChangeState )
    ON_WM_CONTEXTMENU( )
END_MESSAGE_MAP( )

void CDialogPlugins::DoDataExchange( CDataExchange* pDX )
{
    DDX_Control( pDX, IDC_PLUGIN_LIST, m_PluginsList );
    CDialogEx::DoDataExchange( pDX );
}

BOOL CDialogPlugins::OnInitDialog( )
{
    CDialogEx::OnInitDialog( );

    m_PluginsList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER );

    m_PluginsList.InsertColumn( 0, _T( "Plugins" ), LVCFMT_CENTER, 280 );
    m_PluginsList.InsertColumn( 1, _T( "Version" ), LVCFMT_CENTER, 65 );
    m_PluginsList.InsertColumn( 2, _T( "State" ), LVCFMT_CENTER, 65 );

    RefreshPlugins( );

    return TRUE;
}

void CDialogPlugins::OnContextMenu( CWnd *pWnd, CPoint pos )
{
    CDialogEx::OnContextMenu( pWnd, pos );

    if (*pWnd == m_PluginsList)
    {
        //
        // FUTURE: Use items rect to check if mouse pos is inside of rect when right clicked, not needed but maybe 
        //         if it becomes a problem down the road...
        POSITION SelectedPos = m_PluginsList.GetFirstSelectedItemPosition( );
        if (SelectedPos != nullptr && m_PluginsList.GetNextSelectedItem( SelectedPos ) != -1)
        {
            CString About;
            CMenu ContextMenu;
            PRECLASS_PLUGIN Plugin;

            Plugin = GetSelectedPlugin( );
    
            About.Format( _T( "About \"%ls\"" ), Plugin->Info.Name );
            
            ContextMenu.CreatePopupMenu( );
            if (Plugin->SettingDlgFunction != NULL)
                ContextMenu.AppendMenu( MF_STRING, MENU_SETTINGS, _T( "Settings" ) );
            if (Plugin->StateChangeFunction != NULL)
                ContextMenu.AppendMenu( MF_STRING, MENU_STATECHANGE, Plugin->State ? _T( "Disable" ) : _T( "Enable" ) );
            ContextMenu.AppendMenu( MF_STRING, MENU_ABOUT, About );
            ContextMenu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON, pos.x, pos.y, this );
        }
    }
}

void CDialogPlugins::OnPopupMenuSettings( )
{
    PRECLASS_PLUGIN Plugin = GetSelectedPlugin( );

    DialogBox( Plugin->LoadedBase, MAKEINTRESOURCE( Plugin->Info.DialogId ), GetSafeHwnd( ), Plugin->SettingDlgFunction );
}

void CDialogPlugins::OnPopupMenuAbout( )
{  
    PRECLASS_PLUGIN Plugin = GetSelectedPlugin( );  

    //TODO: make cool about dialog maybe??

    CStringW Title( Plugin->Info.Name );
    Title.Append( L" - About" );
    ::MessageBoxW( GetSafeHwnd( ), Plugin->Info.About, Title, MB_OK | MB_ICONINFORMATION );
}

void CDialogPlugins::OnPopupMenuChangeState( )
{
    PRECLASS_PLUGIN Plugin = GetSelectedPlugin( );

    Plugin->State = !Plugin->State;
    Plugin->StateChangeFunction( Plugin->State );

    #ifdef _UNICODE
    g_ReClassApp.WriteProfileInt( L"PluginState", Plugin->Info.Name, Plugin->State ? 1 : 0 );
    #else
    g_ReClassApp.WriteProfileInt( "PluginState", CW2A( Plugin->Info.Name ), Plugin->State ? 1 : 0 );
    #endif

    RefreshPlugins( );
}

VOID CDialogPlugins::RefreshPlugins( )
{
    LVITEMW Item;

    m_PluginsList.DeleteAllItems( );

    for (PRECLASS_PLUGIN Plugin : g_LoadedPlugins)
    {
        ZeroMemory( &Item, sizeof( LVITEMW ) );
        Item.mask = LVIF_TEXT;
        Item.pszText = Plugin->Info.Name;
        Item.cchTextMax = (int)wcslen( Plugin->Info.Name ) + 1;
        Item.iItem = m_PluginsList.GetItemCount( );

        int PosIdx = m_PluginsList.InsertItem( (LVITEM*)&Item );
#ifdef _UNICODE
        m_PluginsList.SetItemText( PosIdx, 1, Plugin->Info.Version );
#else
        m_PluginsList.SetItemText( PosIdx, 1, CW2A( Plugin->Info.Version ) );
#endif
        m_PluginsList.SetItemText( PosIdx, 2, Plugin->State ? _T( "Enabled" ) : _T( "Disabled" ) );
    }
}
