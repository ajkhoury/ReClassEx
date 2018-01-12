#ifndef _RECLASS_API_H_
#define _RECLASS_API_H_

//#include <afxribbonbar.h> // Used for ribbon bar. comment this out if not used

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "ReClassEx64_dbg.lib")
#else
#pragma comment(lib, "ReClassEx64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "ReClassEx_dbg.lib")
#else
#pragma comment(lib, "ReClassEx.lib")
#endif
#endif

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
// Plugin info structure to be filled in during initialization
// which is passed back to ReClass to display in the plugins dialog
//
typedef DECLSPEC_ALIGN(16) struct _RECLASS_PLUGIN_INFO {
    wchar_t Name[256];		//< Name of the plugin
    wchar_t Version[256];	//< Plugin version
    wchar_t About[2048];	//< Small snippet about the plugin 
    int DialogId;			//< Identifier for the settings dialog
} RECLASS_PLUGIN_INFO, *PRECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

//
// Plugin initialization callback to fill in the RECLASS_PLUGIN_INFO struct,
// and initialize any other plugin resources
//
BOOL 
PLUGIN_CC 
PluginInit( 
    OUT LPRECLASS_PLUGIN_INFO lpRCInfo
    );

//
// Callback for when the plugin state is changed (enabled or disabled). 
// Plugins disabled and enabled state are dependent on the implementation inside the plugin. 
// All we do is send a state change to plugins for them to disable or enable their functionality.
//
VOID 
PLUGIN_CC 
PluginStateChange( 
    IN BOOL state 
    );

//
// Window Proc for the settings dialog
//
INT_PTR 
PLUGIN_CC 
PluginSettingsDlg( 
    IN HWND hWnd, 
    IN UINT Msg, 
    IN WPARAM wParam, 
    IN LPARAM lParam 
    );

// 
// Register, remove, get or check overrides for the read/write memory operations.
// 
BOOL 
PLUGIN_CC 
ReClassOverrideReadMemoryOperation( 
    IN PPLUGIN_READ_MEMORY_OPERATION ReadMemoryOperation 
    );

BOOL 
PLUGIN_CC 
ReClassOverrideWriteMemoryOperation( 
    IN PPLUGIN_WRITE_MEMORY_OPERATION WriteMemoryOperation 
    );

BOOL 
PLUGIN_CC 
ReClassOverrideMemoryOperations( 
    IN PPLUGIN_READ_MEMORY_OPERATION ReadMemoryOperation, 
    IN PPLUGIN_WRITE_MEMORY_OPERATION WriteMemoryOperation 
    );


BOOL 
PLUGIN_CC 
ReClassRemoveReadMemoryOverride( 
    VOID 
    );

BOOL 
PLUGIN_CC 
ReClassRemoveWriteMemoryOverride( 
    VOID 
    );


BOOL 
PLUGIN_CC 
ReClassIsReadMemoryOverriden( 
    VOID 
    );

BOOL 
PLUGIN_CC 
ReClassIsWriteMemoryOverriden( 
    VOID 
    );


PPLUGIN_READ_MEMORY_OPERATION 
PLUGIN_CC 
ReClassGetCurrentReadMemory( 
    VOID 
    );

PPLUGIN_WRITE_MEMORY_OPERATION 
PLUGIN_CC 
ReClassGetCurrentWriteMemory( 
    VOID 
    );

// 
// Register, remove, get or check overrides for the opening of handles 
// for various process/thread operations.
// 
BOOL
PLUGIN_CC
ReClassOverrideOpenProcessOperation( 
    IN PPLUGIN_OPEN_PROCESS_OPERATION OpenProcessOperation 
    );

BOOL
PLUGIN_CC
ReClassOverrideOpenThreadOperation( 
    IN PPLUGIN_OPEN_THREAD_OPERATION OpenThreadOperation 
    );

BOOL
PLUGIN_CC
ReClassOverrideHandleOperations( 
    IN PPLUGIN_OPEN_PROCESS_OPERATION OpenProcessOperation, 
    IN PPLUGIN_OPEN_THREAD_OPERATION OpenThreadOperation 
    );


BOOL
PLUGIN_CC
ReClassRemoveOpenProcessOverride( 
    VOID 
    );

BOOL
PLUGIN_CC
ReClassRemoveOpenThreadOverride( 
    VOID 
    );


BOOL 
PLUGIN_CC 
ReClassIsOpenProcessOverriden( 
    VOID 
    );

BOOL 
PLUGIN_CC 
ReClassIsOpenThreadOverriden( 
    VOID 
    );


PPLUGIN_OPEN_PROCESS_OPERATION 
PLUGIN_CC 
ReClassGetCurrentOpenProcess( 
    VOID 
    );

PPLUGIN_OPEN_THREAD_OPERATION 
PLUGIN_CC 
ReClassGetCurrentOpenThread( 
    VOID 
    );

// 
// Print text to the ReClass console window
// 
VOID 
PLUGIN_CC 
ReClassPrintConsole( 
    IN const wchar_t *Format, 
    ... 
    );

// 
// Gets the current attached process handle, null if not attached
// 
HANDLE 
PLUGIN_CC 
ReClassGetProcessHandle( 
    VOID 
    );

// 
// Gets the current attached process ID, 0 if not attached
// 
DWORD 
PLUGIN_CC 
ReClassGetProcessId( 
    VOID 
    );

// 
// Return the main window handle for ReClass
// 
HWND 
PLUGIN_CC 
ReClassMainWindow( 
    VOID 
    );

#ifdef _MFC_VER 
// 
// Get the ribbon interface for MFC (useful for adding custom buttons and such)
// 
CMFCRibbonBar* 
PLUGIN_CC 
ReClassRibbonInterface( 
    VOID 
    );
#endif


//
// Class Manipulation
//
// TODO: Implement this.
// 



#endif // _RECLASS_API_H_