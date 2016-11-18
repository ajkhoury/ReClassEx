#pragma once

#include <Windows.h>

#ifndef _NTDEF_
typedef _Return_type_success_( return >= 0 ) LONG NTSTATUS;
typedef NTSTATUS *PNTSTATUS;
#endif

#define NT_SUCCESS(x) ((x) >= 0)
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

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation, // q: SYSTEM_BASIC_INFORMATION
	SystemProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
	SystemPerformanceInformation, // q: SYSTEM_PERFORMANCE_INFORMATION
	SystemTimeOfDayInformation, // q: SYSTEM_TIMEOFDAY_INFORMATION
	SystemPathInformation, // not implemented
	SystemProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
	SystemCallCountInformation, // q: SYSTEM_CALL_COUNT_INFORMATION
	SystemDeviceInformation, // q: SYSTEM_DEVICE_INFORMATION
	SystemProcessorPerformanceInformation, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
	SystemFlagsInformation, // q: SYSTEM_FLAGS_INFORMATION
	SystemCallTimeInformation, // not implemented // 10
	SystemModuleInformation, // q: RTL_PROCESS_MODULES
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation, // not implemented
	SystemNonPagedPoolInformation, // not implemented
	SystemHandleInformation, // q: SYSTEM_HANDLE_INFORMATION
	SystemObjectInformation, // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
	SystemPageFileInformation, // q: SYSTEM_PAGEFILE_INFORMATION
	SystemVdmInstemulInformation, // q
	SystemVdmBopInformation, // not implemented // 20
	SystemFileCacheInformation, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
	SystemPoolTagInformation, // q: SYSTEM_POOLTAG_INFORMATION
	SystemInterruptInformation, // q: SYSTEM_INTERRUPT_INFORMATION
	SystemDpcBehaviorInformation, // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
	SystemFullMemoryInformation, // not implemented
	SystemLoadGdiDriverInformation, // s (kernel-mode only)
	SystemUnloadGdiDriverInformation, // s (kernel-mode only)
	SystemTimeAdjustmentInformation, // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
	SystemSummaryMemoryInformation, // not implemented
	SystemMirrorMemoryInformation, // s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege) // 30
	SystemPerformanceTraceInformation, // s
	SystemObsolete0, // not implemented
	SystemExceptionInformation, // q: SYSTEM_EXCEPTION_INFORMATION
	SystemCrashDumpStateInformation, // s (requires SeDebugPrivilege)
	SystemKernelDebuggerInformation, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
	SystemContextSwitchInformation, // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
	SystemRegistryQuotaInformation, // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
	SystemExtendServiceTableInformation, // s (requires SeLoadDriverPrivilege) // loads win32k only
	SystemPrioritySeperation, // s (requires SeTcbPrivilege)
	SystemVerifierAddDriverInformation, // s (requires SeDebugPrivilege) // 40
	SystemVerifierRemoveDriverInformation, // s (requires SeDebugPrivilege)
	SystemProcessorIdleInformation, // q: SYSTEM_PROCESSOR_IDLE_INFORMATION
	SystemLegacyDriverInformation, // q: SYSTEM_LEGACY_DRIVER_INFORMATION
	SystemCurrentTimeZoneInformation, // q
	SystemLookasideInformation, // q: SYSTEM_LOOKASIDE_INFORMATION
	SystemTimeSlipNotification, // s (requires SeSystemtimePrivilege)
	SystemSessionCreate, // not implemented
	SystemSessionDetach, // not implemented
	SystemSessionInformation, // not implemented
	SystemRangeStartInformation, // q // 50
	SystemVerifierInformation, // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
	SystemVerifierThunkExtend, // s (kernel-mode only)
	SystemSessionProcessInformation, // q: SYSTEM_SESSION_PROCESS_INFORMATION
	SystemLoadGdiDriverInSystemSpace, // s (kernel-mode only) (same as SystemLoadGdiDriverInformation)
	SystemNumaProcessorMap, // q
	SystemPrefetcherInformation, // q: PREFETCHER_INFORMATION; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
	SystemExtendedProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
	SystemRecommendedSharedDataAlignment, // q
	SystemComPlusPackage, // q; s
	SystemNumaAvailableMemory, // 60
	SystemProcessorPowerInformation, // q: SYSTEM_PROCESSOR_POWER_INFORMATION
	SystemEmulationBasicInformation, // q
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation, // q: SYSTEM_HANDLE_INFORMATION_EX
	SystemLostDelayedWriteInformation, // q: ULONG
	SystemBigPoolInformation, // q: SYSTEM_BIGPOOL_INFORMATION
	SystemSessionPoolTagInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION
	SystemSessionMappedViewInformation, // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
	SystemHotpatchInformation, // q; s
	SystemObjectSecurityMode, // q // 70
	SystemWatchdogTimerHandler, // s (kernel-mode only)
	SystemWatchdogTimerInformation, // q (kernel-mode only); s (kernel-mode only)
	SystemLogicalProcessorInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION
	SystemWow64SharedInformationObsolete, // not implemented
	SystemRegisterFirmwareTableInformationHandler, // s (kernel-mode only)
	SystemFirmwareTableInformation, // not implemented
	SystemModuleInformationEx, // q: RTL_PROCESS_MODULE_INFORMATION_EX
	SystemVerifierTriageInformation, // not implemented
	SystemSuperfetchInformation, // q: SUPERFETCH_INFORMATION; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
	SystemMemoryListInformation, // q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege) // 80
	SystemFileCacheInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
	SystemThreadPriorityClientIdInformation, // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege)
	SystemProcessorIdleCycleTimeInformation, // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[]
	SystemVerifierCancellationInformation, // not implemented // name:wow64:whNT32QuerySystemVerifierCancellationInformation
	SystemProcessorPowerInformationEx, // not implemented
	SystemRefTraceInformation, // q; s // ObQueryRefTraceInformation
	SystemSpecialPoolInformation, // q; s (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
	SystemProcessIdInformation, // q: SYSTEM_PROCESS_ID_INFORMATION
	SystemErrorPortInformation, // s (requires SeTcbPrivilege)
	SystemBootEnvironmentInformation, // q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION // 90
	SystemHypervisorInformation, // q; s (kernel-mode only)
	SystemVerifierInformationEx, // q; s
	SystemTimeZoneInformation, // s (requires SeTimeZonePrivilege)
	SystemImageFileExecutionOptionsInformation, // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
	SystemCoverageInformation, // q; s // name:wow64:whNT32QuerySystemCoverageInformation; ExpCovQueryInformation
	SystemPrefetchPatchInformation, // not implemented
	SystemVerifierFaultsInformation, // s (requires SeDebugPrivilege)
	SystemSystemPartitionInformation, // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
	SystemSystemDiskInformation, // q: SYSTEM_SYSTEM_DISK_INFORMATION
	SystemProcessorPerformanceDistribution, // q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION // 100
	SystemNumaProximityNodeInformation, // q
	SystemDynamicTimeZoneInformation, // q; s (requires SeTimeZonePrivilege)
	SystemCodeIntegrityInformation, // q // SeCodeIntegrityQueryInformation
	SystemProcessorMicrocodeUpdateInformation, // s
	SystemProcessorBrandString, // q // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
	SystemVirtualAddressInformation, // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
	SystemLogicalProcessorAndGroupInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // since WIN7 // KeQueryLogicalProcessorRelationship
	SystemProcessorCycleTimeInformation, // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[]
	SystemStoreInformation, // q; s // SmQueryStoreInformation
	SystemRegistryAppendString, // s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS // 110
	SystemAitSamplingValue, // s: ULONG (requires SeProfileSingleProcessPrivilege)
	SystemVhdBootInformation, // q: SYSTEM_VHD_BOOT_INFORMATION
	SystemCpuQuotaInformation, // q; s // PsQueryCpuQuotaInformation
	SystemNativeBasicInformation, // not implemented
	SystemSpare1, // not implemented
	SystemLowPriorityIoInformation, // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
	SystemTpmBootEntropyInformation, // q: TPM_BOOT_ENTROPY_NT_RESULT // ExQueryTpmBootEntropyInformation
	SystemVerifierCountersInformation, // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
	SystemPagedPoolInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
	SystemSystemPtesInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes) // 120
	SystemNodeDistanceInformation, // q
	SystemAcpiAuditInformation, // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
	SystemBasicPerformanceInformation, // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
	SystemQueryPerformanceCounterInformation, // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
	SystemSessionBigPoolInformation, // since WIN8
	SystemBootGraphicsInformation,
	SystemScrubPhysicalMemoryInformation,
	SystemBadPageInformation,
	SystemProcessorProfileControlArea,
	SystemCombinePhysicalMemoryInformation, // 130
	SystemEntropyInterruptTimingCallback,
	SystemConsoleInformation,
	SystemPlatformBinaryInformation,
	SystemThrottleNotificationInformation,
	SystemHypervisorProcessorCountInformation,
	SystemDeviceDataInformation,
	SystemDeviceDataEnumerationInformation,
	SystemMemoryTopologyInformation,
	SystemMemoryChannelInformation,
	SystemBootLogoInformation, // 140
	SystemProcessorPerformanceInformationEx, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX // since WINBLUE
	SystemSpare0,
	SystemSecureBootPolicyInformation,
	SystemPageFileInformationEx, // q: SYSTEM_PAGEFILE_INFORMATION_EX
	SystemSecureBootInformation,
	SystemEntropyInterruptTimingRawInformation,
	SystemPortableWorkspaceEfiLauncherInformation,
	SystemFullProcessInformation, // q: SYSTEM_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
	SystemKernelDebuggerInformationEx, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
	SystemBootMetadataInformation, // 150
	SystemSoftRebootInformation,
	SystemElamCertificateInformation,
	SystemOfflineDumpConfigInformation,
	SystemProcessorFeaturesInformation, // q: SYSTEM_PROCESSOR_FEATURES_INFORMATION
	SystemRegistryReconciliationInformation,
	SystemEdidInformation,
	SystemManufacturingInformation, // q: SYSTEM_MANUFACTURING_INFORMATION // since THRESHOLD
	SystemEnergyEstimationConfigInformation, // q: SYSTEM_ENERGY_ESTIMATION_CONFIG_INFORMATION
	SystemHypervisorDetailInformation, // q: SYSTEM_HYPERVISOR_DETAIL_INFORMATION
	SystemProcessorCycleStatsInformation, // q: SYSTEM_PROCESSOR_CYCLE_STATS_INFORMATION // 160
	SystemVmGenerationCountInformation,
	SystemTrustedPlatformModuleInformation, // q: SYSTEM_TPM_INFORMATION
	SystemKernelDebuggerFlags,
	SystemCodeIntegrityPolicyInformation,
	SystemIsolatedUserModeInformation,
	SystemHardwareSecurityTestInterfaceResultsInformation,
	SystemSingleModuleInformation, // q: SYSTEM_SINGLE_MODULE_INFORMATION
	SystemAllowedCpuSetsInformation,
	SystemDmaProtectionInformation, // q: SYSTEM_DMA_PROTECTION_INFORMATION
	SystemInterruptCpuSetsInformation,
	SystemSecureBootPolicyFullInformation,
	SystemCodeIntegrityPolicyFullInformation,
	SystemAffinitizedInterruptProcessorInformation,
	SystemRootSiloInformation, // q: SYSTEM_ROOT_SILO_INFORMATION
	SystemCpuSetInformation, // q: SYSTEM_CPU_SET_INFORMATION // since THRESHOLD2
	SystemCpuSetTagInformation, // q: SYSTEM_CPU_SET_TAG_INFORMATION
	SystemWin32WerStartCallout,
	SystemSecureKernelProfileInformation,
	MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;

