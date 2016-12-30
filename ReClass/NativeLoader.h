#pragma once

#include "NativeCommon.h"
#include "NativeRtl.h"

///
/// < Loader Definitions >
///

#define LDR_DLL_NOTIFICATION_REASON_LOADED   1
#define LDR_DLL_NOTIFICATION_REASON_UNLOADED 2


///
/// < Loader Enums >
///

enum _LDR_DLL_LOAD_REASON
{
	LoadReasonStaticDependency = 0,
	LoadReasonStaticForwarderDependency = 1,
	LoadReasonDynamicForwarderDependency = 2,
	LoadReasonDelayloadDependency = 3,
	LoadReasonDynamicLoad = 4,
	LoadReasonAsImageLoad = 5,
	LoadReasonAsDataLoad = 6,
	LoadReasonUnknown = -1
};


///
/// < Loader Structures >
///

typedef struct _LDR_DATA_TABLE_ENTRY32
{
	LIST_ENTRY32 InLoadOrderLinks;
	LIST_ENTRY32 InMemoryOrderLinks;
	LIST_ENTRY32 InInitializationOrderLinks;
	ULONG DllBase; // Ptr PVOID
	ULONG EntryPoint; // Ptr PVOID
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	union
	{
		UCHAR FlagGroup[4]; // 0x68
		ULONG Flags; // 0x68
		struct
		{
			ULONG PackagedBinary : 1; // 0x68
			ULONG MarkedForRemoval : 1; // 0x68
			ULONG ImageDll : 1; // 0x68
			ULONG LoadNotificationsSent : 1; // 0x68
			ULONG TelemetryEntryProcessed : 1; // 0x68
			ULONG ProcessStaticImport : 1; // 0x68
			ULONG InLegacyLists : 1; // 0x68
			ULONG InIndexes : 1; // 0x68
			ULONG ShimDll : 1; // 0x68
			ULONG InExceptionTable : 1; // 0x68
			ULONG ReservedFlags1 : 2; // 0x68
			ULONG LoadInProgress : 1; // 0x68
			ULONG LoadConfigProcessed : 1; // 0x68
			ULONG EntryProcessed : 1; // 0x68
			ULONG ProtectDelayLoad : 1; // 0x68
			ULONG ReservedFlags3 : 2; // 0x68
			ULONG DontCallForThreads : 1; // 0x68
			ULONG ProcessAttachCalled : 1; // 0x68
			ULONG ProcessAttachFailed : 1; // 0x68
			ULONG CorDeferredValidate : 1; // 0x68
			ULONG CorImage : 1; // 0x68
			ULONG DontRelocate : 1; // 0x68
			ULONG CorILOnly : 1; // 0x68
			ULONG ReservedFlags5 : 3; // 0x68
			ULONG Redirected : 1; // 0x68
			ULONG ReservedFlags6 : 2; // 0x68
			ULONG CompatDatabaseProcessed : 1; // 0x68
		};
	};
	USHORT ObsoleteLoadCount;
	USHORT TlsIndex;
	LIST_ENTRY32 HashLinks;
	ULONG TimeDateStamp;
	ULONG EntryPointActivationContext; // Ptr struct _ACTIVATION_CONTEXT *
	ULONG Lock; // Ptr PVOID
	ULONG DdagNode; // Ptr PLDR_DDAG_NODE
	LIST_ENTRY32 NodeModuleLink;
	ULONG LoadContext; // Ptr struct _LDRP_LOAD_CONTEXT *
	ULONG ParentDllBase; // Ptr PVOID
	ULONG SwitchBackContext; // Ptr PVOID
	RTL_BALANCED_NODE32 BaseAddressIndexNode;
	RTL_BALANCED_NODE32 MappingInfoIndexNode;
	ULONG OriginalBase; // ULONG_PTR
	LARGE_INTEGER LoadTime;
	ULONG BaseNameHashValue;
	enum _LDR_DLL_LOAD_REASON LoadReason;
	ULONG ImplicitPathOptions;
	ULONG ReferenceCount;
} LDR_DATA_TABLE_ENTRY32, *PLDR_DATA_TABLE_ENTRY32;

