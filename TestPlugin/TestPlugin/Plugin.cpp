#include "Plugin.h"
#include "resource.h"

BOOL PLUGIN_CC PluginInit( PRECLASS_PLUGIN_INFO lpRCInfo )
{
	wcscpy_s( lpRCInfo->Name, L"Test Plugin Name" );
	wcscpy_s( lpRCInfo->Version, L"1.0.0.2" );
	wcscpy_s( lpRCInfo->About, L"This plugin is a test plugin" );
	lpRCInfo->DialogID = IDD_SETTINGS_DLG;

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

VOID PLUGIN_CC PluginStateChange( BOOL state )
{
	ReClassPrintConsole( (state == FALSE) ? L"[TestPlugin] Disabled!" : L"[TestPlugin] Enabled!" );
}

INT_PTR CALLBACK PluginSettingsDlg( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

BOOL WINAPI WriteCallback( LPVOID write_address, LPVOID buffer_ptr, SIZE_T write_size, PSIZE_T num_write )
{
	DWORD OldProtect;
	HANDLE hProcess = ReClassGetProcessHandle( );
	VirtualProtectEx( hProcess, (PVOID)write_address, write_size, PAGE_EXECUTE_READWRITE, &OldProtect );
	BOOL ret = WriteProcessMemory( hProcess, (PVOID)write_address, buffer_ptr, write_size, num_write );
	VirtualProtectEx( hProcess, (PVOID)write_address, write_size, OldProtect, NULL );
	return ret;
}

BOOL WINAPI ReadCallback( LPVOID read_address, LPVOID buffer_ptr, SIZE_T read_size, PSIZE_T num_read )
{
	HANDLE hProcess = ReClassGetProcessHandle( );
	BOOL return_val = ReadProcessMemory( hProcess, (LPVOID)read_address, buffer_ptr, read_size, num_read );
	if (!return_val)
		ZeroMemory( buffer_ptr, read_size );
	return return_val;
}
