#pragma once

#pragma warning(disable : 4996)

// For demangling rtti names
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")
#ifdef UNICODE
#define _UnDecorateSymbolName UnDecorateSymbolNameW
#else
#define _UnDecorateSymbolName UnDecorateSymbolName
#endif

#define VALID(x) (x != NULL && HIWORD(x))

#ifdef _WIN64
#define IsValidPtr(Ptr) ((Ptr >= 0x10000) && (Ptr < 0x000F000000000000))
#else
#define IsValidPtr(Ptr) ((Ptr >= 0x10000) && (Ptr < 0xFFF00000))
#endif

#pragma warning(default : 4996)