typedef enum _tagPROCESSINFOCLASS
{
	ProcessBasicInformation = 0, // 0, q: PROCESS_BASIC_INFORMATION, PROCESS_EXTENDED_BASIC_INFORMATION
	ProcessQuotaLimits, // qs: QUOTA_LIMITS, QUOTA_LIMITS_EX
	ProcessIoCounters, // q: IO_COUNTERS
	ProcessVmCounters, // q: VM_COUNTERS, VM_COUNTERS_EX
	ProcessTimes, // q: KERNEL_USER_TIMES
	ProcessBasePriority, // s: KPRIORITY
	ProcessRaisePriority, // s: ULONG
	ProcessDebugPort, // q: HANDLE
	ProcessExceptionPort, // s: HANDLE
	ProcessAccessToken, // s: PROCESS_ACCESS_TOKEN
	ProcessLdtInformation, // 10
	ProcessLdtSize,
	ProcessDefaultHardErrorMode, // qs: ULONG
	ProcessIoPortHandlers, // (kernel-mode only)
	ProcessPooledUsageAndLimits, // q: POOLED_USAGE_AND_LIMITS
	ProcessWorkingSetWatch, // q: PROCESS_WS_WATCH_INFORMATION[]; s: void
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup, // s: BOOLEAN
	ProcessPriorityClass, // qs: PROCESS_PRIORITY_CLASS
	ProcessWx86Information,
	ProcessHandleCount, // 20, q: ULONG, PROCESS_HANDLE_INFORMATION
	ProcessAffinityMask, // s: KAFFINITY
	ProcessPriorityBoost, // qs: ULONG
	ProcessDeviceMap, // qs: PROCESS_DEVICEMAP_INFORMATION, PROCESS_DEVICEMAP_INFORMATION_EX
	ProcessSessionInformation, // q: PROCESS_SESSION_INFORMATION
	ProcessForegroundInformation, // s: PROCESS_FOREGROUND_BACKGROUND
	ProcessWow64Information, // q: ULONG_PTR
	ProcessImageFileName, // q: UNICODE_STRING
	ProcessLUIDDeviceMapsEnabled, // q: ULONG
	ProcessBreakOnTermination, // qs: ULONG
	ProcessDebugObjectHandle, // 30, q: HANDLE
	ProcessDebugFlags, // qs: ULONG
	ProcessHandleTracing, // q: PROCESS_HANDLE_TRACING_QUERY; s: size 0 disables, otherwise enables
	ProcessIoPriority, // qs: ULONG
	ProcessExecuteFlags, // qs: ULONG
	ProcessResourceManagement,
	ProcessCookie, // q: ULONG
	ProcessImageInformation, // q: SECTION_IMAGE_INFORMATION
	ProcessCycleTime, // q: PROCESS_CYCLE_TIME_INFORMATION
	ProcessPagePriority, // q: ULONG
	ProcessInstrumentationCallback, // 40
	ProcessThreadStackAllocation, // s: PROCESS_STACK_ALLOCATION_INFORMATION, PROCESS_STACK_ALLOCATION_INFORMATION_EX
	ProcessWorkingSetWatchEx, // q: PROCESS_WS_WATCH_INFORMATION_EX[]
	ProcessImageFileNameWin32, // q: UNICODE_STRING
	ProcessImageFileMapping, // q: HANDLE (input)
	ProcessAffinityUpdateMode, // qs: PROCESS_AFFINITY_UPDATE_MODE
	ProcessMemoryAllocationMode, // qs: PROCESS_MEMORY_ALLOCATION_MODE
	ProcessGroupInformation, // q: USHORT[]
	ProcessTokenVirtualizationEnabled, // s: ULONG
	ProcessConsoleHostProcess, // q: ULONG_PTR
	ProcessWindowInformation, // 50, q: PROCESS_WINDOW_INFORMATION
	ProcessHandleInformation, // q: PROCESS_HANDLE_SNAPSHOT_INFORMATION // since WIN8
	ProcessMitigationPolicy, // s: PROCESS_MITIGATION_POLICY_INFORMATION
	ProcessDynamicFunctionTableInformation,
	ProcessHandleCheckingMode,
	ProcessKeepAliveCount, // q: PROCESS_KEEPALIVE_COUNT_INFORMATION
	ProcessRevokeFileHandles, // s: PROCESS_REVOKE_FILE_HANDLES_INFORMATION
	MaxProcessInfoClass
} PROCESSINFOCLASS;

