#pragma once

//
// WinAPI, MFC, & STD Includes
//

// Disable warnings for type casting from HANDLE to DWORD and vice versa
#pragma warning(disable : 4312 4311 4302 4099) 

#define PSAPI_VERSION 1
#define WIN32_LEAN_AND_MEAN
//#define _NO_CRT_STDIO_INLINE

#include "targetver.h"

#include <afx.h>

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>				// MFC core and standard components
#include <afxext.h>				// MFC extensions
#include <afxdisp.h>			// MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <ShellScalingApi.h>

#include <vector>

//
// BeaEngine disassembler 
//
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "..\\beaengine\\beaengine\\headers\\BeaEngine.h"

//
// Scintilla parser
//
#define PLAT_WIN 1
#include "SciLexer.h"
#include "Scintilla.h"

// A few basic colors for scintilla editor
#define RGB_BLACK       RGB( 0, 0, 0 )
#define RGB_WHITE       RGB( 255, 255, 255 )
#define RGB_GREEN       RGB( 0, 200, 0 )
#define RGB_DARKGREEN   RGB( 0, 100, 0 )
#define RGB_RED         RGB( 255, 0, 0 )
#define RGB_LIGHTRED    RGB( 255, 100, 100 )
#define RGB_BLUE        RGB( 0, 0, 255 )
#define RGB_DARKBLUE    RGB( 0, 0, 150 )
#define RGB_YELLOW      RGB( 255, 255, 0 )
#define RGB_DARKYELLOW  RGB( 100, 100, 0 )
#define RGB_ORANGE      RGB( 255, 175, 65 )
#define RGB_MAGENTA     RGB( 255, 0, 255 )
#define RGB_CYAN        RGB( 0, 255, 255 )
#define RGB_PURPLE      RGB( 128, 0, 255 )

//
// TinyXml parser
//
#include "tinyxml2.h"
using namespace tinyxml2;

//
// Utilities
//
#include "Utils.h"

//
// Symbols
//
#include "Symbols.h"

//
// NTDLL
//
#include "NtDll.h"

//
// Globals
//
extern HANDLE g_hProcess;
extern DWORD  g_ProcessID;
extern ULONG_PTR g_AttachedProcessAddress;
extern DWORD  g_AttachedProcessSize;
extern CString g_ProcessName;

extern std::vector<struct MemMapInfo> g_MemMap;
extern std::vector<struct MemMapInfo> g_MemMapCode;
extern std::vector<struct MemMapInfo> g_MemMapData;
extern std::vector<struct MemMapInfo> g_MemMapModules;
extern std::vector<struct AddressName> g_Exports;
extern std::vector<struct AddressName> g_CustomNames;

extern std::vector<HICON> g_Icons;

extern COLORREF g_clrBackground;
extern COLORREF g_clrSelect;
extern COLORREF g_clrHidden;
extern COLORREF g_clrOffset;
extern COLORREF g_clrAddress;
extern COLORREF g_clrType;
extern COLORREF g_clrName;
extern COLORREF g_clrIndex;
extern COLORREF g_clrValue;
extern COLORREF g_clrComment;
extern COLORREF g_clrVTable;
extern COLORREF g_clrFunction;
extern COLORREF g_clrChar;
extern COLORREF g_clrCustom;
extern COLORREF g_clrHex;

#define FONT_DEFAULT_WIDTH	8
#define FONT_DEFAULT_HEIGHT 16
#define FONT_DEFAULT_SIZE	10

extern CString g_ViewFontName;
extern CFont g_ViewFont;
extern int g_FontWidth;
extern int g_FontHeight;

extern bool g_bAddress;
extern bool g_bOffset;
extern bool g_bText;
extern bool g_bRTTI;
extern bool g_bRandomName;
extern bool g_bResizingFont;
extern bool g_bSymbolResolution;
extern bool g_bLoadModuleSymbol;

extern bool g_bFloat;
extern bool g_bInt;
extern bool g_bString;
extern bool g_bPointers;
extern bool g_bUnsignedHex;

extern bool g_bTop;
extern bool g_bClassBrowser;
extern bool g_bFilterProcesses;
extern bool g_bPrivatePadding;
extern bool g_bClipboardCopy;

typedef struct _RCTYPEDEFS {
    CString Hex;
    CString Int64;
    CString Int32;
    CString Int16;
    CString Int8;
    CString Qword;
    CString Dword;
    CString Word;
    CString Byte;
    CString Float;
    CString Double;
    CString Vec2;
    CString Vec3;
    CString Quat;
    CString Matrix;
    CString PChar;
    CString PWChar;
} RCTYPEDEFS;
extern RCTYPEDEFS g_Typedefs;


