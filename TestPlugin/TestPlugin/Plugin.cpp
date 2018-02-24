#include "Plugin.h"
#include "resource.h"

BOOL gTestPluginState = FALSE;


BOOL 
PLUGIN_CC 
PluginInit( 
    OUT LPRECLASS_PLUGIN_INFO lpRCInfo 
)
{
    wcscpy_s( lpRCInfo->Name, L"Test Plugin Name" );
    wcscpy_s( lpRCInfo->Version, L"1.0.0.2" );
    wcscpy_s( lpRCInfo->About, L"This plugin is a test plugin" );
    lpRCInfo->DialogId = IDD_SETTINGS_DLG;

    if (!ReClassIsReadMemoryOverriden( ) && !ReClassIsWriteMemoryOverriden( ))
    {
        if (ReClassOverrideMemoryOperations( ReadCallback, WriteCallback ) == FALSE)
        {
            ReClassPrintConsole( L"[TestPlugin] Failed to register read/write callbacks, failing PluginInit" );
            return FALSE;
        }
    }

    gTestPluginState = TRUE;

    return TRUE;
}

VOID 
PLUGIN_CC 
PluginStateChange( 
    IN BOOL State 
)
{
    //
    // Update global state variable
    //
    gTestPluginState = State;


    if (State)
    {
        ReClassPrintConsole( L"[TestPlugin] Enabled!" );

        //
        // Nothing for now.
        //
    }
    else
    {
        ReClassPrintConsole( L"[TestPlugin] Disabled!" );

        //
        // Remove our overrides if we're disabling/disabled.
        //
        if (ReClassGetCurrentReadMemory( ) == &ReadCallback)
            ReClassRemoveReadMemoryOverride( );

        if (ReClassGetCurrentWriteMemory( ) == &WriteCallback)
            ReClassRemoveWriteMemoryOverride( );
    }
}