typedef enum _KWAIT_REASON
{
	Executive,
	FreePage,
	PageIn,
	PoolAllocation,
	DelayExecution,
	Suspended,
	UserRequest,
	WrExecutive,
	WrFreePage,
	WrPageIn,
	WrPoolAllocation,
	WrDelayExecution,
	WrSuspended,
	WrUserRequest,
	WrEventPair,
	WrQueue,
	WrLpcReceive,
	WrLpcReply,
	WrVirtualMemory,
	WrPageOut,
	WrRendezvous,
	WrKeyedEvent,
	WrTerminated,
	WrProcessInSwap,
	WrCpuRateControl,
	WrCalloutStack,
	WrKernel,
	WrResource,
	WrPushLock,
	WrMutex,
	WrQuantumEnd,
	WrDispatchInt,
	WrPreempted,
	WrYieldExecution,
	WrFastMutex,
	WrGuardedMutex,
	WrRundown,
	WrAlertByThreadId,
	WrDeferredPreempt,
	MaximumWaitReason
} KWAIT_REASON, *PKWAIT_REASON;

typedef enum _PS_ATTRIBUTE_NUM
{
	PsAttributeParentProcess, // in HANDLE
	PsAttributeDebugPort, // in HANDLE
	PsAttributeToken, // in HANDLE
	PsAttributeClientId, // out PCLIENT_ID
	PsAttributeTebAddress, // out PTEB
	PsAttributeImageName, // in PWSTR
	PsAttributeImageInfo, // out PSECTION_IMAGE_INFORMATION
	PsAttributeMemoryReserve, // in PPS_MEMORY_RESERVE
	PsAttributePriorityClass, // in UCHAR
	PsAttributeErrorMode, // in ULONG
	PsAttributeStdHandleInfo, // 10, in PPS_STD_HANDLE_INFO
	PsAttributeHandleList, // in PHANDLE
	PsAttributeGroupAffinity, // in PGROUP_AFFINITY
	PsAttributePreferredNode, // in PUSHORT
	PsAttributeIdealProcessor, // in PPROCESSOR_NUMBER
	PsAttributeUmsThread, // see UpdateProceThreadAttributeList in msdn (CreateProcessA/W...) in PUMS_CREATE_THREAD_ATTRIBUTES
	PsAttributeMitigationOptions, // in UCHAR
	PsAttributeProtectionLevel,
	PsAttributeSecureProcess, // since THRESHOLD (Virtual Secure Mode, Device Guard)
	PsAttributeJobList,
	PsAttributeMax
} PS_ATTRIBUTE_NUM;

typedef struct _ANSI_STRING
{
	USHORT					Length;
	USHORT					MaximumLength;
	PSTR					Buffer;
} ANSI_STRING, *PANSI_STRING;

