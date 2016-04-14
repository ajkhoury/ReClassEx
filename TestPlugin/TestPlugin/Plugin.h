#pragma once

#include "stdafx.h"

//Plugin specific stuff
typedef BOOL( WINAPI *MEMORY_OPERATION )( LPVOID, LPVOID, SIZE_T, PSIZE_T );

typedef struct _RECLASS_PLUGIN_INFO
{
	wchar_t Name[ 260 ];
	wchar_t About[ 2048 ];
	wchar_t Version[ 260 ];
	HMODULE ModuleBase;
} RECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

#ifdef PLUGIN_EXPORT
#ifdef __cplusplus
#define PLUGIN_API extern "C" __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllexport)
#endif
#else
#define PLUGIN_API
#endif

#ifdef __cplusplus
#define GET_RECLASS_FUNCTION(modulebase, funcname) reinterpret_cast<decltype( &##funcname)>(GetProcAddress(modulebase, #funcname))
#else 
#define GET_RECLASS_FUNCTION(modulebase, funcname, prototype) (prototype)GetProcAddress(modulebase, #funcname)
#endif

#define PLUGIN_CC __stdcall

PLUGIN_API BOOL PLUGIN_CC PluginInit( HMODULE callee_base, LPRECLASS_PLUGIN_INFO lpRCInfo );

/*
*	Exported function that will register overides for the read/write memory operations
*  returns false if a plugin has already registered it or one of the paramaters was null
*  returns true if succeeded or if force flag is set
*/
BOOL PLUGIN_CC ReClassOverrideMemoryOperations( MEMORY_OPERATION write, MEMORY_OPERATION read, BOOL force );

//Start of plugin prototypes
