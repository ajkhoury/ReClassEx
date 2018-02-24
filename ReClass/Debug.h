#pragma once

#pragma warning(disable : 4996)

// For demangling rtti names

//
// UnDecorateSymbolName Flags
//
#ifndef UNDNAME_COMPLETE
#define UNDNAME_COMPLETE                 (0x0000)  // Enable full undecoration
#endif
#ifndef UNDNAME_NO_LEADING_UNDERSCORES
#define UNDNAME_NO_LEADING_UNDERSCORES   (0x0001)  // Remove leading underscores from MS extended keywords
#endif
#ifndef UNDNAME_NO_MS_KEYWORDS
#define UNDNAME_NO_MS_KEYWORDS           (0x0002)  // Disable expansion of MS extended keywords
#endif
#ifndef UNDNAME_NO_FUNCTION_RETURNS
#define UNDNAME_NO_FUNCTION_RETURNS      (0x0004)  // Disable expansion of return type for primary declaration
#endif
#ifndef UNDNAME_NO_ALLOCATION_MODEL
#define UNDNAME_NO_ALLOCATION_MODEL      (0x0008)  // Disable expansion of the declaration model
#endif
#ifndef UNDNAME_NO_ALLOCATION_LANGUAGE
#define UNDNAME_NO_ALLOCATION_LANGUAGE   (0x0010)  // Disable expansion of the declaration language specifier
#endif
#ifndef UNDNAME_NO_MS_THISTYPE
#define UNDNAME_NO_MS_THISTYPE           (0x0020)  // NYI Disable expansion of MS keywords on the 'this' type for primary declaration
#endif
#ifndef UNDNAME_NO_CV_THISTYPE
#define UNDNAME_NO_CV_THISTYPE           (0x0040)  // NYI Disable expansion of CV modifiers on the 'this' type for primary declaration
#endif
#ifndef UNDNAME_NO_THISTYPE
#define UNDNAME_NO_THISTYPE              (0x0060)  // Disable all modifiers on the 'this' type
#endif
#ifndef UNDNAME_NO_ACCESS_SPECIFIERS
#define UNDNAME_NO_ACCESS_SPECIFIERS     (0x0080)  // Disable expansion of access specifiers for members
#endif
#ifndef UNDNAME_NO_THROW_SIGNATURES
#define UNDNAME_NO_THROW_SIGNATURES      (0x0100)  // Disable expansion of 'throw-signatures' for functions and pointers to functions
#endif
#ifndef UNDNAME_NO_MEMBER_TYPE
#define UNDNAME_NO_MEMBER_TYPE           (0x0200)  // Disable expansion of 'static' or 'virtual'ness of members
#endif
#ifndef UNDNAME_NO_RETURN_UDT_MODEL
#define UNDNAME_NO_RETURN_UDT_MODEL      (0x0400)  // Disable expansion of MS model for UDT returns
#endif
#ifndef UNDNAME_32_BIT_DECODE
#define UNDNAME_32_BIT_DECODE            (0x0800)  // Undecorate 32-bit decorated names
#endif
#ifndef UNDNAME_NAME_ONLY
#define UNDNAME_NAME_ONLY                (0x1000)  // Crack only the name for primary declaration;
#endif
//  return just [scope::]name.  Does expand template params
#ifndef UNDNAME_NO_ARGUMENTS
#define UNDNAME_NO_ARGUMENTS             (0x2000)  // Don't undecorate arguments to function
#endif
#ifndef UNDNAME_NO_SPECIAL_SYMS
#define UNDNAME_NO_SPECIAL_SYMS          (0x4000)  // Don't undecorate special names (v-table, vcall, vector xxx, metatype, etc)
#endif

DWORD
WINAPI
UndecorateSymbolName(
    LPCSTR name,
    LPSTR outputString,
    DWORD maxStringLength,
    DWORD flags
    );
DWORD
WINAPI
UndecorateSymbolNameUnicode(
    LPCWSTR name,
    LPWSTR outputString,
    DWORD maxStringLength,
    DWORD flags
    );
#ifdef _UNICODE
#define _UnDecorateSymbolName UndecorateSymbolNameUnicode
#else
#define _UnDecorateSymbolName UndecorateSymbolName
#endif

#define VALID(x) (x != NULL && HIWORD(x))

#ifdef _WIN64
#define IsValidPtr(Ptr) ((Ptr >= 0x10000) && (Ptr < 0x000F000000000000))
#else
#define IsValidPtr(Ptr) ((Ptr >= 0x10000) && (Ptr < 0xFFF00000))
#endif

#pragma warning(default : 4996)