typedef struct _UNICODE_STRING
{
	USHORT					Length;
	USHORT					MaximumLength;
	PWSTR					Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _STRING32
{
	USHORT   Length;
	USHORT   MaximumLength;
	ULONG  Buffer;
} STRING32;
typedef STRING32 *PSTRING32;
typedef STRING32 UNICODE_STRING32;
typedef UNICODE_STRING32 *PUNICODE_STRING32;

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
	USHORT					Flags;
	USHORT					Length;
	ULONG					TimeStamp;
	UNICODE_STRING			DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG					MaximumLength;
	ULONG					Length;
	ULONG					Flags;
	ULONG					DebugFlags;
	PVOID					ConsoleHandle;
	ULONG					ConsoleFlags;
	HANDLE					StdInputHandle;
	HANDLE					StdOutputHandle;
	HANDLE					StdErrorHandle;
	UNICODE_STRING			CurrentDirectoryPath;
	HANDLE					CurrentDirectoryHandle;
	UNICODE_STRING			DllPath;
	UNICODE_STRING			ImagePathName;
	UNICODE_STRING			CommandLine;
	PVOID					Environment;
	ULONG					StartingPositionLeft;
	ULONG					StartingPositionTop;
	ULONG					Width;
	ULONG					Height;
	ULONG					CharWidth;
	ULONG					CharHeight;
	ULONG					ConsoleTextAttributes;
	ULONG					WindowFlags;
	ULONG					ShowWindowFlags;
	UNICODE_STRING			WindowTitle;
	UNICODE_STRING			DesktopName;
	UNICODE_STRING			ShellInfo;
	UNICODE_STRING			RuntimeData;
	RTL_DRIVE_LETTER_CURDIR	DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef LONG_PTR KPRIORITY;

typedef struct _CLIENT_ID
{
	HANDLE					UniqueProcess;
	HANDLE					UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _RTL_BALANCED_NODE
{
	union
	{
		struct _RTL_BALANCED_NODE *Children[2];
		struct
		{
			struct _RTL_BALANCED_NODE *Left;
			struct _RTL_BALANCED_NODE *Right;
		} DUMMYSTRUCTNAME;
	} DUMMYUNIONNAME;
	#define RTL_BALANCED_NODE_RESERVED_PARENT_MASK 3
	union
	{
		UCHAR Red : 1;
		UCHAR Balance : 2;
		ULONG_PTR ParentValue;
	} DUMMYUNIONNAME2;
} RTL_BALANCED_NODE, *PRTL_BALANCED_NODE;

typedef struct _LDR_DATA_TABLE_ENTRY32
{
	LIST_ENTRY32 InLoadOrderLinks;
	LIST_ENTRY32 InMemoryOrderLinks;
	LIST_ENTRY32 InInitializationOrderLinks;
	ULONG DllBase;
	ULONG EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	LIST_ENTRY32 HashLinks;
	ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY32, *PLDR_DATA_TABLE_ENTRY32;

typedef struct _PEB_LDR_DATA32
{
	ULONG Length; // 0x0
	UCHAR Initialized; // 0x4
	ULONG SsHandle; // 0x8
	LIST_ENTRY32 InLoadOrderModuleList;
	LIST_ENTRY32 InMemoryOrderModuleList;
	LIST_ENTRY32 InInitializationOrderModuleList;
	ULONG EntryInProgress;
	UCHAR ShutdownInProgress;
	ULONG ShutdownThreadId;
} PEB_LDR_DATA32, *PPEB_LDR_DATA32;

typedef struct _PEB32
{
	UCHAR InheritedAddressSpace; // 0x0
	UCHAR ReadImageFileExecOptions; // 0x1
	UCHAR BeingDebugged; // 0x2
	union
	{
		UCHAR BitField; // 0x3
		struct
		{
			UCHAR ImageUsesLargePages : 1; // 0x3
			UCHAR IsProtectedProcess : 1; // 0x3
			UCHAR IsImageDynamicallyRelocated : 1; // 0x3
			UCHAR SkipPatchingUser32Forwarders : 1; // 0x3
			UCHAR IsPackagedProcess : 1; // 0x3
			UCHAR IsAppContainer : 1; // 0x3
			UCHAR IsProtectedProcessLight : 1; // 0x3
			UCHAR SpareBits : 1; // 0x3
		};
	};
	ULONG Mutant; // 0x4
	ULONG ImageBaseAddress; // 0x8
	ULONG Ldr; // 0xC
	ULONG ProcessParameters; // 0x10
	ULONG SubSystemData; // 0x14
	ULONG ProcessHeap; // 0x18
	ULONG FastPebLock; // 0x1C
	ULONG AtlThunkSListPtr; // 0x20
	ULONG IFEOKey; // 0x24
	union
	{
		ULONG CrossProcessFlags; // 0x28
		struct
		{
			ULONG ProcessInJob : 1; // 0x28
			ULONG ProcessInitializing : 1; // 0x28
			ULONG ProcessUsingVEH : 1; // 0x28
			ULONG ProcessUsingVCH : 1; // 0x28
			ULONG ProcessUsingFTH : 1; // 0x28
			ULONG ReservedBits0 : 27; // 0x28
		};
	};
	union
	{
		ULONG KernelCallbackTable; // 0x2C
		ULONG UserSharedInfoPtr; // 0x2C
	};
	ULONG SystemReserved[1]; // 0x30
	ULONG AtlThunkSListPtr32; // 0x34
	ULONG ApiSetMap; // 0x38
	ULONG TlsExpansionCounter; // 0x3C
	ULONG TlsBitmap; // 0x40
	ULONG TlsBitmapBits[2]; // 0x44
	ULONG ReadOnlySharedMemoryBase; // 0x4C
	ULONG SparePvoid0; // 0x50
	ULONG ReadOnlyStaticServerData; // 0x54
	ULONG AnsiCodePageData; // 0x58
	ULONG OemCodePageData; // 0x5C
	ULONG UnicodeCaseTableData; // 0x60
	ULONG NumberOfProcessors; // 0x64
	ULONG NtGlobalFlag; // 0x68
	LARGE_INTEGER CriticalSectionTimeout; // 0x70
	ULONG HeapSegmentReserve; // 0x78
	ULONG HeapSegmentCommit; // 0x7C
	ULONG HeapDeCommitTotalFreeThreshold; // 0x80
	ULONG HeapDeCommitFreeBlockThreshold; // 0x84
	ULONG NumberOfHeaps; // 0x88
	ULONG MaximumNumberOfHeaps; // 0x8C
	ULONG ProcessHeaps; // 0x90
	ULONG GdiSharedHandleTable; // 0x94
	ULONG ProcessStarterHelper; // 0x98
	ULONG GdiDCAttributeList; // 0x9C
	ULONG LoaderLock; // 0xA0
	ULONG OSMajorVersion; // 0xA4
	ULONG OSMinorVersion; // 0xA8
	USHORT OSBuildNumber; // 0xAC
	USHORT OSCSDVersion; // 0xAE
	ULONG OSPlatformId; // 0xB0
	ULONG ImageSubsystem; // 0xB4
	ULONG ImageSubsystemMajorVersion; // 0xB8
	ULONG ImageSubsystemMinorVersion; // 0xBC
	ULONG ActiveProcessAffinityMask; // 0xC0
	ULONG GdiHandleBuffer[34]; // 0xC4
	ULONG PostProcessInitRoutine; // 0x14C
	ULONG TlsExpansionBitmap; // 0x150
	ULONG TlsExpansionBitmapBits[32]; // 0x154
	ULONG SessionId; // 0x1D4
	ULARGE_INTEGER AppCompatFlags; // 0x1D8
	ULARGE_INTEGER AppCompatFlagsUser; // 0x1E0
	ULONG pShimData; // 0x1E8
	ULONG AppCompatInfo; // 0x1EC
	struct _STRING32 CSDVersion; // 0x1F0
	ULONG ActivationContextData; // 0x1F8
	ULONG ProcessAssemblyStorageMap; // 0x1FC
	ULONG SystemDefaultActivationContextData; // 0x200
	ULONG SystemAssemblyStorageMap; // 0x204
	ULONG MinimumStackCommit; // 0x208
	ULONG FlsCallback; // 0x20C
	LIST_ENTRY32 FlsListHead; // 0x210
	ULONG FlsBitmap; // 0x218
	ULONG FlsBitmapBits[4]; // 0x21C
	ULONG FlsHighIndex; // 0x22C
	ULONG WerRegistrationData; // 0x230
	ULONG WerShipAssertPtr; // 0x234
	ULONG pUnused; // 0x238
	ULONG pImageHeaderHash; // 0x23C
	union
	{
		ULONG TracingFlags; // 0x240
		struct
		{
			ULONG HeapTracingEnabled : 1; // 0x240
			ULONG CritSecTracingEnabled : 1; // 0x240
			ULONG LibLoaderTracingEnabled : 1; // 0x240
			ULONG SpareTracingBits : 29; // 0x240
		};
	};
	ULONG64 CsrServerReadOnlySharedMemoryBase; // 0x248
	ULONG TppWorkerpListLock; // 0x250
	LIST_ENTRY32 TppWorkerpList; // 0x254
	ULONG WaitOnAddressHashTable[128]; // 0x25C
} PEB32, *PPEB32;

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

//typedef struct LDR_DATA_ENTRY
//{
//	LIST_ENTRY              InInitializationOrderLinks;
//	PVOID                   BaseAddress;
//	PVOID                   EntryPoint;
//	ULONG                   SizeOfImage;
//	UNICODE_STRING          FullDllName;
//	UNICODE_STRING          BaseDllName;
//	ULONG                   Flags;
//	SHORT                   LoadCount;
//	SHORT                   TlsIndex;
//	LIST_ENTRY              HashTableEntry;
//	ULONG                   TimeDateStamp;
//} LDR_DATA_ENTRY, *PLDR_DATA_ENTRY;

typedef struct _PEB_LDR_DATA
{
	ULONG Length; // 0x0
	UCHAR Initialized; // 0x4
	PVOID SsHandle; // 0x8
	struct _LIST_ENTRY InLoadOrderModuleList; // 0x10
	struct _LIST_ENTRY InMemoryOrderModuleList; // 0x20
	struct _LIST_ENTRY InInitializationOrderModuleList; // 0x30
	PVOID EntryInProgress; // 0x40
	UCHAR ShutdownInProgress; // 0x48
	PVOID ShutdownThreadId; // 0x50
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB64
{
	UCHAR InheritedAddressSpace; // 0x0
	UCHAR ReadImageFileExecOptions; // 0x1
	UCHAR BeingDebugged; // 0x2
	union
	{
		UCHAR BitField; // 0x3
		struct
		{
			UCHAR ImageUsesLargePages : 1; // 0x3
			UCHAR IsProtectedProcess : 1; // 0x3
			UCHAR IsImageDynamicallyRelocated : 1; // 0x3
			UCHAR SkipPatchingUser32Forwarders : 1; // 0x3
			UCHAR IsPackagedProcess : 1; // 0x3
			UCHAR IsAppContainer : 1; // 0x3
			UCHAR IsProtectedProcessLight : 1; // 0x3
			UCHAR SpareBits : 1; // 0x3
		};
	};
	UCHAR Padding0[4]; // 0x4
	PVOID Mutant; // 0x8
	PVOID ImageBaseAddress; // 0x10
	struct _LDR_DATA* Ldr; // 0x18
	struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters; // 0x20
	PVOID SubSystemData; // 0x28
	PVOID ProcessHeap; // 0x30
	struct _RTL_CRITICAL_SECTION* FastPebLock; // 0x38
	PVOID AtlThunkSListPtr; // 0x40
	PVOID IFEOKey; // 0x48
	union
	{
		ULONG CrossProcessFlags; // 0x50
		struct
		{
			ULONG ProcessInJob : 1; // 0x50
			ULONG ProcessInitializing : 1; // 0x50
			ULONG ProcessUsingVEH : 1; // 0x50
			ULONG ProcessUsingVCH : 1; // 0x50
			ULONG ProcessUsingFTH : 1; // 0x50
			ULONG ReservedBits0 : 27; // 0x50
		};
	};
	UCHAR Padding1[4]; // 0x54
	PVOID KernelCallbackTable; // 0x58
	PVOID UserSharedInfoPtr; // 0x58
	ULONG SystemReserved[1]; // 0x60
	ULONG AtlThunkSListPtr32; // 0x64
	PVOID ApiSetMap; // 0x68
	ULONG TlsExpansionCounter; // 0x70
	UCHAR Padding2[4]; // 0x74
	PVOID TlsBitmap; // 0x78
	ULONG TlsBitmapBits[2]; // 0x80
	PVOID ReadOnlySharedMemoryBase; // 0x88
	PVOID SparePvoid0; // 0x90
	PVOID* ReadOnlyStaticServerData; // 0x98
	PVOID AnsiCodePageData; // 0xA0
	PVOID OemCodePageData; // 0xA8
	PVOID UnicodeCaseTableData; // 0xB0
	ULONG NumberOfProcessors; // 0xB8
	ULONG NtGlobalFlag; // 0xBC
	LARGE_INTEGER CriticalSectionTimeout; // 0xC0
	ULONG64 HeapSegmentReserve; // 0xC8
	ULONG64 HeapSegmentCommit; // 0xD0
	ULONG64 HeapDeCommitTotalFreeThreshold; // 0xD8
	ULONG64 HeapDeCommitFreeBlockThreshold; // 0xE0
	ULONG NumberOfHeaps; // 0xE8
	ULONG MaximumNumberOfHeaps; // 0xEC
	PVOID* ProcessHeaps; // 0xF0
	PVOID GdiSharedHandleTable; // 0xF8
	PVOID ProcessStarterHelper; // 0x100
	ULONG GdiDCAttributeList; // 0x108
	UCHAR Padding3[4]; // 0x10C
	struct _RTL_CRITICAL_SECTION* LoaderLock; // 0x110
	ULONG OSMajorVersion; // 0x118
	ULONG OSMinorVersion; // 0x11C
	USHORT OSBuildNumber; // 0x120
	USHORT OSCSDVersion; // 0x122
	ULONG OSPlatformId; // 0x124
	ULONG ImageSubsystem; // 0x128
	ULONG ImageSubsystemMajorVersion; // 0x12C
	ULONG ImageSubsystemMinorVersion; // 0x130
	UCHAR Padding4[4]; // 0x134
	ULONG64 ActiveProcessAffinityMask; // 0x138
	ULONG GdiHandleBuffer[60]; // 0x140
	PVOID PostProcessInitRoutine; // 0x230
	PVOID TlsExpansionBitmap; // 0x238
	ULONG TlsExpansionBitmapBits[32]; // 0x240
	ULONG SessionId; // 0x2C0
	UCHAR Padding5[4]; // 0x2C4
	ULARGE_INTEGER AppCompatFlags; // 0x2C8
	ULARGE_INTEGER AppCompatFlagsUser; // 0x2D0
	PVOID pShimData; // 0x2D8
	PVOID AppCompatInfo; // 0x2E0
	struct _UNICODE_STRING CSDVersion; // 0x2E8
	PVOID ActivationContextData; // 0x2F8
	PVOID ProcessAssemblyStorageMap; // 0x300
	PVOID SystemDefaultActivationContextData; // 0x308
	PVOID SystemAssemblyStorageMap; // 0x310
	ULONG64 MinimumStackCommit; // 0x318
	PVOID FlsCallback; // 0x320
	struct _LIST_ENTRY FlsListHead; // 0x328
	PVOID FlsBitmap; // 0x338
	ULONG FlsBitmapBits[4]; // 0x340
	ULONG FlsHighIndex; // 0x350
	PVOID WerRegistrationData; // 0x358
	PVOID WerShipAssertPtr; // 0x360
	PVOID pUnused; // 0x368
	PVOID pImageHeaderHash; // 0x370
	union
	{
		ULONG TracingFlags; // 0x378
		struct
		{
			ULONG HeapTracingEnabled : 1; // 0x378
			ULONG CritSecTracingEnabled : 1; // 0x378
			ULONG LibLoaderTracingEnabled : 1; // 0x378
			ULONG SpareTracingBits : 29; // 0x378
		};
	};
	UCHAR Padding6[4]; // 0x37C
	ULONG64 CsrServerReadOnlySharedMemoryBase; // 0x380
	ULONG64 TppWorkerpListLock; // 0x388
	struct _LIST_ENTRY TppWorkerpList; // 0x390
	PVOID WaitOnAddressHashTable[128]; // 0x3A0
} PEB64, *PPEB64;

#if defined(_M_X64)
typedef PEB64 PEB, *PPEB;
#else
typedef PEB32 PEB, *PPEB;
#endif

#define NtCurrentPeb( ) ((PPEB)(NtCurrentTeb()->ProcessEnvironmentBlock))

typedef struct _GDI_TEB_BATCH
{
	ULONG Offset;
	HANDLE HDC;
	ULONG Buffer[310];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _VM_COUNTERS
{
	ULONG_PTR	PeakVirtualSize;
	#ifndef _WIN64
	ULONG_PTR	VirtualSize;
	#endif
	ULONG_PTR	PageFaultCount;
	ULONG_PTR	PeakWorkingSetSize;
	ULONG_PTR	WorkingSetSize;
	ULONG_PTR	QuotaPeakPagedPoolUsage;
	ULONG_PTR	QuotaPagedPoolUsage;
	ULONG_PTR	QuotaPeakNonPagedPoolUsage;
	ULONG_PTR	QuotaNonPagedPoolUsage;
	ULONG_PTR	PagefileUsage;
	ULONG_PTR	PeakPagefileUsage;
	#ifdef _WIN64
	ULONG_PTR	VirtualSize;
	#endif
} VM_COUNTERS, *PVM_COUNTERS;

typedef struct _TEB
{
	NT_TIB NtTib;
	PVOID EnvironmentPointer;
	CLIENT_ID ClientId;
	PVOID ActiveRpcHandle;
	PVOID ThreadLocalStoragePointer;
	PVOID ProcessEnvironmentBlock;
	ULONG LastErrorValue;
	ULONG CountOfOwnedCriticalSections;
	PVOID CsrClientThread;
	PVOID Win32ThreadInfo;
	ULONG User32Reserved[26];
	ULONG UserReserved[5];
	PVOID WOW32Reserved;
	LCID CurrentLocale;
	ULONG FpSoftwareStatusRegister;
	PVOID SystemReserved1[54];
	NTSTATUS ExceptionCode;
	PVOID ActivationContextStackPointer;
#ifdef _WIN64
	UCHAR SpareBytes[24];
#else
	UCHAR SpareBytes[36];
#endif
	ULONG TxFsContext;
	GDI_TEB_BATCH GdiTebBatch;
	CLIENT_ID RealClientId;
	HANDLE GdiCachedProcessHandle;
	ULONG GdiClientPID;
	ULONG GdiClientTID;
	PVOID GdiThreadLocalInfo;
	ULONG_PTR Win32ClientInfo[62];
	PVOID glDispatchTable[233];
	ULONG_PTR glReserved1[29];
	PVOID glReserved2;
	PVOID glSectionInfo;
	PVOID glSection;
	PVOID glTable;
	PVOID glCurrentRC;
	PVOID glContext;
	NTSTATUS LastStatusValue;
	UNICODE_STRING StaticUnicodeString;
	WCHAR StaticUnicodeBuffer[261];
	PVOID DeallocationStack;
	PVOID TlsSlots[64];
	LIST_ENTRY TlsLinks;
	PVOID Vdm;
	PVOID ReservedForNtRpc;
	PVOID DbgSsReserved[2];
	ULONG HardErrorMode;
#ifdef _WIN64
	PVOID Instrumentation[11];
#else
	PVOID Instrumentation[9];
#endif
	GUID ActivityId;
	PVOID SubProcessTag;
	PVOID EtwLocalData;
	PVOID EtwTraceData;
	PVOID WinSockData;
	ULONG GdiBatchCount;
	union
	{
		PROCESSOR_NUMBER CurrentIdealProcessor;
		ULONG IdealProcessorValue;
		struct
		{
			UCHAR ReservedPad0;
			UCHAR ReservedPad1;
			UCHAR ReservedPad2;
			UCHAR IdealProcessor;
		};
	};
	ULONG GuaranteedStackBytes;
	PVOID ReservedForPerf;
	PVOID ReservedForOle;
	ULONG WaitingOnLoaderLock;
	PVOID SavedPriorityState;
	ULONG_PTR SoftPatchPtr1;
	PVOID ThreadPoolData;
	PVOID *TlsExpansionSlots;
#ifdef _WIN64
	PVOID DeallocationBStore;
	PVOID BStoreLimit;
#endif
	ULONG MuiGeneration;
	ULONG IsImpersonating;
	PVOID NlsCache;
	PVOID pShimData;
	ULONG HeapVirtualAffinity;
	HANDLE CurrentTransactionHandle;
	PVOID ActiveFrame;
	PVOID FlsData;
	PVOID PreferredLanguages;
	PVOID UserPrefLanguages;
	PVOID MergedPrefLanguages;
	ULONG MuiImpersonation;
	union
	{
		USHORT CrossTebFlags;
		USHORT SpareCrossTebBits : 16;
	};
	union
	{
		USHORT SameTebFlags;
		struct
		{
			USHORT SafeThunkCall : 1;
			USHORT InDebugPrint : 1;
			USHORT HasFiberData : 1;
			USHORT SkipThreadAttach : 1;
			USHORT WerInShipAssertCode : 1;
			USHORT RanProcessInit : 1;
			USHORT ClonedThread : 1;
			USHORT SuppressDebugMsg : 1;
			USHORT DisableUserStackWalk : 1;
			USHORT RtlExceptionAttached : 1;
			USHORT InitialThread : 1;
			USHORT SessionAware : 1;
			USHORT SpareSameTebBits : 4;
		};
	};
	PVOID TxnScopeEnterCallback;
	PVOID TxnScopeExitCallback;
	PVOID TxnScopeContext;
	ULONG LockCount;
	ULONG SpareUlong0;
	PVOID ResourceRetValue;
	PVOID ReservedForWdf;
} TEB, *PTEB;

//
// Private
//

typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER	KernelTime;
	LARGE_INTEGER	UserTime;
	LARGE_INTEGER	CreateTime;
	ULONG			WaitTime;
	PVOID			StartAddress;
	CLIENT_ID		ClientId;
	LONG			Priority;
	LONG			BasePriority;
	ULONG			ContextSwitchCount;
	LONG			State;
	LONG			WaitReason;
} SYSTEM_THREADS, *PSYSTEM_THREADS;

typedef struct _PROCESS_BASIC_INFORMATION
{
	LONG_PTR ExitStatus;
	PVOID PebBaseAddress;
	ULONG_PTR AffinityMask;
	LONG_PTR BasePriority;
	ULONG_PTR UniqueProcessId;
	ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

#define MAX_UNICODE_PATH 32767L
typedef struct _PROCESS_INFO
{
	DWORD   dwPID;
	DWORD   dwParentPID;
	DWORD   dwSessionID;
	DWORD   dwPEBBaseAddress;
	DWORD   dwAffinityMask;
	LONG    dwBasePriority;
	LONG    dwExitStatus;
	BYTE    cBeingDebugged;
	TCHAR   szImgPath[MAX_UNICODE_PATH];
	TCHAR   szCmdLine[MAX_UNICODE_PATH];
} PROCESS_INFO, *PPROCESS_INFO;

typedef struct _SYSTEM_THREAD_INFORMATION
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	LONG BasePriority;
	ULONG ContextSwitches;
	ULONG ThreadState;
	KWAIT_REASON WaitReason;
} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;

typedef struct _SYSTEM_EXTENDED_THREAD_INFORMATION
{
	SYSTEM_THREAD_INFORMATION ThreadInfo;
	PVOID StackBase;
	PVOID StackLimit;
	PVOID Win32StartAddress;
	PTEB TebBase; // since VISTA
	ULONG_PTR Reserved2;
	ULONG_PTR Reserved3;
	ULONG_PTR Reserved4;
} SYSTEM_EXTENDED_THREAD_INFORMATION, *PSYSTEM_EXTENDED_THREAD_INFORMATION;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER WorkingSetPrivateSize; // since VISTA
	ULONG HardFaultCount; // since WIN7
	ULONG NumberOfThreadsHighWatermark; // since WIN7
	ULONGLONG CycleTime; // since WIN7
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR UniqueProcessKey; // since VISTA (requires SystemExtendedProcessInformation)
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
	SYSTEM_THREAD_INFORMATION Threads[1];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

#define PROCESS_INFORMATION_FIRST_PROCESS(Processes) ((PSYSTEM_PROCESS_INFORMATION)(Processes))
/*
* Gets a pointer to the process information structure after a given structure.
* \param Process A pointer to a process information structure.
* \return A pointer to the next process information structure, or NULL if there are no more.
*/
#define PROCESS_INFORMATION_NEXT_PROCESS(Process) ((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset ? (PSYSTEM_PROCESS_INFORMATION)((PCHAR)(Process) + ((PSYSTEM_PROCESS_INFORMATION)(Process))->NextEntryOffset) : NULL
    
typedef struct _SYSTEM_PROCESSOR_INFORMATION
{
	USHORT ProcessorArchitecture;
	USHORT ProcessorLevel;
	USHORT ProcessorRevision;
	USHORT Reserved;
	ULONG ProcessorFeatureBits;
} SYSTEM_PROCESSOR_INFORMATION, *PSYSTEM_PROCESSOR_INFORMATION;

//
// Attributes stuff
//
typedef struct _PS_ATTRIBUTE
{
	ULONG Attribute;
	SIZE_T Size;
	union
	{
		ULONG Value;
		PVOID ValuePtr;
	};
	PSIZE_T ReturnLength;
} PS_ATTRIBUTE, *PPS_ATTRIBUTE;
typedef struct _PS_ATTRIBUTE_LIST
{
	SIZE_T TotalLength;
	PS_ATTRIBUTE Attributes[1];
} PS_ATTRIBUTE_LIST, *PPS_ATTRIBUTE_LIST;
typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor; // PSECURITY_DESCRIPTOR;
	PVOID SecurityQualityOfService; // PSECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef struct _OBJECT_ATTRIBUTES64
{
	ULONG Length;
	ULONG64 RootDirectory;
	ULONG64 ObjectName;
	ULONG Attributes;
	ULONG64 SecurityDescriptor;
	ULONG64 SecurityQualityOfService;
} OBJECT_ATTRIBUTES64, *POBJECT_ATTRIBUTES64;

#define InitializeObjectAttributes(p,n,a,r,s) { \
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

#define PS_ATTRIBUTE_NUMBER_MASK 0x0000FFFF
#define PS_ATTRIBUTE_THREAD 0x00010000 // can be used with threads
#define PS_ATTRIBUTE_INPUT 0x00020000 // input only
#define PS_ATTRIBUTE_ADDITIVE 0x00040000 /// Is an additional option (see ProcThreadAttributeValue in WinBase.h)
#define PsAttributeValue(Number, Thread, Input, Additive) (((Number) & PS_ATTRIBUTE_NUMBER_MASK) | ((Thread) ? PS_ATTRIBUTE_THREAD : 0) | ((Input) ? PS_ATTRIBUTE_INPUT : 0) | ((Additive) ? PS_ATTRIBUTE_ADDITIVE : 0))

typedef VOID( NTAPI *tRtlInitUnicodeString )(
	IN OUT PUNICODE_STRING DestinationString,
	IN PCWSTR SourceString
	);

typedef VOID( NTAPI *tRtlFreeUnicodeString )(
	IN PUNICODE_STRING UnicodeString
	);

typedef ULONG( NTAPI *tRtlNtStatusToDosError )(
	IN NTSTATUS Status
	);

typedef NTSTATUS( NTAPI *tRtlGetNativeSystemInformation )(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT OPTIONAL PULONG ReturnLength
	);

typedef NTSTATUS( NTAPI *tRtlGetVersion )(
	OUT PRTL_OSVERSIONINFOW lpVersionInformation
	);

typedef NTSTATUS( NTAPI *tRtlDosApplyFileIsolationRedirection_Ustr )(
	IN ULONG Flags,
	IN PUNICODE_STRING OriginalName,
	IN PUNICODE_STRING Extension,
	IN OUT PUNICODE_STRING StaticString,
	IN OUT PUNICODE_STRING DynamicString,
	IN OUT PUNICODE_STRING *NewName,
	IN PULONG  NewFlags,
	IN PSIZE_T FileNameSize,
	IN PSIZE_T RequiredLength
	);

typedef NTSTATUS( NTAPI *tNtQueryInformationProcess )(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT OPTIONAL PULONG ReturnLength
	);

typedef NTSTATUS( NTAPI *tNtQuerySystemInformation )(
	IN ULONG SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT OPTIONAL PULONG ReturnLength
	);

typedef NTSTATUS( NTAPI *tNtOpenProcessToken )(
	IN HANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE TokenHandle
	);

typedef NTSTATUS( NTAPI *tNtAdjustPrivilegesToken )(
	IN HANDLE TokenHandle,
	IN BOOLEAN DisableAllPrivileges,
	IN PTOKEN_PRIVILEGES NewState,
	IN ULONG BufferLength,
	OUT PTOKEN_PRIVILEGES PreviousState OPTIONAL,
	OUT PULONG ReturnLength OPTIONAL
	);

typedef NTSTATUS( NTAPI *tNtCreateThreadEx )(
	OUT PHANDLE ThreadHandle,
	IN ACCESS_MASK DesiredAccess,
	IN OPTIONAL POBJECT_ATTRIBUTES ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN PVOID StartRoutine, // PUSER_THREAD_START_ROUTINE
	IN OPTIONAL PVOID Argument,
	IN ULONG CreateFlags, // THREAD_CREATE_FLAGS_*
	IN SIZE_T ZeroBits,
	IN SIZE_T StackSize,
	IN SIZE_T MaximumStackSize,
	IN OPTIONAL PPS_ATTRIBUTE_LIST AttributeList
	);

typedef NTSTATUS( NTAPI *tNtResumeThread )(
	IN HANDLE ThreadHandle,
	OUT PULONG SuspendCount OPTIONAL
	);

typedef NTSTATUS( NTAPI *tNtTerminateThread )(
	IN HANDLE ThreadHandle,
	IN UINT ExitCode
	);

typedef NTSTATUS( NTAPI *tNtClose )(
	IN HANDLE ObjectHandle
	);

typedef NTSTATUS( NTAPI *tNtTerminateProcess )(
	IN HANDLE ProcessHandle OPTIONAL,
	IN NTSTATUS ExitStatus
	);

typedef NTSTATUS( NTAPI *tNtWaitForSingleObject )(
	IN HANDLE ObjectHandle,
	IN BOOLEAN Alertable,
	IN PLARGE_INTEGER TimeOut OPTIONAL
	);

typedef NTSTATUS( NTAPI *tNtOpenProcess )(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK AccessMask,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	IN PCLIENT_ID ClientId
	);

