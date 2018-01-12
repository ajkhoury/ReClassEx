#ifndef _RECLASS_PLUGIN_API_H_
#define _RECLASS_PLUGIN_API_H_

#include <afxwin.h>
#include <afxribbonbar.h>
#include <vector>

//
// ReClass Plugin Loading and Unloading - Not part of the plugin API.
//
VOID 
LoadPlugins( 
    VOID 
    );

VOID 
UnloadPlugins( 
    VOID 
    );

//
// ReClass Plugin Calling Convention
//
#define PLUGIN_CC __stdcall

//
// Plugin Operation API Prototypes
//
typedef BOOL( PLUGIN_CC *PPLUGIN_READ_MEMORY_OPERATION )(
    IN LPVOID Address,
    IN LPVOID Buffer,
    IN SIZE_T Size,
    OUT PSIZE_T BytesRead
    );
typedef BOOL( PLUGIN_CC *PPLUGIN_WRITE_MEMORY_OPERATION )(
    IN LPVOID Address,
    IN LPVOID Buffer,
    IN SIZE_T Size,
    OUT PSIZE_T BytesWritten
    );
typedef HANDLE( PLUGIN_CC *PPLUGIN_OPEN_PROCESS_OPERATION )(
    IN DWORD dwDesiredAccess,
    IN BOOL bInheritHandle,
    IN DWORD dwProcessId
    );
typedef HANDLE( PLUGIN_CC *PPLUGIN_OPEN_THREAD_OPERATION )(
    IN DWORD dwDesiredAccess,
    IN BOOL bInheritHandle,
    IN DWORD dwThreadId
    );

//
// Basic ReClass Pugin Information Struct.
//
typedef DECLSPEC_ALIGN(16) struct _RECLASS_PLUGIN_INFO {
	wchar_t Name[256];
	wchar_t Version[256];
	wchar_t About[2048];
	int DialogId;
} RECLASS_PLUGIN_INFO, *PRECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

//
// Mandatory Plugin Exports Prototypes
//
// These must be exported by the plugin module!
// 
// Example:
// 
//      BOOL
//      PLUGIN_CC
//      PluginInit(
//          OUT LPRECLASS_PLUGIN_INFO lpRCInfo
//          );
//      
//      VOID
//      PLUGIN_CC
//      PluginStateChange(
//          IN BOOL State
//          );
//
typedef BOOL( PLUGIN_CC *PPLUGIN_INIT )(
    OUT LPRECLASS_PLUGIN_INFO lpRCInfo
    );
typedef VOID( PLUGIN_CC *PPLUGIN_STATE_CHANGE )(
    IN BOOL State
    );

//
// Defines a ReClass Plugin.
//
typedef DECLSPEC_ALIGN(16) struct _RECLASS_PLUGIN {
	RECLASS_PLUGIN_INFO Info;
	wchar_t FileName[MAX_PATH];
	BOOL State;
	HMODULE LoadedBase;
	PPLUGIN_INIT InitFunction;
	PPLUGIN_STATE_CHANGE StateChangeFunction;
	DLGPROC SettingDlgFunction;
} RECLASS_PLUGIN, *PRECLASS_PLUGIN, *LPRECLASS_PLUGIN;


//
// Exported Plugin API Below
//
#define RECLASS_EXPORT __declspec(dllexport) 

//
// Plugin Memory Operation API
//
RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassOverrideReadMemoryOperation( 
    IN PPLUGIN_READ_MEMORY_OPERATION ReadMemoryOperation 
    );

RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassOverrideWriteMemoryOperation( 
    IN PPLUGIN_WRITE_MEMORY_OPERATION WriteMemoryOperation 
    );

RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassOverrideMemoryOperations( 
    IN PPLUGIN_READ_MEMORY_OPERATION ReadMemoryOperation, 
    IN PPLUGIN_WRITE_MEMORY_OPERATION WriteMemoryOperation 
    );


RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassRemoveReadMemoryOverride( 
    VOID 
    );

RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassRemoveWriteMemoryOverride( 
    VOID 
    );


RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassIsReadMemoryOverriden( 
    VOID 
    );

RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassIsWriteMemoryOverriden( 
    VOID 
    );


RECLASS_EXPORT
PPLUGIN_READ_MEMORY_OPERATION 
PLUGIN_CC 
ReClassGetCurrentReadMemory( 
    VOID 
    );

RECLASS_EXPORT
PPLUGIN_WRITE_MEMORY_OPERATION 
PLUGIN_CC 
ReClassGetCurrentWriteMemory( 
    VOID 
    );


//
// Plugin Handle Operation API
//
RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassOverrideOpenProcessOperation( 
    IN PPLUGIN_OPEN_PROCESS_OPERATION OpenProcessOperation 
    );

RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassOverrideOpenThreadOperation( 
    IN PPLUGIN_OPEN_THREAD_OPERATION OpenThreadOperation 
    );

RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassOverrideHandleOperations( 
    IN PPLUGIN_OPEN_PROCESS_OPERATION OpenProcessOperation, 
    IN PPLUGIN_OPEN_THREAD_OPERATION OpenThreadOperation 
    );


RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassRemoveOpenProcessOverride( 
    VOID 
    );

RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassRemoveOpenThreadOverride( 
    VOID 
    );


RECLASS_EXPORT
BOOL 
PLUGIN_CC 
ReClassIsOpenProcessOverriden( 
    VOID 
    );

RECLASS_EXPORT 
BOOL 
PLUGIN_CC 
ReClassIsOpenThreadOverriden( 
    VOID 
    );


RECLASS_EXPORT 
PPLUGIN_OPEN_PROCESS_OPERATION 
PLUGIN_CC 
ReClassGetCurrentOpenProcess( 
    VOID 
    );

RECLASS_EXPORT 
PPLUGIN_OPEN_THREAD_OPERATION 
PLUGIN_CC 
ReClassGetCurrentOpenThread( 
    VOID 
    );


//
// Plugin Logging API
//
RECLASS_EXPORT 
VOID 
CDECL // __stdcall can't handle variadic functions 
ReClassPrintConsole( 
    IN const wchar_t* Format,
    ...
    );


//
// Plugin Process Info API
//
RECLASS_EXPORT 
HANDLE 
PLUGIN_CC 
ReClassGetProcessHandle( 
    VOID 
    );

RECLASS_EXPORT 
DWORD 
PLUGIN_CC 
ReClassGetProcessId( 
    VOID 
    );


//
// Plugin UI API
//
RECLASS_EXPORT 
HWND 
PLUGIN_CC 
ReClassMainWindow( 
    VOID 
    );

RECLASS_EXPORT 
CMFCRibbonBar* 
PLUGIN_CC 
ReClassRibbonInterface( 
    VOID 
    );

extern PPLUGIN_READ_MEMORY_OPERATION g_PluginOverrideReadMemoryOperation;
extern PPLUGIN_WRITE_MEMORY_OPERATION g_PluginOverrideWriteMemoryOperation;
extern PPLUGIN_OPEN_PROCESS_OPERATION g_PluginOverrideOpenProcessOperation;
extern PPLUGIN_OPEN_THREAD_OPERATION g_PluginOverrideOpenThreadOperation;

extern std::vector<PRECLASS_PLUGIN> g_LoadedPlugins;


#endif // _RECLASS_PLUGIN_API_H_
