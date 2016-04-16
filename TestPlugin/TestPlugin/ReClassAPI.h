#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef _WIN64
#pragma comment(lib, "ReClass64.lib")
#else
#pragma comment(lib, "ReClass.lib")
#endif

typedef BOOL( WINAPI *MEMORY_OPERATION )( LPVOID, LPVOID, SIZE_T, PSIZE_T );

typedef struct _RECLASS_PLUGIN_INFO
{
	wchar_t Name[ 260 ];
	wchar_t About[ 2048 ];
	wchar_t Version[ 260 ];
} RECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

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

PLUGIN_API BOOL PLUGIN_CC PluginInit( OUT LPRECLASS_PLUGIN_INFO lpRCInfo );

/*
*	Register overides for the read/write memory operations
*	returns false if a plugin has already registered it or one of the paramaters was null
*	returns true if succeeded or if force flag is set
*/
RECLASS_API BOOL PLUGIN_CC ReClassOverrideMemoryOperations( MEMORY_OPERATION write, MEMORY_OPERATION read, BOOL force = FALSE );

/*
*	Print text to the ReClass console window
*/
RECLASS_API void PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... );

/*
 *	Get thecurrent attached process handle, null if not attached
 */
RECLASS_API const HANDLE ReClassGetProcessHandle( );