#include "stdafx.h"

#include "PluginAPI.h"


PPLUGIN_READ_MEMORY_OPERATION g_PluginOverrideReadMemoryOperation = nullptr;
PPLUGIN_WRITE_MEMORY_OPERATION g_PluginOverrideWriteMemoryOperation = nullptr;
PPLUGIN_OPEN_PROCESS_OPERATION g_PluginOverrideOpenProcessOperation = nullptr;
PPLUGIN_OPEN_THREAD_OPERATION g_PluginOverrideOpenThreadOperation = nullptr;

std::vector<PRECLASS_PLUGIN> g_LoadedPlugins;

VOID 
LoadPlugins( 
    VOID 
)
{
    PRECLASS_PLUGIN Plugin;
    HMODULE PluginBase;

    HANDLE FileTreeHandle;
    WIN32_FIND_DATA FileData;

    PPLUGIN_INIT PluginInitFunction;
    PPLUGIN_STATE_CHANGE PluginStateChangeFunction;
    DLGPROC PluginSettingDlgFunction;

    //TCHAR ModulePath[MAX_PATH];
    //GetModuleFileName( GetModuleHandle( NULL ), ModulePath, MAX_PATH );

#if defined(_M_AMD64)
#ifndef _DEBUG
    FileTreeHandle = FindFirstFile( _T( "plugins\\*.rc-plugin64" ), &FileData );
#else
    FileTreeHandle = FindFirstFile( _T( "plugins\\*.rc-plugin64d" ), &FileData );
#endif
#else
#ifndef _DEBUG
    FileTreeHandle = FindFirstFile( _T( "plugins\\*.rc-plugin" ), &FileData );
#else
    FileTreeHandle = FindFirstFile( _T( "plugins\\*.rc-plugind" ), &FileData );
#endif
#endif
    if (FileTreeHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            PluginBase = LoadLibrary( CString( _T( "plugins\\" ) ) + FileData.cFileName );
            if (PluginBase == NULL)
            {
                PrintOut( _T( "plugin %s was not able to be loaded!" ), FileData.cFileName );
                continue;
            }

            PluginInitFunction = (PPLUGIN_INIT)GetProcAddress( PluginBase, "PluginInit" );
            if (!PluginInitFunction)
            {
                PrintOut( _T( "%s is not a reclass plugin!" ), FileData.cFileName );
                FreeLibrary( PluginBase );
                continue;
            }

            PluginStateChangeFunction = (PPLUGIN_STATE_CHANGE)GetProcAddress( PluginBase, "PluginStateChange" );
            if (!PluginStateChangeFunction)
            {
                PrintOut( _T( "%s doesnt have exported state change function! "
                    "Unable to disable plugin on request, stop reclass and delete the plugin to disable it" ), FileData .cFileName );
            }

            PluginSettingDlgFunction = (DLGPROC)GetProcAddress( PluginBase, "PluginSettingsDlg" );

            Plugin = (PRECLASS_PLUGIN)_aligned_malloc( sizeof( RECLASS_PLUGIN ), 16 );
            wcscpy_s( Plugin->FileName, FileData.cFileName );
            Plugin->LoadedBase = PluginBase;
            Plugin->InitFunction = PluginInitFunction;
            Plugin->SettingDlgFunction = PluginSettingDlgFunction;
            Plugin->StateChangeFunction = PluginStateChangeFunction;

            if (PluginInitFunction( &Plugin->Info ))
            {
                Plugin->State = g_ReClassApp.GetProfileIntW( L"PluginState", Plugin->Info.Name, 1 ) == 1;

                if (Plugin->Info.DialogId == -1)
                    Plugin->SettingDlgFunction = NULL;

                PrintOut( _T( "Loaded plugin %s (%ls version %ls) - %ls" ), FileData.cFileName, Plugin->Info.Name, Plugin->Info.Version, Plugin->Info.About );
                if (Plugin->StateChangeFunction)
                    Plugin->StateChangeFunction( Plugin->State );

                g_LoadedPlugins.push_back( Plugin );
            }
            else
            {
                PrintOut( _T( "Failed to load plugin %s" ), FileData.cFileName );
                FreeLibrary( PluginBase );
            }
        } while (FindNextFile( FileTreeHandle, &FileData ));
    }
}

VOID 
UnloadPlugins( 
    VOID 
)
{
    for (PRECLASS_PLUGIN Plugin : g_LoadedPlugins)
    {
        FreeLibrary( Plugin->LoadedBase );
        _aligned_free( Plugin );
    }
    g_LoadedPlugins.clear( );
}


//
// Plugin API Routines Implementation
//

