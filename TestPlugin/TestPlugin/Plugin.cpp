#include "Plugin.h"

BOOL WINAPI PluginInit( HMODULE callee_base, LPRECLASS_PLUGIN_INFO lpRCInfo )
{
	wcscpy_s( lpRCInfo->Name, L"Test Plugin Name Here!" );
	wcscpy_s( lpRCInfo->Version, L"1.0.0.2" );
	wcscpy_s( lpRCInfo->About, L"This plugin is a test plugin for exports etc etc" );

	auto pfnPluginOverideMemoryOperation = GET_RECLASS_FUNCTION( callee_base, ReClassOverrideMemoryOperations );
	if ( pfnPluginOverideMemoryOperation == nullptr ) return FALSE;
	pfnPluginOverideMemoryOperation( nullptr, nullptr, FALSE );

	return TRUE;
}