typedef struct _LDR_DATA_TABLE_ENTRY64
{
	LIST_ENTRY64 InLoadOrderLinks; // 0x0
	LIST_ENTRY64 InMemoryOrderLinks; // 0x10
	LIST_ENTRY64 InInitializationOrderLinks; // 0x20
	ULONG64 DllBase; // 0x30 PVOID
	ULONG64 EntryPoint; // 0x38 PVOID
	ULONG SizeOfImage; // 0x40
	UNICODE_STRING64 FullDllName; // 0x48
	UNICODE_STRING64 BaseDllName; // 0x58
	union
	{
		UCHAR FlagGroup[4]; // 0x68
		ULONG Flags; // 0x68
		struct
		{
			ULONG PackagedBinary : 1; // 0x68
			ULONG MarkedForRemoval : 1; // 0x68
			ULONG ImageDll : 1; // 0x68
			ULONG LoadNotificationsSent : 1; // 0x68
			ULONG TelemetryEntryProcessed : 1; // 0x68
			ULONG ProcessStaticImport : 1; // 0x68
			ULONG InLegacyLists : 1; // 0x68
			ULONG InIndexes : 1; // 0x68
			ULONG ShimDll : 1; // 0x68
			ULONG InExceptionTable : 1; // 0x68
			ULONG ReservedFlags1 : 2; // 0x68
			ULONG LoadInProgress : 1; // 0x68
			ULONG LoadConfigProcessed : 1; // 0x68
			ULONG EntryProcessed : 1; // 0x68
			ULONG ProtectDelayLoad : 1; // 0x68
			ULONG ReservedFlags3 : 2; // 0x68
			ULONG DontCallForThreads : 1; // 0x68
			ULONG ProcessAttachCalled : 1; // 0x68
			ULONG ProcessAttachFailed : 1; // 0x68
			ULONG CorDeferredValidate : 1; // 0x68
			ULONG CorImage : 1; // 0x68
			ULONG DontRelocate : 1; // 0x68
			ULONG CorILOnly : 1; // 0x68
			ULONG ReservedFlags5 : 3; // 0x68
			ULONG Redirected : 1; // 0x68
			ULONG ReservedFlags6 : 2; // 0x68
			ULONG CompatDatabaseProcessed : 1; // 0x68
		};
	};
	USHORT ObsoleteLoadCount; // 0x6C
	USHORT TlsIndex; // 0x6E
	LIST_ENTRY64 HashLinks; // 0x70
	ULONG TimeDateStamp; // 0x80
	ULONG64 EntryPointActivationContext; // 0x88 _ACTIVATION_CONTEXT*
	PVOID Lock; // 0x90
	ULONG64 DdagNode; // 0x98 _LDR_DDAG_NODE*
	LIST_ENTRY64 NodeModuleLink; // 0xA0
	ULONG64 LoadContext; // 0xB0 _LDRP_LOAD_CONTEXT*
	PVOID ParentDllBase; // 0xB8
	PVOID SwitchBackContext; // 0xC0
	RTL_BALANCED_NODE64 BaseAddressIndexNode; // 0xC8
	RTL_BALANCED_NODE64 MappingInfoIndexNode; // 0xE0
	ULONG64 OriginalBase; // 0xF8
	LARGE_INTEGER LoadTime; // 0x100
	ULONG BaseNameHashValue; // 0x108
	enum _LDR_DLL_LOAD_REASON LoadReason; // 0x10C
	ULONG ImplicitPathOptions; // 0x110
	ULONG ReferenceCount; // 0x114
} LDR_DATA_TABLE_ENTRY64, *PLDR_DATA_TABLE_ENTRY64;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	struct _LIST_ENTRY InLoadOrderLinks; // 0x0
	struct _LIST_ENTRY InMemoryOrderLinks; // 0x10
	struct _LIST_ENTRY InInitializationOrderLinks; // 0x20
	PVOID DllBase; // 0x30
	PVOID EntryPoint; // 0x38
	ULONG SizeOfImage; // 0x40
	struct _UNICODE_STRING FullDllName; // 0x48
	struct _UNICODE_STRING BaseDllName; // 0x58
	union
	{
		UCHAR FlagGroup[4]; // 0x68
		ULONG Flags; // 0x68
		struct
		{
			ULONG PackagedBinary : 1; // 0x68
			ULONG MarkedForRemoval : 1; // 0x68
			ULONG ImageDll : 1; // 0x68
			ULONG LoadNotificationsSent : 1; // 0x68
			ULONG TelemetryEntryProcessed : 1; // 0x68
			ULONG ProcessStaticImport : 1; // 0x68
			ULONG InLegacyLists : 1; // 0x68
			ULONG InIndexes : 1; // 0x68
			ULONG ShimDll : 1; // 0x68
			ULONG InExceptionTable : 1; // 0x68
			ULONG ReservedFlags1 : 2; // 0x68
			ULONG LoadInProgress : 1; // 0x68
			ULONG LoadConfigProcessed : 1; // 0x68
			ULONG EntryProcessed : 1; // 0x68
			ULONG ProtectDelayLoad : 1; // 0x68
			ULONG ReservedFlags3 : 2; // 0x68
			ULONG DontCallForThreads : 1; // 0x68
			ULONG ProcessAttachCalled : 1; // 0x68
			ULONG ProcessAttachFailed : 1; // 0x68
			ULONG CorDeferredValidate : 1; // 0x68
			ULONG CorImage : 1; // 0x68
			ULONG DontRelocate : 1; // 0x68
			ULONG CorILOnly : 1; // 0x68
			ULONG ReservedFlags5 : 3; // 0x68
			ULONG Redirected : 1; // 0x68
			ULONG ReservedFlags6 : 2; // 0x68
			ULONG CompatDatabaseProcessed : 1; // 0x68
		};
	};
	USHORT ObsoleteLoadCount; // 0x6C
	USHORT TlsIndex; // 0x6E
	struct _LIST_ENTRY HashLinks; // 0x70
	ULONG TimeDateStamp; // 0x80
	PVOID EntryPointActivationContext; // 0x88
	PVOID Lock; // 0x90
	PVOID DdagNode; // 0x98
	struct _LIST_ENTRY NodeModuleLink; // 0xA0
	PVOID LoadContext; // 0xB0
	PVOID ParentDllBase; // 0xB8
	PVOID SwitchBackContext; // 0xC0
	struct _RTL_BALANCED_NODE BaseAddressIndexNode; // 0xC8
	struct _RTL_BALANCED_NODE MappingInfoIndexNode; // 0xE0
	ULONG64 OriginalBase; // 0xF8
	LARGE_INTEGER LoadTime; // 0x100
	ULONG BaseNameHashValue; // 0x108
	enum _LDR_DLL_LOAD_REASON LoadReason; // 0x10C
	ULONG ImplicitPathOptions; // 0x110
	ULONG ReferenceCount; // 0x114
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;
typedef LDR_DATA_TABLE_ENTRY *PCLDR_DATA_TABLE_ENTRY;

