#pragma once

#include <Windows.h>

///
/// < Common Defines >
///

#ifndef _NTDEF_
typedef long NTSTATUS;
typedef NTSTATUS *PNTSTATUS;
#endif
#ifndef NT_SUCCESS
#define NT_SUCCESS(x) ((x) >= 0)
#endif //!NT_SUCCESS
#ifndef NT_INFORMATION
#define NT_INFORMATION(Status) ((((ULONG)(Status)) >> 30) == 1)
#endif //!NT_INFORMATION
#ifndef NT_WARNING
#define NT_WARNING( Status ) ((((ULONG)(Status)) >> 30) == 2)
#endif //!NT_WARNING
#ifndef NT_ERROR
#define NT_ERROR( Status ) ((((ULONG)(Status)) >> 30) == 3)
#endif //!NT_ERROR
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS                 ((NTSTATUS)0x00000000L)
#endif //!STATUS_SUCCESS
#ifndef STATUS_UNSUCCESSFUL
#define STATUS_UNSUCCESSFUL            ((NTSTATUS)0xC0000001L)
#endif //!STATUS_UNSUCCESSFUL
#ifndef STATUS_NOT_IMPLEMENTED
#define STATUS_NOT_IMPLEMENTED         ((NTSTATUS)0xC0000002L)
#endif //!STATUS_NOT_IMPLEMENTED
#ifndef STATUS_INFO_LENGTH_MISMATCH
#define STATUS_INFO_LENGTH_MISMATCH    ((NTSTATUS)0xC0000004L)
#endif //!STATUS_INFO_LENGTH_MISMATCH
#ifndef STATUS_NO_MEMORY
#define STATUS_NO_MEMORY               ((NTSTATUS)0xC0000017L)
#endif //!STATUS_NO_MEMORY
#ifndef STATUS_ACCESS_DENIED
#define STATUS_ACCESS_DENIED           ((NTSTATUS)0xC0000022L)
#endif //!STATUS_ACCESS_DENIED
#ifndef STATUS_BUFFER_TOO_SMALL
#define STATUS_BUFFER_TOO_SMALL        ((NTSTATUS)0xC0000023L)
#endif //!STATUS_BUFFER_TOO_SMALL
#ifndef STATUS_PROCEDURE_NOT_FOUND
#define STATUS_PROCEDURE_NOT_FOUND     ((NTSTATUS)0xC000007AL)
#endif //!STATUS_PROCEDURE_NOT_FOUND
#ifndef STATUS_NOT_SUPPORTED
#define STATUS_NOT_SUPPORTED           ((NTSTATUS)0xC00000BBL)
#endif //!STATUS_NOT_SUPPORTED
#ifndef STATUS_NOT_FOUND
#define STATUS_NOT_FOUND               ((NTSTATUS)0xC0000225L)
#endif //!STATUS_NOT_FOUND

#ifndef NOTHING
#define NOTHING
#endif

#ifndef FASTCALL
#ifndef _WIN64
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
#endif


///
/// < Common Datatypes >
///

// Cardinal types
typedef char CCHAR;
typedef short CSHORT;
typedef ULONG CLONG;
typedef CCHAR *PCCHAR;
typedef CSHORT *PCSHORT;
typedef CLONG *PCLONG;
typedef PCSTR PCSZ;

// Specific
typedef LONG KPRIORITY;
typedef UCHAR KIRQL, *PKIRQL;
typedef USHORT RTL_ATOM, *PRTL_ATOM;
typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;
typedef GUID *PGUID;

///
/// < Common Enums >
///

typedef enum _WAIT_TYPE
{
	WaitAll,
	WaitAny,
	WaitNotification
} WAIT_TYPE;

typedef enum _TIMER_TYPE
{
	NotificationTimer,
	SynchronizationTimer
} TIMER_TYPE;

typedef enum _EVENT_TYPE
{
	NotificationEvent,
	SynchronizationEvent
} EVENT_TYPE;


///
/// < Common structures >
///

typedef struct _CLIENT_ID
{
	HANDLE					UniqueProcess;
	HANDLE					UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _CLIENT_ID32
{
    ULONG UniqueProcess;
    ULONG UniqueThread;
} CLIENT_ID32, *PCLIENT_ID32;

typedef struct _CLIENT_ID64
{
    ULONGLONG UniqueProcess;
    ULONGLONG UniqueThread;
} CLIENT_ID64, *PCLIENT_ID64;

typedef struct _SINGLE_LIST_ENTRY64
{
	ULONGLONG Next;
} SINGLE_LIST_ENTRY64, *PSINGLE_LIST_ENTRY64;

typedef struct _SINGLE_LIST_ENTRY32
{
	ULONG Next;
} SINGLE_LIST_ENTRY32, *PSINGLE_LIST_ENTRY32;

typedef struct _ANSI_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PSTR Buffer;
} STRING, ANSI_STRING, OEM_STRING, *PSTRING, *PANSI_STRING, *POEM_STRING;
typedef const STRING *PCSTRING;
typedef const ANSI_STRING *PCANSI_STRING;
typedef const OEM_STRING *PCOEM_STRING;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef struct _STRING64
{
    USHORT Length;
    USHORT MaximumLength;
    ULONGLONG Buffer;
} STRING64, *PSTRING64;
typedef STRING64 UNICODE_STRING64, *PUNICODE_STRING64;
typedef STRING64 ANSI_STRING64, *PANSI_STRING64;

typedef struct _STRING32
{
	USHORT   Length;
	USHORT   MaximumLength;
	ULONG  Buffer;
} STRING32;
typedef STRING32 *PSTRING32;
typedef STRING32 UNICODE_STRING32;
typedef UNICODE_STRING32 *PUNICODE_STRING32;

typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, *PCURDIR;

typedef struct _CURDIR64
{
	UNICODE_STRING64 DosPath;
	ULONGLONG Handle;
} CURDIR64, *PCURDIR64;

typedef struct _CURDIR32
{
    UNICODE_STRING32 DosPath;
    ULONG Handle; // HANDLE
} CURDIR32, *PCURDIR32;

typedef struct _OBJECT_ATTRIBUTES
{
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor; // PSECURITY_DESCRIPTOR;
    PVOID SecurityQualityOfService; // PSECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef const OBJECT_ATTRIBUTES *PCOBJECT_ATTRIBUTES;

typedef struct _OBJECT_ATTRIBUTES64
{
    ULONG Length;
    ULONG64 RootDirectory;
    ULONG64 ObjectName;
    ULONG Attributes;
    ULONG64 SecurityDescriptor;
    ULONG64 SecurityQualityOfService;
} OBJECT_ATTRIBUTES64, *POBJECT_ATTRIBUTES64;
typedef const OBJECT_ATTRIBUTES64 *PCOBJECT_ATTRIBUTES64;

typedef struct _OBJECT_ATTRIBUTES32
{
    ULONG Length;
    ULONG RootDirectory;
    ULONG ObjectName;
    ULONG Attributes;
    ULONG SecurityDescriptor;
    ULONG SecurityQualityOfService;
} OBJECT_ATTRIBUTES32, *POBJECT_ATTRIBUTES32;
typedef const OBJECT_ATTRIBUTES32 *PCOBJECT_ATTRIBUTES32;
