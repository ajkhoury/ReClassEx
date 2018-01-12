#include "Plugin.h"
#include "resource.h"

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

	return TRUE;
}

VOID 
PLUGIN_CC 
PluginStateChange( 
    IN BOOL State 
)
{
	ReClassPrintConsole( L"[TestPlugin] %s!", (State == FALSE) ? L"Disabled" : L"Enabled" );
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
		SendMessage( 
			GetDlgItem( hWnd, IDC_CHECK_READ_MEMORY_OVERRIDE ), 
			BM_SETCHECK, 
			MAKEWPARAM( ReClassIsReadMemoryOverriden( ) ? BST_CHECKED : BST_UNCHECKED, 0 ), 
			0 
		);
		SendMessage( 
			GetDlgItem( hWnd, IDC_CHECK_WRITE_MEMORY_OVERRIDE ), 
			BM_SETCHECK, 
			MAKEWPARAM( ReClassIsWriteMemoryOverriden( ) ? BST_CHECKED : BST_UNCHECKED, 0 ), 
			0 
		);
	}
	return TRUE;

	case WM_COMMAND:
	{
		WORD NotificationCode = HIWORD( wParam );
		WORD ControlId = LOWORD( wParam );
		HWND hControlWnd = reinterpret_cast<HWND>(lParam);
		
		if (NotificationCode == BN_CLICKED)
		{
			BOOLEAN bChecked = (SendMessage( hControlWnd, BM_GETCHECK, 0, 0 ) == BST_CHECKED);

			if (ControlId == IDC_CHECK_READ_MEMORY_OVERRIDE)
			{
				if (bChecked)
					ReClassOverrideReadMemoryOperation( ReadCallback );
				else
					ReClassRemoveReadMemoryOverride( );
			}
			else if (ControlId == IDC_CHECK_WRITE_MEMORY_OVERRIDE)
			{
				if (bChecked)
					ReClassOverrideWriteMemoryOperation( WriteCallback );
				else
					ReClassRemoveWriteMemoryOverride( );
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
