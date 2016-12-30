#include "stdafx.h"

#include "PluginAPI.h"


tReadMemoryOperation g_PluginOverrideReadMemory = nullptr;
tWriteMemoryOperation g_PluginOverrideWriteMemory = nullptr;
tOpenProcessOperation g_PluginOverrideOpenProcess = nullptr;
tOpenThreadOperation g_PluginOverrideOpenThread = nullptr;

std::vector<PRECLASS_PLUGIN> g_LoadedPlugins;

VOID LoadPlugins( )
{
	WIN32_FIND_DATA FileData = { 0 };
	#ifdef _WIN64
	#ifndef _DEBUG
	HANDLE hFileTree = FindFirstFile( _T( "plugins\\*.rc-plugin64" ), &FileData );
	#else
	HANDLE hFileTree = FindFirstFile( _T( "plugins\\*.rc-plugin64d" ), &FileData );
	#endif
	#else
	HANDLE hFileTree = FindFirstFile( _T( "plugins\\*.rc-plugin" ), &FileData );
	#endif
	if (hFileTree != INVALID_HANDLE_VALUE)
	{
		do
		{
			HMODULE PluginBase = LoadLibrary( CString( _T( "plugins\\" ) ) + FileData.cFileName );
			if (PluginBase == NULL)
			{
				PrintOut( _T( "plugin %s was not able to be loaded!" ), FileData.cFileName );
				continue;
			}

			auto pfnPluginInit = reinterpret_cast<tPluginInit>(GetProcAddress( PluginBase, "PluginInit" ));
			if (pfnPluginInit == nullptr)
			{
				PrintOut( _T( "%s is not a reclass plugin!" ), FileData.cFileName );
				FreeLibrary( PluginBase );
				continue;
			}

			auto pfnPluginStateChange = reinterpret_cast<tPluginStateChange>(GetProcAddress( PluginBase, "PluginStateChange" ));
			if (pfnPluginStateChange == nullptr)
			{
				PrintOut( _T( "%s doesnt have exported state change function! Unable to disable plugin on request, stop reclass and delete the plugin to disable it" ), FileData .cFileName );
			}

			auto pfnPluginSettingDlgProc = reinterpret_cast<DLGPROC>(GetProcAddress( PluginBase, "PluginSettingsDlg" ));

			PRECLASS_PLUGIN Plugin = new RECLASS_PLUGIN();
			wcscpy_s( Plugin->FileName, FileData.cFileName );
			Plugin->LoadedBase = PluginBase;
			Plugin->InitFnc = pfnPluginInit;
			Plugin->SettingDlgFnc = pfnPluginSettingDlgProc;
			Plugin->StateChangeFnc = pfnPluginStateChange;

			if (pfnPluginInit( &Plugin->Info ))
			{
				#ifdef UNICODE
				Plugin->State = g_ReClassApp.GetProfileInt( L"PluginState", Plugin->Info.Name, 1 ) == 1;
				#else
				Plugin->State = g_ReClassApp.GetProfileInt( "PluginState", CW2A( Plugin->Info.Name ), 1 ) == 1;
				#endif
				if (Plugin->Info.DialogID == -1)
					Plugin->SettingDlgFnc = nullptr;
				PrintOut( _T( "Loaded plugin %s (%ls version %ls) - %ls" ), FileData.cFileName, Plugin->Info.Name, Plugin->Info.Version, Plugin->Info.About );
				if (Plugin->StateChangeFnc != nullptr)
					Plugin->StateChangeFnc( Plugin->State );
				g_LoadedPlugins.push_back( Plugin );
			}
			else
			{
				PrintOut( _T( "Failed to load plugin %s" ), FileData.cFileName );
				FreeLibrary( PluginBase );
			}

		} while (FindNextFile( hFileTree, &FileData ));
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

tReadMemoryOperation PLUGIN_CC ReClassGetCurrentReadMemory( )
{
	return g_PluginOverrideReadMemory;
}

RECLASS_EXPORT tWriteMemoryOperation PLUGIN_CC ReClassGetCurrentWriteMemory( )
{
	return g_PluginOverrideWriteMemory;
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

tOpenProcessOperation PLUGIN_CC ReClassGetCurrentOpenProcess( )
{
	return g_PluginOverrideOpenProcess;
}

tOpenThreadOperation PLUGIN_CC ReClassGetCurrentOpenThread( )
{
	return g_PluginOverrideOpenThread;
}

VOID PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... )
{
	wchar_t buffer[2048];
	ZeroMemory( buffer, 2048 );
	
	va_list va;
	va_start( va, format );
	_vsnwprintf_s( buffer, 2048, format, va );
	va_end( va );

	#if defined(_UNICODE)
	g_ReClassApp.m_pConsole->PrintText( format );
	#else
	g_ReClassApp.m_pConsole->PrintText( CW2A( buffer ) );
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