typedef struct _LDR_DLL_LOADED_NOTIFICATION_DATA
{
	ULONG Flags;                    //Reserved.
	PCUNICODE_STRING FullDllName;   //The full path name of the DLL module.
	PCUNICODE_STRING BaseDllName;   //The base file name of the DLL module.
	PVOID DllBase;                  //A pointer to the base address for the DLL in memory.
	ULONG SizeOfImage;              //The size of the DLL image, in bytes.
} LDR_DLL_LOADED_NOTIFICATION_DATA, *PLDR_DLL_LOADED_NOTIFICATION_DATA;

typedef struct _LDR_DLL_UNLOADED_NOTIFICATION_DATA
{
	ULONG Flags;                    //Reserved.
	PCUNICODE_STRING FullDllName;   //The full path name of the DLL module.
	PCUNICODE_STRING BaseDllName;   //The base file name of the DLL module.
	PVOID DllBase;                  //A pointer to the base address for the DLL in memory.
	ULONG SizeOfImage;              //The size of the DLL image, in bytes.
} LDR_DLL_UNLOADED_NOTIFICATION_DATA, *PLDR_DLL_UNLOADED_NOTIFICATION_DATA;

typedef union _LDR_DLL_NOTIFICATION_DATA
{
	struct _LDR_DLL_LOADED_NOTIFICATION_DATA Loaded;
	struct _LDR_DLL_UNLOADED_NOTIFICATION_DATA Unloaded;
} LDR_DLL_NOTIFICATION_DATA, *PLDR_DLL_NOTIFICATION_DATA;
typedef const LDR_DLL_NOTIFICATION_DATA *PCLDR_DLL_NOTIFICATION_DATA;


