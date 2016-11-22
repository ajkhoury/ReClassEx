#include "stdafx.h"

#include "PluginAPI.h"

tReadMemoryOperation g_PluginOverrideReadMemory = nullptr;
tWriteMemoryOperation g_PluginOverrideWriteMemory = nullptr;
tOpenProcessOperation g_PluginOverrideOpenProcess = nullptr;
tOpenThreadOperation g_PluginOverrideOpenThread = nullptr;

std::vector<PRECLASS_PLUGIN> g_LoadedPlugins;

VOID LoadPlugins( )
{
	WIN32_FIND_DATA file_data = { 0 };
	#ifdef _WIN64
	HANDLE findfile_tree = FindFirstFile( _T( "plugins\\*.rc-plugin64" ), &file_data );
	#else
	HANDLE findfile_tree = FindFirstFile( _T( "plugins\\*.rc-plugin" ), &file_data );
	#endif
	if (findfile_tree != INVALID_HANDLE_VALUE)
	{
		CString message;

		do
		{
			HMODULE plugin_base = LoadLibrary( CString( _T( "plugins\\" ) ) + file_data.cFileName );
			if (plugin_base == NULL)
			{
				message.Format( _T( "plugin %s was not able to be loaded!" ), file_data.cFileName );
				PrintOut( message );
				continue;
			}

			auto pfnPluginInit = reinterpret_cast<tPluginInit>(GetProcAddress( plugin_base, "PluginInit" ));
			if (pfnPluginInit == nullptr)
			{
				message.Format( _T( "%s is not a reclass plugin!" ), file_data.cFileName );
				PrintOut( message );
				FreeLibrary( plugin_base );
				continue;
			}

			auto pfnPluginStateChange = reinterpret_cast<tPluginStateChange>(GetProcAddress( plugin_base, "PluginStateChange" ));
			if (pfnPluginStateChange == nullptr)
			{
				message.Format( _T( "%s doesnt have exported state change function! Unable to disable plugin on request, stop reclass and delete the plugin to disable it" ), file_data.cFileName );
				PrintOut( message );
			}

			auto pfnPluginSettingDlgProc = reinterpret_cast<DLGPROC>(GetProcAddress( plugin_base, "PluginSettingsDlg" ));

			PRECLASS_PLUGIN plugin = new RECLASS_PLUGIN;
			ZeroMemory( plugin, sizeof( RECLASS_PLUGIN ) );
			wcscpy_s( plugin->FileName, file_data.cFileName );
			plugin->LoadedBase = plugin_base;
			plugin->InitFnc = pfnPluginInit;
			plugin->SettingDlgFnc = pfnPluginSettingDlgProc;
			plugin->StateChangeFnc = pfnPluginStateChange;

			if (pfnPluginInit( &plugin->Info ))
			{
				#ifdef UNICODE
				plugin->State = g_ReClassApp.GetProfileInt( L"PluginState", plugin->Info.Name, 1 ) == 1;
				#else
				plugin->State = g_ReClassApp.GetProfileInt( "PluginState", CW2A( plugin->Info.Name ), 1 ) == 1;
				#endif
				if (plugin->Info.DialogID == -1)
					plugin->SettingDlgFnc = nullptr;
				PrintOut( _T( "Loaded plugin %s (%ls version %ls) - %ls" ), file_data.cFileName, plugin->Info.Name, plugin->Info.Version, plugin->Info.About );
				if (plugin->StateChangeFnc != nullptr)
					plugin->StateChangeFnc( plugin->State );
				g_LoadedPlugins.push_back( plugin );
			}
			else
			{
				message.Format( _T( "Failed to load plugin %s" ), file_data.cFileName );
				PrintOut( message );
				FreeLibrary( plugin_base );
			}

		} while (FindNextFile( findfile_tree, &file_data ));
	}
}

VOID UnloadPlugins( )
{
	for (PRECLASS_PLUGIN plugin : g_LoadedPlugins)
	{
		FreeLibrary( plugin->LoadedBase );
		delete plugin;
	}
	g_LoadedPlugins.clear( );
}

