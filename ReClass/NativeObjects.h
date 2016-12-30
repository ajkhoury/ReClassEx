#pragma once

#include "NativeCommon.h"

///
/// < Objects Macros and Definitions >
///

#define OBJECT_TYPE_CREATE 0x0001
#define OBJECT_TYPE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)

#define DIRECTORY_QUERY 0x0001
#define DIRECTORY_TRAVERSE 0x0002
#define DIRECTORY_CREATE_OBJECT 0x0004
#define DIRECTORY_CREATE_SUBDIRECTORY 0x0008
#define DIRECTORY_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0xF)

#define SYMBOLIC_LINK_QUERY 0x0001
#define SYMBOLIC_LINK_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)

#define DUPLICATE_CLOSE_SOURCE 0x00000001
#define DUPLICATE_SAME_ACCESS 0x00000002
#define DUPLICATE_SAME_ATTRIBUTES 0x00000004

// Object attribute defines
#define OBJ_PROTECT_CLOSE 0x00000001L
#define OBJ_INHERIT 0x00000002L
#define OBJ_AUDIT_OBJECT_CLOSE 0x00000004L
#define OBJ_PERMANENT 0x00000010L
#define OBJ_EXCLUSIVE 0x00000020L
#define OBJ_CASE_INSENSITIVE 0x00000040L
#define OBJ_OPENIF 0x00000080L
#define OBJ_OPENLINK 0x00000100L
#define OBJ_KERNEL_HANDLE 0x00000200L
#define OBJ_FORCE_ACCESS_CHECK 0x00000400L
#define OBJ_VALID_ATTRIBUTES 0x000007F2L
//#define OBJ_VALID_ATTRIBUTES_KRNL 0x00001FF2L

#define InitializeObjectAttributes(p, n, a, r, s) { \
    (p)->Length = sizeof(OBJECT_ATTRIBUTES); \
    (p)->RootDirectory = r; \
    (p)->Attributes = a; \
    (p)->ObjectName = n; \
    (p)->SecurityDescriptor = s; \
    (p)->SecurityQualityOfService = NULL; \
    }

#define InitializeObjectAttributes64(p,n,a,r,s) { \
	(p)->Length = sizeof(_OBJECT_ATTRIBUTES64); \
	(p)->RootDirectory = r; \
	(p)->Attributes = a; \
	(p)->ObjectName = n; \
	(p)->SecurityDescriptor = s; \
	(p)->SecurityQualityOfService = NULL; \
}

#define InitializeObjectAttributes32(p,n,a,r,s) { \
	(p)->Length = sizeof(_OBJECT_ATTRIBUTES32); \
	(p)->RootDirectory = r; \
	(p)->Attributes = a; \
	(p)->ObjectName = n; \
	(p)->SecurityDescriptor = s; \
	(p)->SecurityQualityOfService = NULL; \
}


///
/// < Objects Enums >
///

enum _KOBJECTS
{
	EventNotificationObject = 0x0,
	EventSynchronizationObject = 0x1,
	MutantObject = 0x2,
	ProcessObject = 0x3,
	QueueObject = 0x4,
	SemaphoreObject = 0x5,
	ThreadObject = 0x6,
	GateObject = 0x7,
	TimerNotificationObject = 0x8,
	TimerSynchronizationObject = 0x9,
	Spare2Object = 0xa,
	Spare3Object = 0xb,
	Spare4Object = 0xc,
	Spare5Object = 0xd,
	Spare6Object = 0xe,
	Spare7Object = 0xf,
	Spare8Object = 0x10,
	Spare9Object = 0x11,
	ApcObject = 0x12,
	DpcObject = 0x13,
	DeviceQueueObject = 0x14,
	EventPairObject = 0x15,
	InterruptObject = 0x16,
	ProfileObject = 0x17,
	ThreadedDpcObject = 0x18,
	MaximumKernelObject = 0x19,
};


///
/// < Objects Routines >
///

typedef NTSTATUS(NTAPI *tNtDuplicateObject)(
	IN HANDLE SourceProcessHandle,
	IN HANDLE SourceHandle,
	IN HANDLE TargetProcessHandle OPTIONAL,
	OUT PHANDLE TargetHandle OPTIONAL,
	IN ACCESS_MASK DesiredAccess,
	IN ULONG HandleAttributes,
	IN ULONG Options
	);

typedef NTSTATUS(NTAPI *tNtMakeTemporaryObject)(
	IN HANDLE Handle
	);

typedef NTSTATUS(NTAPI *tNtMakePermanentObject)(
	IN HANDLE Handle
    );

typedef NTSTATUS(NTAPI *tNtSignalAndWaitForSingleObject)(
    IN HANDLE SignalHandle,
    IN HANDLE WaitHandle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

typedef NTSTATUS(NTAPI *tNtWaitForSingleObject)(
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

typedef NTSTATUS(NTAPI *tNtWaitForMultipleObjects)(
    IN ULONG Count,
	IN HANDLE Handles[],
    IN WAIT_TYPE WaitType,
    IN BOOLEAN Alertable,
	IN PLARGE_INTEGER Timeout OPTIONAL
    );

#if (NTDDI_VERSION >= NTDDI_WS03)
typedef NTSTATUS(NTAPI *tNtWaitForMultipleObjects32)(
	IN ULONG Count,
	IN LONG Handles[],
	IN WAIT_TYPE WaitType,
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER Timeout OPTIONAL
    );
#endif

typedef NTSTATUS(NTAPI *tNtSetSecurityObject)(
    IN HANDLE Handle,
    IN SECURITY_INFORMATION SecurityInformation,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor
    );

typedef NTSTATUS(NTAPI *tNtQuerySecurityObject)(
    IN HANDLE Handle,
    IN SECURITY_INFORMATION SecurityInformation,
    IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor OPTIONAL,
	IN ULONG Length,
    OUT PULONG LengthNeeded
    );

typedef NTSTATUS(NTAPI *tNtClose)(
    IN HANDLE Handle
    );

#if (NTDDI_VERSION >= NTDDI_WIN10)
typedef NTSTATUS(NTAPI *tNtCompareObjects)(
    IN HANDLE FirstObjectHandle,
    IN HANDLE SecondObjectHandle
    );
#endif

typedef NTSTATUS(NTAPI *tNtCreateSymbolicLinkObject)(
	OUT PHANDLE LinkHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PUNICODE_STRING LinkTarget
    );

typedef NTSTATUS(NTAPI *tNtOpenSymbolicLinkObject)(
	OUT PHANDLE LinkHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef NTSTATUS(NTAPI *tNtQuerySymbolicLinkObject)(
    IN HANDLE LinkHandle,
    IN OUT PUNICODE_STRING LinkTarget,
    OUT PULONG ReturnedLength OPTIONAL
    );