/// 
/// < Loader Routines >
/// 

typedef VOID( NTAPI *PLDR_LOADED_MODULE_ENUMERATION_CALLBACK_FUNCTION )(
	IN PLDR_DATA_TABLE_ENTRY DataTableEntry,
	IN PVOID Context,
	IN OUT BOOLEAN* StopEnumeration
	);

typedef VOID( CALLBACK *PLDR_DLL_NOTIFICATION_FUNCTION )(
	IN ULONG NotificationReason,
	IN PLDR_DLL_NOTIFICATION_DATA NotificationData,
	IN PVOID Context OPTIONAL
	);

typedef NTSTATUS(NTAPI *tLdrAccessResource)(
	IN PVOID DllHandle,
	IN CONST IMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry,
	OUT PVOID *Address OPTIONAL,
	OUT PULONG Size OPTIONAL
	);

typedef NTSTATUS(NTAPI *tLdrAddRefDll)(
	IN ULONG Flags,
	IN PVOID DllHandle
	);

typedef NTSTATUS(NTAPI *tLdrEnumerateLoadedModules)(
	IN ULONG Flags OPTIONAL,
	IN PLDR_LOADED_MODULE_ENUMERATION_CALLBACK_FUNCTION CallbackFunction,
	IN PVOID Context OPTIONAL
	);

typedef NTSTATUS(NTAPI *tLdrFindResource_U)(
	IN PVOID DllHandle,
	IN CONST ULONG_PTR* ResourceIdPath,
	IN ULONG ResourceIdPathLength,
	OUT PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry
	);

typedef NTSTATUS(NTAPI *tLdrFindEntryForAddress)(
	IN PVOID Address,
	OUT PLDR_DATA_TABLE_ENTRY *TableEntry
	);

typedef NTSTATUS(NTAPI *tLdrGetDllHandle)(
	IN PCWSTR DllPath OPTIONAL,
	IN PULONG DllCharacteristics OPTIONAL,
	IN PCUNICODE_STRING DllName,
	OUT PVOID *DllHandle
	);

typedef NTSTATUS(NTAPI *tLdrGetProcedureAddress)(
	IN PVOID DllHandle,
	IN CONST ANSI_STRING* ProcedureName OPTIONAL,
	IN ULONG ProcedureNumber OPTIONAL,
	OUT PVOID* ProcedureAddress
	);

typedef NTSTATUS(NTAPI *tLdrLoadDll)(
	IN PCWSTR DllPath OPTIONAL,
	IN PULONG DllCharacteristics OPTIONAL,
	IN PCUNICODE_STRING DllName,
	OUT PVOID *DllHandle
	);

typedef NTSTATUS(NTAPI *tLdrQueryProcessModuleInformation)(
	OUT PRTL_PROCESS_MODULES ModuleInformation,
	IN ULONG ModuleInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

typedef NTSTATUS(NTAPI *tLdrUnloadDll)(
	IN PVOID DllHandle
	);

typedef NTSTATUS(NTAPI *tLdrRegisterDllNotification)(
    IN ULONG Flags,
    IN PLDR_DLL_NOTIFICATION_FUNCTION NotificationFunction,
    IN PVOID Context OPTIONAL,
    OUT PVOID* Cookie
    );

typedef NTSTATUS(NTAPI *tLdrUnregisterDllNotification)(
    IN PVOID Cookie
    );