BOOL 
PLUGIN_CC 
ReClassOverrideReadMemoryOperation( 
    IN PPLUGIN_READ_MEMORY_OPERATION ReadMemoryOperation 
)
{
    if (ReadMemoryOperation != nullptr)
    {
        g_PluginOverrideReadMemoryOperation = ReadMemoryOperation;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassOverrideWriteMemoryOperation( 
    IN PPLUGIN_WRITE_MEMORY_OPERATION WriteMemoryOperation
)
{
    if (WriteMemoryOperation != nullptr)
    {
        g_PluginOverrideWriteMemoryOperation = WriteMemoryOperation;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassOverrideMemoryOperations( 
    IN PPLUGIN_READ_MEMORY_OPERATION ReadMemoryOperation,
    IN PPLUGIN_WRITE_MEMORY_OPERATION WriteMemoryOperation
)
{
    if (ReadMemoryOperation == nullptr && WriteMemoryOperation == nullptr)
        return FALSE;
    if (ReadMemoryOperation != nullptr)
        g_PluginOverrideReadMemoryOperation = ReadMemoryOperation;
    if (WriteMemoryOperation != nullptr)
        g_PluginOverrideWriteMemoryOperation = WriteMemoryOperation;
    return TRUE;
}

BOOL 
PLUGIN_CC 
ReClassRemoveReadMemoryOverride( 
    VOID 
)
{
    if (g_PluginOverrideReadMemoryOperation != nullptr)
    {
        g_PluginOverrideReadMemoryOperation = nullptr;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassRemoveWriteMemoryOverride( 
    VOID 
)
{
    if (g_PluginOverrideWriteMemoryOperation != nullptr)
    {
        g_PluginOverrideWriteMemoryOperation = nullptr;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassIsReadMemoryOverriden( 
    VOID 
)
{
    return (g_PluginOverrideReadMemoryOperation != nullptr) ? TRUE : FALSE;
}

BOOL 
PLUGIN_CC 
ReClassIsWriteMemoryOverriden( 
    VOID 
)
{
    return (g_PluginOverrideWriteMemoryOperation != nullptr) ? TRUE : FALSE;
}

PPLUGIN_READ_MEMORY_OPERATION 
PLUGIN_CC 
ReClassGetCurrentReadMemory( 
    VOID 
)
{
    return g_PluginOverrideReadMemoryOperation;
}

PPLUGIN_WRITE_MEMORY_OPERATION 
PLUGIN_CC 
ReClassGetCurrentWriteMemory( 
    VOID 
)
{
    return g_PluginOverrideWriteMemoryOperation;
}

BOOL 
PLUGIN_CC 
ReClassOverrideOpenProcessOperation( 
    IN PPLUGIN_OPEN_PROCESS_OPERATION OpenProcessOperation 
)
{
    if (OpenProcessOperation != nullptr)
    {
        g_PluginOverrideOpenProcessOperation = OpenProcessOperation;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassOverrideOpenThreadOperation( 
    IN PPLUGIN_OPEN_THREAD_OPERATION OpenThreadOperation 
)
{
    if (OpenThreadOperation != nullptr)
    {
        g_PluginOverrideOpenThreadOperation = OpenThreadOperation;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassOverrideHandleOperations( 
    IN PPLUGIN_OPEN_PROCESS_OPERATION OpenProcessOperation, 
    IN PPLUGIN_OPEN_THREAD_OPERATION OpenThreadOperation )
{
    if (OpenProcessOperation == nullptr && OpenThreadOperation == nullptr)
        return FALSE;
    if (OpenProcessOperation != nullptr)
        g_PluginOverrideOpenProcessOperation = OpenProcessOperation;
    if (OpenThreadOperation != nullptr)
        g_PluginOverrideOpenThreadOperation = OpenThreadOperation;
    return TRUE;
}

BOOL 
PLUGIN_CC 
ReClassRemoveOpenProcessOverride( 
    VOID 
)
{
    if (g_PluginOverrideOpenProcessOperation != nullptr)
    {
        g_PluginOverrideOpenProcessOperation = nullptr;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassRemoveOpenThreadOverride( 
    VOID 
)
{
    if (g_PluginOverrideOpenThreadOperation != nullptr)
    {
        g_PluginOverrideOpenThreadOperation = nullptr;
        return TRUE;
    }
    return FALSE;
}

BOOL 
PLUGIN_CC 
ReClassIsOpenProcessOverriden( 
    VOID 
)
{
    return (g_PluginOverrideOpenProcessOperation != nullptr) ? TRUE : FALSE;
}

BOOL 
PLUGIN_CC 
ReClassIsOpenThreadOverriden( 
    VOID 
)
{
    return (g_PluginOverrideOpenThreadOperation != nullptr) ? TRUE : FALSE;
}

PPLUGIN_OPEN_PROCESS_OPERATION 
PLUGIN_CC 
ReClassGetCurrentOpenProcess( 
    VOID 
)
{
    return g_PluginOverrideOpenProcessOperation;
}

PPLUGIN_OPEN_THREAD_OPERATION 
PLUGIN_CC 
ReClassGetCurrentOpenThread( 
    VOID 
)
{
    return g_PluginOverrideOpenThreadOperation;
}

VOID
CDECL 
ReClassPrintConsole( 
    IN const wchar_t *Format,
    ...
)
{
    wchar_t Buffer[2048];
    //ZeroMemory( Buffer, 2048 );
    
    va_list Args;
    va_start( Args, Format );
    _vsnwprintf_s( Buffer, 2048, Format, Args );
    va_end( Args );

    #if defined(_UNICODE)
    g_ReClassApp.m_pConsole->PrintText( Buffer );
    #else
    g_ReClassApp.m_pConsole->PrintText( CW2A( Buffer ) );
    #endif
}

HANDLE 
PLUGIN_CC 
ReClassGetProcessHandle( 
    VOID 
)
{
    return g_hProcess;
}

DWORD 
PLUGIN_CC 
ReClassGetProcessId( 
    VOID 
)
{
    return g_ProcessID;
}

HWND 
PLUGIN_CC 
ReClassMainWindow( 
    VOID 
)
{
    return *g_ReClassApp.GetMainWnd( );
}

CMFCRibbonBar* 
PLUGIN_CC 
ReClassRibbonInterface( 
    VOID 
)
{
    return g_ReClassApp.GetRibbonBar( );
}