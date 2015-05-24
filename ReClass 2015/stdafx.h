#pragma once

//#ifndef _SECURE_ATL
//#define _SECURE_ATL 1
//#endif

#define WIN32_LEAN_AND_MEAN
#include "targetver.h"

#include <afx.h>

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <Shlwapi.h>
#include <Psapi.h>
#include <tlhelp32.h>

#include "..\SQLite\CppSQLite3.h"
#include "..\tinyxml\tinyxml.h"

#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "..\beaengine\headers\BeaEngine.h"
#ifdef _WIN64
#pragma comment( lib, "../beaengine/Win64/Lib/BeaEngine64.lib")
#else
#pragma comment( lib, "../beaengine/Win32/Lib/BeaEngine.lib")
#endif

// Include Scintilla parser
#include "..\scintilla/include/SciLexer.h"
#include "..\scintilla/include/Scintilla.h"

#ifdef _DEBUG

#ifdef _WIN64
#pragma comment(lib, "../scintilla/bin/SciLexerd_64.lib")
#else
#pragma comment(lib, "../scintilla/bin/SciLexerd.lib")
#endif

#else

#ifdef _WIN64
#pragma comment(lib, "../scintilla/bin/SciLexer_64.lib")
#else
#pragma comment(lib, "../scintilla/bin/SciLexer.lib")
#endif

#endif


#pragma comment(lib,"Psapi.lib")
#include <vector>


//Globals
extern HANDLE hProcess;
extern DWORD ProcessID;

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

extern CFont Font;
//extern CFont SmallFont;
extern int FontWidth;
extern int FontHeight;

extern bool gbAddress;
extern bool gbOffset;
extern bool gbText;
extern bool gbFloat;
extern bool gbInt;
extern bool gbString;
extern bool gbPointers;

extern bool gbTop;
extern bool gbClassBrowser;
extern bool gbFilterProcesses;

extern CString tdHex;
extern CString tdInt64;

extern CString tdInt32;
extern CString tdInt16;
extern CString tdInt8;
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

#define WM_MAXITEMS 128

#define WM_CLASSMENU WM_USER
#define WM_PROCESSMENU (WM_USER+128)
#define WM_CHANGECLASSMENU (WM_USER+256)
#define WM_DELETECLASSMENU (WM_USER+384)

#define	NONE -1
#define HS_EDIT 0
#define HS_OPENCLOSE 1
#define HS_SELECT 2
#define HS_DROP 3
#define HS_CLICK 4
#define HS_CHANGE_A 5
#define HS_CHANGE_X 6
#define HS_DELETE 7
#define HS_RTTI 8

class CNodeBase;

class HotSpot
{
public:
	CRect Rect;
	CString Text;
	DWORD_PTR Address;
	int ID;
	int Type;
	UINT Level;
	CNodeBase* object;
};

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

extern std::vector<HICON> Icons;

enum NodeType
{
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
	nt_pchar
};

#define HEXTYPE (nt_hex64 | nt_hex32 | nt_hex16 | nt_hex8)

__inline const char* NodeTypeToString(NodeType type)
{
	static const char* pszNodeTypes[31] = {
		"nt_base",
		"nt_instance",
		"nt_struct",
		"nt_hidden",
		"nt_hex32",
		"nt_hex64",
		"nt_hex16",
		"nt_hex8",
		"nt_pointer",
		"nt_int64",
		"nt_int32",
		"nt_int16",
		"nt_int8",
		"nt_float",
		"nt_double",
		"nt_uint32",
		"nt_uint16",
		"nt_uint8",
		"nt_text",
		"nt_unicode",
		"nt_function",
		"nt_custom",
		"nt_vec2",
		"nt_vec3",
		"nt_quat",
		"nt_matrix",
		"nt_vtable",
		"nt_array",
		"nt_class",
		"nt_pchar"
	};
	return pszNodeTypes[type];
}

struct MemMapInfo
{
	DWORD_PTR Start;
	DWORD_PTR End;
	CString Name;
	//bool IsModule;
};
struct AddressName
{
	CString Name;
	DWORD_PTR Address;
};
extern std::vector<MemMapInfo> MemMap;
extern std::vector<MemMapInfo> MemMapCode;
extern std::vector<MemMapInfo> MemMapData;
extern std::vector<MemMapInfo> MemMapModule;
extern std::vector<AddressName> Exports;
extern std::vector<AddressName> CustomNames;

void PauseResumeThreadList(bool bResumeThread);

void UpdateMemoryMap();
void UpdateExports();

bool IsCode(DWORD_PTR Address);
bool IsData(DWORD_PTR Address);
bool IsMemory(DWORD_PTR Address);
bool IsModule(DWORD_PTR Address);

CString GetAddressName(DWORD_PTR Address,bool bHEX);
CString GetModuleName(DWORD_PTR Address);

void ReadMemory(DWORD_PTR Address,void* Buffer,DWORD Size);
void WriteMemory(DWORD_PTR Address,void* Buffer,DWORD Size);
CString ReadMemoryString( DWORD_PTR address, SIZE_T max = 40 );

#include "Classes.h"

extern DWORD NodeCreateIndex;

__int64 StrToNum(const TCHAR *udata, int udatalen, int base);
int SplitString(const CString& input, const CString& delimiter, CStringArray& results);
DWORD_PTR ConvertStrToAddress(CString Address);