INT_PTR 
PLUGIN_CC 
PluginSettingsDlg( 
    IN HWND hWnd, 
    IN UINT Msg, 
    IN WPARAM wParam, 
    IN LPARAM lParam 
)
{
    switch (Msg)
    {

    case WM_INITDIALOG:
    {
        if (gTestPluginState)
        {
            //
            // Apply checkboxes appropriately if we're in anm enabled state.
            //
            BOOL ReadChecked = (ReClassGetCurrentReadMemory( ) == &ReadCallback) ? BST_CHECKED : BST_UNCHECKED;
            BOOL WriteChecked = (ReClassGetCurrentWriteMemory( ) == &WriteCallback) ? BST_CHECKED : BST_UNCHECKED;

            SendMessage( GetDlgItem( hWnd, IDC_CHECK_READ_MEMORY_OVERRIDE ), BM_SETCHECK, MAKEWPARAM( ReadChecked, 0 ), 0 );
            EnableWindow( GetDlgItem( hWnd, IDC_CHECK_READ_MEMORY_OVERRIDE ), TRUE );

            SendMessage( GetDlgItem( hWnd, IDC_CHECK_WRITE_MEMORY_OVERRIDE ), BM_SETCHECK, MAKEWPARAM( WriteChecked, 0 ), 0 );
            EnableWindow( GetDlgItem( hWnd, IDC_CHECK_WRITE_MEMORY_OVERRIDE ), TRUE );
        }
        else
        {
            //
            // Make sure we can't touch the settings if we're in a disabled state.
            //
            SendMessage( GetDlgItem( hWnd, IDC_CHECK_READ_MEMORY_OVERRIDE ), BM_SETCHECK, MAKEWPARAM( BST_UNCHECKED, 0 ), 0 );
            EnableWindow( GetDlgItem( hWnd, IDC_CHECK_READ_MEMORY_OVERRIDE ), FALSE );

            SendMessage( GetDlgItem( hWnd, IDC_CHECK_WRITE_MEMORY_OVERRIDE ), BM_SETCHECK, MAKEWPARAM( BST_UNCHECKED, 0 ), 0 );
            EnableWindow( GetDlgItem( hWnd, IDC_CHECK_WRITE_MEMORY_OVERRIDE ), FALSE );
        }
    }
    return TRUE;

    case WM_COMMAND:
    {
        WORD NotificationCode = HIWORD( wParam );
        WORD ControlId = LOWORD( wParam );
        HWND hControlWnd = (HWND)lParam;
        
        if (NotificationCode == BN_CLICKED)
        {
            BOOLEAN bChecked = (SendMessage( hControlWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

            if (ControlId == IDC_CHECK_READ_MEMORY_OVERRIDE)
            {
                if (bChecked)
                {
                    //
                    // Make sure the read memory operation is not already overriden.
                    //
                    if (!ReClassIsReadMemoryOverriden( ))
                    {
                        ReClassOverrideReadMemoryOperation( &ReadCallback );
                    }
                    else
                    {
                        //
                        // Make sure it's not us!
                        //
                        if (ReClassGetCurrentReadMemory( ) != &ReadCallback)
                        {
                            //
                            // Ask the user whether or not they want to overwrite the other operation.
                            //
                            if (MessageBoxW( ReClassMainWindow( ),
                                L"Another plugin has already overriden the read operation.\n"
                                L"Would you like to overwrite their read override?",
                                L"Test Plugin", MB_YESNO ) == IDYES)
                            {
                                ReClassOverrideReadMemoryOperation( &ReadCallback );
                            }
                            else
                            {
                                //
                                // If the user chose no, then make sure our checkbox is unchecked.
                                //
                                SendMessage( GetDlgItem( hWnd, IDC_CHECK_READ_MEMORY_OVERRIDE ), 
                                    BM_SETCHECK, MAKEWPARAM( BST_UNCHECKED, 0 ), 0 );
                            }
                        }
                        else
                        {
                            //
                            // This shouldn't happen!
                            //
                            MessageBoxW( ReClassMainWindow( ), 
                                L"WTF! Plugin memory read operation is already set as the active override!", 
                                L"Test Plugin", MB_ICONERROR );
                        }
                    }
                }
                else
                {
                    //
                    // Only remove the read memory operation if it's ours!
                    //
                    if (ReClassGetCurrentReadMemory( ) == &ReadCallback)
                    {
                        ReClassRemoveReadMemoryOverride( );
                    }
                }			
            }
            else if (ControlId == IDC_CHECK_WRITE_MEMORY_OVERRIDE)
            {
                if (bChecked)
                {
                    //
                    // Make sure the write memory operation is not already overriden.
                    //
                    if (!ReClassIsWriteMemoryOverriden( ))
                    {
                        //
                        // We're all good to set our write memory operation!
                        //
                        ReClassOverrideWriteMemoryOperation( &WriteCallback );
                    }
                    else
                    {
                        //
                        // Make sure it's not us!
                        //
                        if (ReClassGetCurrentWriteMemory( ) != &WriteCallback)
                        {
                            //
                            // Ask the user whether or not they want to overwrite the other operation.
                            //
                            if (MessageBoxW( ReClassMainWindow( ),
                                L"Another plugin has already overriden the write operation.\n"
                                L"Would you like to overwrite their write override?",
                                L"Test Plugin", MB_YESNO ) == IDYES)
                            {
                                ReClassOverrideWriteMemoryOperation( &WriteCallback );
                            }
                            else
                            {
                                //
                                // If the user chose no, then make sure our checkbox is unchecked.
                                //
                                SendMessage( GetDlgItem( hWnd, IDC_CHECK_WRITE_MEMORY_OVERRIDE ),
                                    BM_SETCHECK, MAKEWPARAM( BST_UNCHECKED, 0 ), 0 );
                            }
                        }
                        else
                        {
                            //
                            // This shouldn't happen!
                            //
                            MessageBoxW( ReClassMainWindow( ),
                                L"WTF! Plugin memory write operation is already set as the active override!",
                                L"Test Plugin", MB_ICONERROR );
                        }
                    }
                }
                else
                {
                    //
                    // Only remove the read memory operation if it's ours!
                    //
                    if (ReClassGetCurrentWriteMemory( ) == &WriteCallback)
                    {
                        ReClassRemoveWriteMemoryOverride( );
                    }
                }
            }
        }	
    }
    break;

    case WM_CLOSE:
    {
        EndDialog( hWnd, 0 );
    }
    break;

    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
WriteCallback( 
    IN LPVOID Address, 
    IN LPVOID Buffer, 
    IN SIZE_T Size, 
    OUT PSIZE_T BytesWritten 
)
{
    DWORD OldProtect;
    HANDLE ProcessHandle = ReClassGetProcessHandle( );
    VirtualProtectEx( ProcessHandle, (PVOID)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect );
    BOOL Retval = WriteProcessMemory( ProcessHandle, (PVOID)Address, Buffer, Size, BytesWritten );
    VirtualProtectEx( ProcessHandle, (PVOID)Address, Size, OldProtect, NULL );
    return Retval;
}

BOOL 
PLUGIN_CC 
ReadCallback( 
    IN LPVOID Address,
    IN LPVOID Buffer,
    IN SIZE_T Size,
    OUT PSIZE_T BytesRead 
)
{
    HANDLE ProcessHandle = ReClassGetProcessHandle( );
    BOOL Retval = ReadProcessMemory( ProcessHandle, (LPVOID)Address, Buffer, Size, BytesRead );
    if (!Retval)
        ZeroMemory( Buffer, Size );
    return Retval;
}
