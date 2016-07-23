#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef _WIN64
#pragma comment(lib, "ReClass64.lib")
#else
#pragma comment(lib, "ReClass.lib")
#endif

#ifdef PLUGIN_EXPORT
#ifdef __cplusplus
#define PLUGIN_API extern "C" __declspec(dllexport)
#define RECLASS_API __declspec(dllimport)
#else
#define PLUGIN_API __declspec(dllexport)
#define RECLASS_API __declspec(dllimport)
#endif
#else
#define PLUGIN_API
#define RECLASS_API
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

PLUGIN_API BOOL PLUGIN_CC PluginInit( OUT LPRECLASS_PLUGIN_INFO lpRCInfo );
PLUGIN_API INT_PTR CALLBACK PluginSettingsDlg( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/*
*	Register overides for the read/write memory operations
*	returns false if a plugin has already registered it or one of the paramaters was null
*	returns true if succeeded or if force flag is set
*/
RECLASS_API BOOL PLUGIN_CC ReClassOverrideMemoryOperations( MEMORY_OPERATION MemWrite, MEMORY_OPERATION MemRead, BOOL bForceSet = FALSE );

/*
 *	Register overides for the opening of handles for various process/thread operations
 *	returns false if a plugin has already registered it or one of the paramaters was null
 *	returns true if succeeded or if force flag is set
 */
RECLASS_API BOOL PLUGIN_CC ReClassOverrideHandleOperations( HANDLE_OPERATION HandleProcess, HANDLE_OPERATION HandleThread, BOOL bForceSet = FALSE );

/*
*	Print text to the ReClass console window
*/
RECLASS_API void PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... );

/*
 *	Get the current attached process handle, null if not attached
 */
RECLASS_API LPHANDLE PLUGIN_CC ReClassGetProcessHandle( );

/*
 *	Return the main window handle for ReClass
 */
RECLASS_API HWND PLUGIN_CC ReClassMainWindow( );
