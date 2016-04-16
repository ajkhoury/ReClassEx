#include "Plugin.h"

BOOL WINAPI PluginInit( LPRECLASS_PLUGIN_INFO lpRCInfo )
{
	wcscpy_s( lpRCInfo->Name, L"Test Plugin Name Here!" );
	wcscpy_s( lpRCInfo->Version, L"1.0.0.2" );
	wcscpy_s( lpRCInfo->About, L"This plugin is a test plugin for exports etc etc" );

	if ( !ReClassOverrideMemoryOperations( WriteCallback, ReadCallback ) ) {
		ReClassPrintConsole( L"[TestPlugin] Failed to register read/write callbacks, failing PluginInit" );
		return FALSE;
	}	

	return TRUE;
}

BOOL WINAPI WriteCallback( LPVOID write_address, LPVOID buffer_ptr, SIZE_T write_size, PSIZE_T num_write )
{
	DWORD OldProtect;
	VirtualProtectEx( ReClassGetProcessHandle( ), (void*) write_address, write_size, PAGE_EXECUTE_READWRITE, &OldProtect );
	BOOL ret = WriteProcessMemory( ReClassGetProcessHandle( ), (void*) write_address, buffer_ptr, write_size, num_write );
	VirtualProtectEx( ReClassGetProcessHandle( ), (void*) write_address, write_size, OldProtect, NULL );
	return ret;
}

BOOL WINAPI ReadCallback( LPVOID read_address, LPVOID buffer_ptr, SIZE_T read_size, PSIZE_T num_read )
{
	BOOL return_val = ReadProcessMemory( ReClassGetProcessHandle( ), (LPVOID) read_address, buffer_ptr, read_size, num_read );
	if ( !return_val ) ZeroMemory( buffer_ptr, read_size );
	return return_val;
}
