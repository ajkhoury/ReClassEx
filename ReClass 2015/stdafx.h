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

//TODO: Make this easier to maintain
#define RECLASS_VERSION _T("1.0.3.0")

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
const COLORREF black = RGB(0, 0, 0);
const COLORREF white = RGB(255, 255, 255);
const COLORREF green = RGB(0, 200, 0);
const COLORREF red = RGB(255, 0, 0);
const COLORREF lightred = RGB(255, 100, 100);
const COLORREF blue = RGB(0, 0, 255);
const COLORREF darkblue = RGB(0, 0, 100);
const COLORREF yellow = RGB(255, 255, 0);
const COLORREF orange = RGB(255, 175, 65);
const COLORREF magenta = RGB(255, 0, 255);
const COLORREF cyan = RGB(0, 255, 255);
const COLORREF purple = RGB(128, 0, 255);

// Scintilla Colors structure
struct SScintillaColors {
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
// Globals
//
extern HANDLE g_hProcess;
extern DWORD  g_ProcessID;
extern size_t g_AttachedProcessAddress;
extern DWORD  g_AttachedProcessSize;
extern CString g_ProcessName;
extern Symbols* g_SymLoader;

extern std::vector<struct MemMapInfo> MemMap;
extern std::vector<struct MemMapInfo> MemMapCode;
extern std::vector<struct MemMapInfo> MemMapData;
extern std::vector<struct MemMapInfo> MemMapModule;
extern std::vector<struct AddressName> Exports;
extern std::vector<struct AddressName> CustomNames;

extern std::vector<HICON> Icons;

extern COLORREF crBackground;
extern COLORREF crSelect;
extern COLORREF crHidden;

extern COLORREF crOffset;
extern COLORREF crAddress;
extern COLORREF crType;
extern COLORREF crName;
extern COLORREF crIndex;
extern COLORREF crValue;
extern COLORREF crComment;

extern COLORREF crVTable;
extern COLORREF crFunction;
extern COLORREF crChar;
extern COLORREF crCustom;
extern COLORREF crHex;

#define FONT_DEFAULT_WIDTH 8
#define FONT_DEFAULT_HEIGHT 16

extern CFont g_ViewFont;
extern int g_FontWidth;
extern int g_FontHeight;

extern bool gbAddress;
extern bool gbOffset;
extern bool gbText;
extern bool gbRTTI;
extern bool gbResizingFont;
extern bool gbSymbolResolution;

extern bool gbFloat;
extern bool gbInt;
extern bool gbString;
extern bool gbPointers;

extern bool gbTop;
extern bool gbClassBrowser;
extern bool gbFilterProcesses;
extern bool gbPrivatePadding;
extern bool gbClipboardCopy;

extern CString tdHex;
extern CString tdInt64;

extern CString tdInt32;
extern CString tdInt16;
extern CString tdInt8;
extern CString tdQWORD;
extern CString tdDWORD;
extern CString tdWORD;
extern CString tdBYTE;
extern CString tdFloat;
extern CString tdDouble;
extern CString tdVec2;
extern CString tdVec3;
extern CString tdQuat;
extern CString tdMatrix;
extern CString tdPChar;
extern CString tdPWChar;

//
// NTDLL
//
namespace ntdll
{
	extern PVOID Base;
	extern tNtQuerySystemInformation NtQuerySystemInformation;
	extern tNtQueryInformationProcess NtQueryInformationProcess;
	extern tRtlGetVersion RtlGetVersion;
	extern tRtlGetNativeSystemInformation RtlGetNativeSystemInformation;
	extern tNtCreateThreadEx NtCreateThreadEx;
}



//
// Hotspot, Node, & Item IDs
//
#define WM_MAXITEMS 1024

#define WM_CLASSMENU WM_USER
#define WM_PROCESSMENU (WM_USER+WM_MAXITEMS)
#define WM_CHANGECLASSMENU (WM_USER+WM_MAXITEMS+WM_MAXITEMS)
#define WM_DELETECLASSMENU (WM_USER+WM_MAXITEMS+WM_MAXITEMS+WM_MAXITEMS)

#define	NONE -1
#define	HS_NONE -1
#define HS_EDIT 0
#define HS_OPENCLOSE 1
#define HS_SELECT 2
#define HS_DROP 3
#define HS_CLICK 4
#define HS_CHANGE_A 5
#define HS_CHANGE_X 6
#define HS_DELETE 7
#define HS_RTTI 8
#define HS_ADDRESS 68
#define HS_NAME 69
#define HS_COMMENT 70

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

enum NodeType
{
	nt_none = -1,
	nt_base,
	nt_instance,
	nt_struct,
	nt_hidden,
	nt_hex32,
	nt_hex64,
	nt_hex16,
	nt_hex8,
	nt_pointer,
	nt_int64,
	nt_int32,
	nt_int16,
	nt_int8,
	nt_float,
	nt_double,
	nt_uint32, // dword
	nt_uint16, // word
	nt_uint8, // byte
	nt_text,
	nt_unicode,
	nt_function,
	nt_custom,
	nt_vec2,
	nt_vec3,
	nt_quat,
	nt_matrix,
	nt_vtable,
	nt_array,
	nt_class,
	nt_pchar,
	nt_pwchar,
	nt_bits,
	nt_uint64, // qword
};

#define ISHEXTYPE(type) (type == nt_hex64 || type == nt_hex32 || type == nt_hex16 || type == nt_hex8 || type == nt_bits)

__inline const TCHAR* NodeTypeToString(NodeType type)
{
	static const TCHAR* pszNodeTypes[] = { _T("nt_base"), _T("nt_instance"), _T("nt_struct"), _T("nt_hidden"), _T("nt_hex32"), _T("nt_hex64"), _T("nt_hex16"), _T("nt_hex8"),
		_T("nt_pointer"), _T("nt_int64"), _T("nt_int32"), _T("nt_int16"), _T("nt_int8"), _T("nt_float"), _T("nt_double"), _T("nt_uint32"), _T("nt_uint16"), _T("nt_uint8"),
		_T("nt_text"), _T("nt_unicode"), _T("nt_function"), _T("nt_custom"), _T("nt_vec2"), _T("nt_vec3"), _T("nt_quat"), _T("nt_matrix"), _T("nt_vtable"), _T("nt_array"),
		_T("nt_class"), _T("nt_pchar"), _T("nt_pwchar"), _T("nt_bits"), _T("nt_uint64")
	};	
	return pszNodeTypes[type];
}


//
// Global functions
//
bool PauseResumeThreadList(bool bResumeThread);

bool UpdateMemoryMap();
bool UpdateExports();

size_t GetBase();
bool IsCode(size_t Address);
bool IsData(size_t Address);
bool IsMemory(size_t Address);
bool IsModule(size_t Address);

CString GetAddressName(size_t Address,bool bHEX);
CString GetModuleName(size_t Address);
size_t  GetAddressFromName(CString moduleName);

BOOL ReClassReadMemory(LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_read = nullptr);
BOOL ReClassWriteMemory(LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_wrote = nullptr);
HANDLE ReClassOpenProcess( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwProcessID );
HANDLE ReClassOpenThread( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwThreadID );

CStringA ReadMemoryStringA(size_t address, SIZE_T max = 40);
CStringW ReadMemoryStringW(size_t address, SIZE_T max = 40);

__int64 StrToNum(const TCHAR *udata, int udatalen, int base);
int SplitString(const CString& input, const CString& delimiter, CStringArray& results);
size_t ConvertStrToAddress(CString str);


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

struct HotSpot
{
	CRect Rect;
	CString Text;
	size_t Address;
	int ID;
	int Type;
	UINT Level;
	class CNodeBase* object;
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
void LoadPlugins();

#define RECLASS_EXPORT __declspec(dllexport) 
#define PLUGIN_CC __stdcall

typedef BOOL(PLUGIN_CC *MEMORY_OPERATION)(LPVOID, LPVOID, SIZE_T, PSIZE_T);
typedef HANDLE(PLUGIN_CC *HANDLE_OPERATION)(DWORD, BOOL, DWORD);

extern MEMORY_OPERATION g_PluginOverrideMemoryWrite;
extern MEMORY_OPERATION g_PluginOverrideMemoryRead;
extern HANDLE_OPERATION g_PluginOverrideHandleProcess;
extern HANDLE_OPERATION g_PluginOverrideHandleThread;

typedef struct _RECLASS_PLUGIN_INFO
{
	_RECLASS_PLUGIN_INFO() : DialogID(-1) {}

	wchar_t Name[260];
	wchar_t About[2048];
	wchar_t Version[260];
	int DialogID;
} RECLASS_PLUGIN_INFO, *LPRECLASS_PLUGIN_INFO;

BOOL PLUGIN_CC PluginInit(LPRECLASS_PLUGIN_INFO lpRCInfo);
void PLUGIN_CC PluginStateChange(bool state);
typedef BOOL(PLUGIN_CC *tPluginInit)(LPRECLASS_PLUGIN_INFO lpRCInfo);
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
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideMemoryOperations(MEMORY_OPERATION MemWrite, MEMORY_OPERATION MemRead, BOOL bForceSet = FALSE);
RECLASS_EXPORT BOOL PLUGIN_CC ReClassOverrideHandleOperations(HANDLE_OPERATION HandleProcess, HANDLE_OPERATION HandleThread, BOOL bForceSet = FALSE);
RECLASS_EXPORT void PLUGIN_CC ReClassPrintConsole(const wchar_t *format, ...);
RECLASS_EXPORT LPHANDLE PLUGIN_CC ReClassGetProcessHandle();
RECLASS_EXPORT HWND PLUGIN_CC ReClassMainWindow();
RECLASS_EXPORT CMFCRibbonBar* PLUGIN_CC ReClassRibbonInterface();

extern std::vector<RECLASS_PLUGINS> LoadedPlugins;
#pragma endregion


//
// Main Application
//
#include "ReClass2015.h"
extern CReClass2015App theApp;


//
// Global preprocessor directive for printing to the Console
//
#define PrintOut(fmt, ...) { \
do { \
	static TCHAR s_logbuf[1024]; \
	if (fmt) { \
		_sntprintf(s_logbuf, 1024, fmt, ##__VA_ARGS__); \
		theApp.Console->PrintText(s_logbuf); \
	} \
} while (0);\
}
