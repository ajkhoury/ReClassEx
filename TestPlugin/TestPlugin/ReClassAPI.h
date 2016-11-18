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

typedef struct _RECLASS_PLUGIN_INFO
{
	wchar_t Name[256];
	wchar_t Version[256];
	wchar_t About[2048];
	int DialogID;
} RECLASS_PLUGIN_INFO, *PRECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

//
// Plugin initialization callback to fill in the RECLASS_PLUGIN_INFO struct,
// and initialize any other plugin resources
//
BOOL PLUGIN_CC PluginInit( PRECLASS_PLUGIN_INFO lpRCInfo );

//
// Callback for when the plugin state is changed (enabled or disabled)
//
VOID PLUGIN_CC PluginStateChange( BOOL state );

//
// Window Proc for the settings dialog
//
INT_PTR CALLBACK PluginSettingsDlg( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

// 
// Register overides for the read/write memory operations
// returns false if a plugin has already registered it or one of the paramaters was null
// returns true if succeeded or if force flag is set
// 
BOOL PLUGIN_CC ReClassOverrideReadMemoryOperation( tReadMemoryOperation MemRead );
BOOL PLUGIN_CC ReClassOverrideWriteMemoryOperation( tWriteMemoryOperation MemWrite );
BOOL PLUGIN_CC ReClassOverrideMemoryOperations( tReadMemoryOperation MemRead, tWriteMemoryOperation MemWrite );
BOOL PLUGIN_CC ReClassRemoveReadMemoryOverride( );
BOOL PLUGIN_CC ReClassRemoveWriteMemoryOverride( );
BOOL PLUGIN_CC ReClassIsReadMemoryOverriden( );
BOOL PLUGIN_CC ReClassIsWriteMemoryOverriden( );

// 
// Register overides for the opening of handles for various process/thread operations
// returns false if a plugin has already registered it or one of the paramaters was null
// returns true if succeeded or if force flag is set
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
// Get the current attached process handle, null if not attached
// 
HANDLE PLUGIN_CC ReClassGetProcessHandle( );

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
