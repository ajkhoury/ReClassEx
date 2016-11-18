#pragma once

//
// WinAPI, MFC, & STD Includes
//

// Disable warnings for type casting from HANDLE to DWORD and vice versa
#pragma warning(disable : 4312 4311 4302 4099) 
#define PSAPI_VERSION 1
#define WIN32_LEAN_AND_MEAN

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
//#include <cstdarg>
#include <Shlwapi.h>
#include <Psapi.h>
#include <CommCtrl.h>
#include <algorithm>
#include <memory>
#include <map>

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
#include "..\\scintilla\\include\\SciLexer.h"
#include "..\\scintilla\\include\\Scintilla.h"

// A few basic colors for scintilla editor
const COLORREF black = RGB( 0, 0, 0 );
const COLORREF white = RGB( 255, 255, 255 );
const COLORREF green = RGB( 0, 200, 0 );
const COLORREF red = RGB( 255, 0, 0 );
const COLORREF lightred = RGB( 255, 100, 100 );
const COLORREF blue = RGB( 0, 0, 255 );
const COLORREF darkblue = RGB( 0, 0, 100 );
const COLORREF yellow = RGB( 255, 255, 0 );
const COLORREF orange = RGB( 255, 175, 65 );
const COLORREF magenta = RGB( 255, 0, 255 );
const COLORREF cyan = RGB( 0, 255, 255 );
const COLORREF purple = RGB( 128, 0, 255 );

// Scintilla Colors structure
struct SScintillaColors
{
	int			iItem;
	COLORREF	rgb;
};


//
// TinyXml parser
//
//#include "tinyxml2_unicode.h"
#include "tinyxml2.h"
using namespace tinyxml2;


//
// SQL parser
// Not even needed until importing is done
#include "..\\SQLite\\CppSQLite3.h"

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
extern size_t g_AttachedProcessAddress;
extern DWORD  g_AttachedProcessSize;
extern CString g_ProcessName;
extern Symbols* g_SymLoader;

extern std::vector<struct MemMapInfo> g_MemMap;
extern std::vector<struct MemMapInfo> g_MemMapCode;
extern std::vector<struct MemMapInfo> g_MemMapData;
extern std::vector<struct MemMapInfo> g_MemMapModules;
extern std::vector<struct AddressName> g_Exports;
extern std::vector<struct AddressName> g_CustomNames;

extern std::vector<HICON> g_Icons;

extern COLORREF g_crBackground;
extern COLORREF g_crSelect;
extern COLORREF g_crHidden;
extern COLORREF g_crOffset;
extern COLORREF g_crAddress;
extern COLORREF g_crType;
extern COLORREF g_crName;
extern COLORREF g_crIndex;
extern COLORREF g_crValue;
extern COLORREF g_crComment;
extern COLORREF g_crVTable;
extern COLORREF g_crFunction;
extern COLORREF g_crChar;
extern COLORREF g_crCustom;
extern COLORREF g_crHex;

#define FONT_DEFAULT_WIDTH 8
#define FONT_DEFAULT_HEIGHT 16

extern CFont g_ViewFont;
extern int g_FontWidth;
extern int g_FontHeight;

extern bool g_bAddress;
extern bool g_bOffset;
extern bool g_bText;
extern bool g_bRTTI;
extern bool g_bResizingFont;
extern bool g_bSymbolResolution;
extern bool g_bLoadModuleSymbol;

extern bool g_bFloat;
extern bool g_bInt;
extern bool g_bString;
extern bool g_bPointers;

extern bool g_bTop;
extern bool g_bClassBrowser;
extern bool g_bFilterProcesses;
extern bool g_bPrivatePadding;
extern bool g_bClipboardCopy;

extern CString g_tdHex;
extern CString g_tdInt64;
extern CString g_tdInt32;
extern CString g_tdInt16;
extern CString g_tdInt8;
extern CString g_tdQWORD;
extern CString g_tdDWORD;
extern CString g_tdWORD;
extern CString g_tdBYTE;
extern CString g_tdFloat;
extern CString g_tdDouble;
extern CString g_tdVec2;
extern CString g_tdVec3;
extern CString g_tdQuat;
extern CString g_tdMatrix;
extern CString g_tdPChar;
extern CString g_tdPWChar;

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

size_t GetBase( );
bool IsCode( size_t Address );
bool IsData( size_t Address );
bool IsMemory( size_t Address );
bool IsModule( size_t Address );

