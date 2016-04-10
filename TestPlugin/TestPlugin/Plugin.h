#pragma once

#include "stdafx.h"

typedef struct _RECLASS_PLUGIN_INFO
{
	wchar_t Name[ 260 ];
	wchar_t About[ 2048 ];
	wchar_t Version[ 260 ];
} RECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

#ifdef PLUGIN_EXPORT
#define PLUGIN_API extern "C" __declspec(dllexport)
#else
#define PLUGIN_API
#endif

PLUGIN_API BOOL WINAPI PluginInit( LPRECLASS_PLUGIN_INFO lpRCInfo );
