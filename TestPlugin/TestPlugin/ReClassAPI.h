#pragma once

//#include <afxribbonbar.h> //used for ribbon bar. comment this out if not used

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "ReClass64_dbg.lib")
#else
#pragma comment(lib, "ReClass64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "ReClass_dbg.lib")
#else
#pragma comment(lib, "ReClass.lib")
#endif
#endif

#define PLUGIN_CC __stdcall

typedef BOOL( PLUGIN_CC *tReadMemoryOperation )(IN LPVOID Address, IN LPVOID Buffer, IN SIZE_T Size, OUT PSIZE_T BytesRead);
typedef BOOL( PLUGIN_CC *tWriteMemoryOperation )(IN LPVOID Address, IN LPVOID Buffer, IN SIZE_T Size, OUT PSIZE_T BytesWritten);
typedef HANDLE( PLUGIN_CC *tOpenProcessOperation )(IN DWORD dwDesiredAccess, IN BOOL bInheritHandle, IN DWORD dwProcessID);
typedef HANDLE( PLUGIN_CC *tOpenThreadOperation )(IN DWORD dwDesiredAccess, IN BOOL bInheritHandle, IN DWORD dwThreadID);

//
// Plugin info structure to be filled in during initialization
// which is passed back to ReClass to display in the plugins dialog
//
typedef struct _RECLASS_PLUGIN_INFO
{
	wchar_t Name[256];		//< Name of the plugin
	wchar_t Version[256];	//< Plugin version
	wchar_t About[2048];	//< Small snippet about the plugin 
	int DialogID;			//< Identifier for the settings dialog
} RECLASS_PLUGIN_INFO, *PRECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

//
// Plugin initialization callback to fill in the RECLASS_PLUGIN_INFO struct,
// and initialize any other plugin resources
//
BOOL PLUGIN_CC PluginInit( PRECLASS_PLUGIN_INFO lpRCInfo );

//
// Callback for when the plugin state is changed (enabled or disabled). 
// Plugins disabled and enabled state are dependent on the implementation inside the plugin. 
// All we do is send a state change to plugins for them to disable or enable their functionality.
//
VOID PLUGIN_CC PluginStateChange( BOOL state );

//
// Window Proc for the settings dialog
//
INT_PTR CALLBACK PluginSettingsDlg( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

// 
// Register, remove, or check overrides for the read/write memory operations.
// 
BOOL PLUGIN_CC ReClassOverrideReadMemoryOperation( tReadMemoryOperation MemRead );
BOOL PLUGIN_CC ReClassOverrideWriteMemoryOperation( tWriteMemoryOperation MemWrite );
BOOL PLUGIN_CC ReClassOverrideMemoryOperations( tReadMemoryOperation MemRead, tWriteMemoryOperation MemWrite );
BOOL PLUGIN_CC ReClassRemoveReadMemoryOverride( );
BOOL PLUGIN_CC ReClassRemoveWriteMemoryOverride( );
BOOL PLUGIN_CC ReClassIsReadMemoryOverriden( );
BOOL PLUGIN_CC ReClassIsWriteMemoryOverriden( );

// 
// Register, remove, or check overrides for the opening of handles 
// for various process/thread operations.
// 
BOOL PLUGIN_CC ReClassOverrideOpenProcessOperation( tOpenProcessOperation ProcessOpen );
BOOL PLUGIN_CC ReClassOverrideOpenThreadOperation( tOpenThreadOperation ThreadOpen );
BOOL PLUGIN_CC ReClassOverrideHandleOperations( tOpenProcessOperation ProcessOpen, tOpenThreadOperation ThreadOpen );
BOOL PLUGIN_CC ReClassRemoveOpenProcessOverride( );
BOOL PLUGIN_CC ReClassRemoveOpenThreadOverride( );
BOOL PLUGIN_CC ReClassIsOpenProcessOverriden( );
BOOL PLUGIN_CC ReClassIsOpenThreadOverriden( );

// 
// Print text to the ReClass console window
// 
VOID PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... );

// 
// Gets the current attached process handle, null if not attached
// 
HANDLE PLUGIN_CC ReClassGetProcessHandle( );

// 
// Gets the current attached process ID, 0 if not attached
// 
DWORD PLUGIN_CC ReClassGetProcessId( );

// 
// Return the main window handle for ReClass
// 
HWND PLUGIN_CC ReClassMainWindow( );

#ifdef _MFC_VER 
// 
// Get the ribbon interface for MFC (useful for adding custom buttons and such)
// 
CMFCRibbonBar* PLUGIN_CC ReClassRibbonInterface( );
#endif