CString GetAddressName( size_t Address, bool bHEX );
CString GetModuleName( size_t Address );
size_t  GetAddressFromName( CString moduleName );

BOOL ReClassReadMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_read = nullptr );
BOOL ReClassWriteMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_wrote = nullptr );
HANDLE ReClassOpenProcess( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwProcessID );
HANDLE ReClassOpenThread( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwThreadID );

CStringA ReadMemoryStringA( size_t address, SIZE_T max = 40 );
CStringW ReadMemoryStringW( size_t address, SIZE_T max = 40 );

__int64 StrToNum( const TCHAR *udata, int udatalen, int base );
int SplitString( const CString& input, const CString& delimiter, CStringArray& results );
size_t ConvertStrToAddress( CString str );


// 
// Classes & Structs
//
#include "CMemory.h"

struct MemMapInfo
{
	size_t  Start;
	size_t  End;
	DWORD   Size;
	CString Name;
	CString Path;
};

struct AddressName
{
	CString Name;
	size_t Address;
};

// All node type classes
#include "Classes.h"

#ifdef _WIN64
#define CNodeHex CNodeHex64
#else
#define CNodeHex CNodeHex32
#endif

//
// Plugins
// NOTE: Plugins disable and enabled state are dependant on the implementation inside the plugin
// All we do is send a state change to plugins for them to disable or enable their functionality
// Also decided to change folder creation so that its up to the user to create the folder if they want/have plugins
//
#pragma region Plugins
void LoadPlugins( );

#define RECLASS_EXPORT __declspec(dllexport) 
#define PLUGIN_CC __stdcall

typedef BOOL( PLUGIN_CC *tMemoryOperation )(LPVOID, LPVOID, SIZE_T, PSIZE_T);
typedef HANDLE( PLUGIN_CC *tHandleOperation )(DWORD, BOOL, DWORD);

typedef struct _RECLASS_PLUGIN_INFO
{
	_RECLASS_PLUGIN_INFO( ) : DialogID( -1 ) { }

	wchar_t Name[260];
	wchar_t About[2048];
	wchar_t Version[260];
	int DialogID;
} RECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

BOOL PLUGIN_CC PluginInit( LPRECLASS_PLUGIN_INFO lpRCInfo );
void PLUGIN_CC PluginStateChange( bool state );
typedef BOOL( PLUGIN_CC *tPluginInit )(LPRECLASS_PLUGIN_INFO lpRCInfo);
typedef void(PLUGIN_CC *tPluginStateChange)(bool state);

typedef struct _RECLASS_PLUGINS
{
	RECLASS_PLUGIN_INFO Info;
	wchar_t FileName[260];
	bool State;
	HMODULE LoadedBase;
	tPluginInit InitFnc;
	tPluginStateChange StateChangeFnc;
	DLGPROC SettingDlgFnc;
} RECLASS_PLUGINS, *LPRECLASS_PLUGINS;

//Exported Functions Below
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideMemoryOperations( tMemoryOperation MemWrite, tMemoryOperation MemRead, BOOL bForceSet = FALSE );
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideHandleOperations( tHandleOperation HandleProcess, tHandleOperation HandleThread, BOOL bForceSet = FALSE );
RECLASS_EXPORT void PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... );
RECLASS_EXPORT LPHANDLE PLUGIN_CC ReClassGetProcessHandle( );
RECLASS_EXPORT HWND PLUGIN_CC ReClassMainWindow( );
RECLASS_EXPORT CMFCRibbonBar* PLUGIN_CC ReClassRibbonInterface( );

extern tMemoryOperation g_PluginOverrideMemoryWrite;
extern tMemoryOperation g_PluginOverrideMemoryRead;
extern tHandleOperation g_PluginOverrideHandleProcess;
extern tHandleOperation g_PluginOverrideHandleThread;

extern std::vector<RECLASS_PLUGINS> g_LoadedPlugins;
#pragma endregion


//
// Main Application
//
#include "ReClass2016.h"
extern CReClass2016App g_ReClassApp;


//
// Global preprocessor directive for printing to the Console
//
#define PrintOut(fmt, ...) { \
do { \
	if (fmt) { \
		static TCHAR s_logbuf[1024]; \
		_sntprintf(s_logbuf, 1024, fmt, ##__VA_ARGS__); \
		g_ReClassApp.Console->PrintText(s_logbuf); \
	} \
} while (0);\
}
