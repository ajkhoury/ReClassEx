#pragma once

//#include <afxribbonbar.h> //used for ribbon bar. comment this out if not used

#ifdef _WIN64
#pragma comment(lib, "ReClass64.lib")
#else
#pragma comment(lib, "ReClass.lib")
#endif

#define PLUGIN_CC __stdcall

typedef BOOL( PLUGIN_CC *MEMORY_OPERATION )( LPVOID, LPVOID, SIZE_T, PSIZE_T );
typedef HANDLE( PLUGIN_CC *HANDLE_OPERATION )( DWORD, BOOL, DWORD );

typedef struct _RECLASS_PLUGIN_INFO
{
#ifdef __cplusplus
	_RECLASS_PLUGIN_INFO( ) : DialogID( -1 ) { }
#endif
	wchar_t Name[ 260 ];
	wchar_t About[ 2048 ];
	wchar_t Version[ 260 ];
	int DialogID;
} RECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

BOOL PLUGIN_CC PluginInit( LPRECLASS_PLUGIN_INFO lpRCInfo );
void PLUGIN_CC PluginStateChange( bool state );
INT_PTR CALLBACK PluginSettingsDlg( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/*
*	Register overides for the read/write memory operations
*	returns false if a plugin has already registered it or one of the paramaters was null
*	returns true if succeeded or if force flag is set
*/
BOOL PLUGIN_CC ReClassOverrideMemoryOperations( MEMORY_OPERATION MemWrite, MEMORY_OPERATION MemRead, BOOL bForceSet = FALSE );

/*
 *	Register overides for the opening of handles for various process/thread operations
 *	returns false if a plugin has already registered it or one of the paramaters was null
 *	returns true if succeeded or if force flag is set
 */
BOOL PLUGIN_CC ReClassOverrideHandleOperations( HANDLE_OPERATION HandleProcess, HANDLE_OPERATION HandleThread, BOOL bForceSet = FALSE );

/*
*	Print text to the ReClass console window
*/
void PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... );

/*
 *	Get the current attached process handle, null if not attached
 */
LPHANDLE PLUGIN_CC ReClassGetProcessHandle( );

/*
 *	Return the main window handle for ReClass
 */
HWND PLUGIN_CC ReClassMainWindow( );

#ifdef _MFC_VER 
/*
 *	Get the ribon interface for MFC (usefull for adding custom buttons and such)
 */
CMFCRibbonBar* PLUGIN_CC ReClassRibbonInterface( );
#endif
