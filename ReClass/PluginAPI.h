#pragma once

#include <afxwin.h>
#include <afxribbonbar.h>
#include <vector>

VOID LoadPlugins( );
VOID UnloadPlugins( );

#define RECLASS_EXPORT __declspec(dllexport) 
#define PLUGIN_CC __stdcall

typedef BOOL( PLUGIN_CC *tReadMemoryOperation )(IN LPVOID Address, IN LPVOID Buffer, IN SIZE_T Size, OUT PSIZE_T BytesRead);
typedef BOOL( PLUGIN_CC *tWriteMemoryOperation )(IN LPVOID Address, IN LPVOID Buffer, IN SIZE_T Size, OUT PSIZE_T BytesWritten);
typedef HANDLE( PLUGIN_CC *tOpenProcessOperation )(IN DWORD dwDesiredAccess, IN BOOL bInheritHandle, IN DWORD dwProcessID);
typedef HANDLE( PLUGIN_CC *tOpenThreadOperation )(IN DWORD dwDesiredAccess, IN BOOL bInheritHandle, IN DWORD dwThreadID);

typedef DECLSPEC_ALIGN( 16 ) struct _RECLASS_PLUGIN_INFO
{
	wchar_t Name[256];
	wchar_t Version[256];
	wchar_t About[2048];
	int DialogID;
} RECLASS_PLUGIN_INFO, *PRECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

BOOL PLUGIN_CC PluginInit( PRECLASS_PLUGIN_INFO lpRCInfo );
VOID PLUGIN_CC PluginStateChange( BOOL state );
typedef BOOL( PLUGIN_CC *tPluginInit )(PRECLASS_PLUGIN_INFO lpRCInfo);
typedef VOID( PLUGIN_CC *tPluginStateChange )(BOOL state);

typedef DECLSPEC_ALIGN( 16 ) struct _RECLASS_PLUGIN
{
	RECLASS_PLUGIN_INFO Info;
	wchar_t FileName[MAX_PATH];
	BOOL State;
	HMODULE LoadedBase;
	tPluginInit InitFnc;
	tPluginStateChange StateChangeFnc;
	DLGPROC SettingDlgFnc;
} RECLASS_PLUGIN, *PRECLASS_PLUGIN, *LPRECLASS_PLUGIN;

// Exported Functions Below
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideReadMemoryOperation( tReadMemoryOperation MemRead );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideWriteMemoryOperation( tWriteMemoryOperation MemWrite );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideMemoryOperations( tReadMemoryOperation MemRead, tWriteMemoryOperation MemWrite );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassRemoveReadMemoryOverride( );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassRemoveWriteMemoryOverride( );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassIsReadMemoryOverriden( );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassIsWriteMemoryOverriden( );
RECLASS_EXPORT tReadMemoryOperation PLUGIN_CC ReClassGetCurrentReadMemory( );
RECLASS_EXPORT tWriteMemoryOperation PLUGIN_CC ReClassGetCurrentWriteMemory( );

RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideOpenProcessOperation( tOpenProcessOperation ProcessOpen );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideOpenThreadOperation( tOpenThreadOperation ThreadOpen );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideHandleOperations( tOpenProcessOperation ProcessOpen, tOpenThreadOperation ThreadOpen );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassRemoveOpenProcessOverride( );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassRemoveOpenThreadOverride( );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassIsOpenProcessOverriden( );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassIsOpenThreadOverriden( );
RECLASS_EXPORT tOpenProcessOperation PLUGIN_CC ReClassGetCurrentOpenProcess( );
RECLASS_EXPORT tOpenThreadOperation PLUGIN_CC ReClassGetCurrentOpenThread( );

RECLASS_EXPORT VOID PLUGIN_CC ReClassPrintConsole( const WCHAR* format, ... );
RECLASS_EXPORT HANDLE PLUGIN_CC ReClassGetProcessHandle( );
RECLASS_EXPORT DWORD PLUGIN_CC ReClassGetProcessId( );
RECLASS_EXPORT HWND PLUGIN_CC ReClassMainWindow( );
RECLASS_EXPORT CMFCRibbonBar* PLUGIN_CC ReClassRibbonInterface( );

extern tReadMemoryOperation g_PluginOverrideReadMemory;
extern tWriteMemoryOperation g_PluginOverrideWriteMemory;
extern tOpenProcessOperation g_PluginOverrideOpenProcess;
extern tOpenThreadOperation g_PluginOverrideOpenThread;

extern std::vector<PRECLASS_PLUGIN> g_LoadedPlugins;

#pragma endregion