//
// Hotspot, Node, & Item IDs
//
#define WM_MAXITEMS 1024

#define WM_CLASSMENU WM_USER
#define WM_PROCESSMENU (WM_USER+WM_MAXITEMS)
#define WM_CHANGECLASSMENU (WM_USER+WM_MAXITEMS+WM_MAXITEMS)
#define WM_DELETECLASSMENU (WM_USER+WM_MAXITEMS+WM_MAXITEMS+WM_MAXITEMS)


#define ICON_OPEN 0
#define ICON_CLOSED 1
#define ICON_CLASS 2
#define ICON_METHOD 3
#define ICON_VTABLE 4
#define ICON_DELETE 5
#define ICON_ADD 6
#define ICON_RANDOM 7
#define ICON_DROPARROW 8
#define ICON_POINTER 9
#define ICON_ARRAY 10
#define ICON_CUSTOM 11
#define ICON_ENUM 12
#define ICON_FLOAT 13
#define ICON_LEFT 14
#define ICON_RIGHT 15
#define ICON_MATRIX 16
#define ICON_INTEGER 17
#define ICON_TEXT 18
#define ICON_UNSIGNED 19
#define ICON_VECTOR 20
#define ICON_CHANGE 21
#define ICON_CAMERA 22

// Max export entries allowed
#define MAX_EXPORTS 16384


//
// Global functions
//
BOOLEAN PauseResumeThreadList( BOOL bResumeThread );
BOOLEAN UpdateMemoryMap( );
BOOLEAN UpdateExports( );

ULONG_PTR GetBaseAddress( );

BOOLEAN IsCode( ULONG_PTR Address );
BOOLEAN IsData( ULONG_PTR Address );
BOOLEAN IsMemory( ULONG_PTR Address );
BOOLEAN IsModule( ULONG_PTR Address );

ULONG_PTR GetModuleBaseFromAddress( ULONG_PTR Address );

CString GetAddressName( ULONG_PTR Address, BOOLEAN bJustAddress );
CString GetModuleName( ULONG_PTR Address );
ULONG_PTR GetAddressFromName( CString moduleName );

BOOL ReClassReadMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, PSIZE_T BytesRead = nullptr );
BOOL ReClassWriteMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, PSIZE_T BytesWritten = nullptr );
HANDLE ReClassOpenProcess( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwProcessID );
HANDLE ReClassOpenThread( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwThreadID );

CStringA ReadMemoryStringA( ULONG_PTR address, SIZE_T max = 40 );
CStringW ReadMemoryStringW( ULONG_PTR address, SIZE_T max = 40 );

int SplitString( const CString& input, const CString& delimiter, CStringArray& results );
ULONG_PTR ConvertStrToAddress( CString str );


// 
// Classes & Structs
//
#include "CMemory.h"

struct MemMapInfo
{
    ULONG_PTR  Start;
    ULONG_PTR  End;
    DWORD   Size;
    CString Name;
    CString Path;
};

struct AddressName
{
    CString Name;
    ULONG_PTR Address;
};

//
// Nodes 
//
#include "Nodes.h"
#if defined(_M_AMD64)
#define CNodeHex CNodeHex64
#else
#define CNodeHex CNodeHex32
#endif


//
// Main Application
//
#include "ReClassEx.h"
extern CReClassExApp g_ReClassApp;


//
// Global preprocessor directive for printing to the Console
//
static TCHAR s_ReClassLogBuf[1024]; \

#define PrintOut(fmt, ...) { \
do { \
    if (fmt) { \
        _sntprintf_s(s_ReClassLogBuf, 1024, fmt, ##__VA_ARGS__); \
        g_ReClassApp.m_pConsole->PrintText(s_ReClassLogBuf); \
    } \
} while (0);\
}
#if _DEBUG
#define PrintOutDbg(fmt, ...) { \
do { \
    if (fmt) { \
        _sntprintf_s(s_ReClassLogBuf, 1024, fmt, ##__VA_ARGS__); \
        g_ReClassApp.m_pConsole->PrintText(s_ReClassLogBuf); \
    } \
} while (0);\
}
#else
#define PrintOutDbg(fmt, ...) (void)(fmt)
#endif

//
// Plugins
//
// NOTE: Plugins disabled and enabled states are dependent on the implementation inside the plugin.
// All we do is send a state change to plugins for them to disable or enable their functionality.
// Also decided to change folder creation so that its up to the user to create the folder if they 
// want/have plugins.
//
#include "PluginAPI.h"