BOOL PLUGIN_CC ReClassOverrideReadMemoryOperation( tReadMemoryOperation MemRead )
{
	if (MemRead != nullptr)
	{
		g_PluginOverrideReadMemory = MemRead;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassOverrideWriteMemoryOperation( tWriteMemoryOperation MemWrite )
{
	if (MemWrite != nullptr)
	{
		g_PluginOverrideWriteMemory = MemWrite;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassOverrideMemoryOperations( tReadMemoryOperation MemRead, tWriteMemoryOperation MemWrite )
{
	if (MemRead == nullptr && MemWrite == nullptr)
		return FALSE;
	if (MemRead != nullptr)
		g_PluginOverrideReadMemory = MemRead;
	if (MemWrite != nullptr)
		g_PluginOverrideWriteMemory = MemWrite;
	return TRUE;
}

BOOL PLUGIN_CC ReClassRemoveReadMemoryOverride( )
{
	if (g_PluginOverrideReadMemory != nullptr)
	{
		g_PluginOverrideReadMemory = nullptr;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassRemoveWriteMemoryOverride( )
{
	if (g_PluginOverrideWriteMemory != nullptr)
	{
		g_PluginOverrideWriteMemory = nullptr;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassIsReadMemoryOverriden( )
{
	return (g_PluginOverrideReadMemory != nullptr) ? TRUE : FALSE;
}

BOOL PLUGIN_CC ReClassIsWriteMemoryOverriden( )
{
	return (g_PluginOverrideWriteMemory != nullptr) ? TRUE : FALSE;
}

BOOL PLUGIN_CC ReClassOverrideOpenProcessOperation( tOpenProcessOperation ProcessOpen )
{
	if (ProcessOpen != nullptr)
	{
		g_PluginOverrideOpenProcess = ProcessOpen;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassOverrideOpenThreadOperation( tOpenThreadOperation ThreadOpen )
{
	if (ThreadOpen != nullptr)
	{
		g_PluginOverrideOpenThread = ThreadOpen;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassOverrideHandleOperations( tOpenProcessOperation ProcessOpen, tOpenThreadOperation ThreadOpen )
{
	if (ProcessOpen == nullptr && ThreadOpen == nullptr)
		return FALSE;
	if (ProcessOpen != nullptr)
		g_PluginOverrideOpenProcess = ProcessOpen;
	if (ThreadOpen != nullptr)
		g_PluginOverrideOpenThread = ThreadOpen;
	return TRUE;
}

BOOL PLUGIN_CC ReClassRemoveOpenProcessOverride( )
{
	if (g_PluginOverrideOpenProcess != nullptr)
	{
		g_PluginOverrideOpenProcess = nullptr;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassRemoveOpenThreadOverride( )
{
	if (g_PluginOverrideOpenThread != nullptr)
	{
		g_PluginOverrideOpenThread = nullptr;
		return TRUE;
	}
	return FALSE;
}

BOOL PLUGIN_CC ReClassIsOpenProcessOverriden( )
{
	return (g_PluginOverrideOpenProcess != nullptr) ? TRUE : FALSE;
}

BOOL PLUGIN_CC ReClassIsOpenThreadOverriden( )
{
	return (g_PluginOverrideOpenThread != nullptr) ? TRUE : FALSE;
}

VOID PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... )
{
	wchar_t buffer[6048];
	ZeroMemory( &buffer, sizeof( buffer ) );

	va_list va;
	va_start( va, format );
	vswprintf_s( buffer, format, va );
	va_end( va );

	#if defined(_UNICODE)
	g_ReClassApp.Console->PrintText( buffer );
	#else
	g_ReClassApp.Console->PrintText( CW2A( buffer ) );
	#endif
}

HANDLE PLUGIN_CC ReClassGetProcessHandle( )
{
	return g_hProcess;
}

DWORD PLUGIN_CC ReClassGetProcessId( )
{
	return g_ProcessID;
}

HWND PLUGIN_CC ReClassMainWindow( )
{
	return *g_ReClassApp.GetMainWnd( );
}

CMFCRibbonBar* PLUGIN_CC ReClassRibbonInterface( )
{
	return g_ReClassApp.GetRibbonBar( );
}