#pragma once

#include "NativeCommon.h"

///
/// < PEB Macros and Definitions >
///

#define GDI_HANDLE_BUFFER_SIZE32 34
#define GDI_HANDLE_BUFFER_SIZE64 60
#ifndef WIN64
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE32
#else
#define GDI_HANDLE_BUFFER_SIZE GDI_HANDLE_BUFFER_SIZE64
#endif

typedef ULONG GDI_HANDLE_BUFFER[GDI_HANDLE_BUFFER_SIZE];
typedef ULONG GDI_HANDLE_BUFFER32[GDI_HANDLE_BUFFER_SIZE32];
typedef ULONG GDI_HANDLE_BUFFER64[GDI_HANDLE_BUFFER_SIZE64];

///
/// < PEB Structures >
///

//
// PEB Loader Data
//

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

typedef struct _PEB_LDR_DATA64
{
	ULONG Length; // 0x0
	UCHAR Initialized; // 0x4
	ULONGLONG SsHandle; // 0x8
	LIST_ENTRY64 InLoadOrderModuleList; // 0x10
	LIST_ENTRY64 InMemoryOrderModuleList; // 0x20
	LIST_ENTRY64 InInitializationOrderModuleList; // 0x30
	ULONGLONG EntryInProgress; // 0x40
	UCHAR ShutdownInProgress; // 0x48
	ULONGLONG ShutdownThreadId; // 0x50
} PEB_LDR_DATA64, *PPEB_LDR_DATA64;

typedef struct _PEB_LDR_DATA
{
	ULONG Length; // 0x0
	UCHAR Initialized; // 0x4
	PVOID SsHandle; // 0x8
	LIST_ENTRY InLoadOrderModuleList; // 0x10
	LIST_ENTRY InMemoryOrderModuleList; // 0x20
	LIST_ENTRY InInitializationOrderModuleList; // 0x30
	PVOID EntryInProgress; // 0x40
	UCHAR ShutdownInProgress; // 0x48
	PVOID ShutdownThreadId; // 0x50
} PEB_LDR_DATA, *PPEB_LDR_DATA;


//
// PEB Structures
//

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
	ULONG Ldr; // 0xC PPEB_LDR_DATA32
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
	ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof( ULONG ) * 8)]; // 0x21C
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
	struct PEB_LDR_DATA64* Ldr; // 0x18
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
	union
	{
		ULONGLONG KernelCallbackTable; // 0x58
		ULONGLONG UserSharedInfoPtr; // 0x58
	};
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
	ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof( ULONG ) * 8)]; // 0x340
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
	#if (NTDDI_VERSION >= NTDDI_WINBLUE)
	ULONG64 TppWorkerpListLock; // 0x388
	struct _LIST_ENTRY TppWorkerpList; // 0x390
	PVOID WaitOnAddressHashTable[128]; // 0x3A0
	#endif
} PEB64, *PPEB64;

typedef struct _PEB
{
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    union
    {
        BOOLEAN BitField;
        struct
        {
            BOOLEAN ImageUsesLargePages : 1;
            BOOLEAN IsProtectedProcess : 1;
            BOOLEAN IsImageDynamicallyRelocated : 1;
            BOOLEAN SkipPatchingUser32Forwarders : 1;
            BOOLEAN IsPackagedProcess : 1;
            BOOLEAN IsAppContainer : 1;
            BOOLEAN IsProtectedProcessLight : 1;
            BOOLEAN SpareBits : 1;
        };
    };
    HANDLE Mutant;

    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
	struct _RTL_USER_PROCESS_PARAMETERS* ProcessParameters;
    PVOID SubSystemData;
    PVOID ProcessHeap;
	struct _RTL_CRITICAL_SECTION* FastPebLock;
    PVOID AtlThunkSListPtr;
    PVOID IFEOKey;
    union
    {
        ULONG CrossProcessFlags;
        struct
        {
            ULONG ProcessInJob : 1;
            ULONG ProcessInitializing : 1;
            ULONG ProcessUsingVEH : 1;
            ULONG ProcessUsingVCH : 1;
            ULONG ProcessUsingFTH : 1;
            ULONG ReservedBits0 : 27;
        };
        ULONG EnvironmentUpdateCount;
    };
    union
    {
        PVOID KernelCallbackTable;
        PVOID UserSharedInfoPtr;
    };
    ULONG SystemReserved[1];
    ULONG AtlThunkSListPtr32;
    PVOID ApiSetMap;
    ULONG TlsExpansionCounter;
    PVOID TlsBitmap;
    ULONG TlsBitmapBits[2];
    PVOID ReadOnlySharedMemoryBase;
    PVOID HotpatchInformation;
    PVOID *ReadOnlyStaticServerData;
    PVOID AnsiCodePageData;
    PVOID OemCodePageData;
    PVOID UnicodeCaseTableData;

    ULONG NumberOfProcessors;
    ULONG NtGlobalFlag;

    LARGE_INTEGER CriticalSectionTimeout;
    SIZE_T HeapSegmentReserve;
    SIZE_T HeapSegmentCommit;
    SIZE_T HeapDeCommitTotalFreeThreshold;
    SIZE_T HeapDeCommitFreeBlockThreshold;

    ULONG NumberOfHeaps;
    ULONG MaximumNumberOfHeaps;
    PVOID *ProcessHeaps;

    PVOID GdiSharedHandleTable;
    PVOID ProcessStarterHelper;
    ULONG GdiDCAttributeList;

    PRTL_CRITICAL_SECTION LoaderLock;

    ULONG OSMajorVersion;
    ULONG OSMinorVersion;
    USHORT OSBuildNumber;
    USHORT OSCSDVersion;
    ULONG OSPlatformId;
    ULONG ImageSubsystem;
    ULONG ImageSubsystemMajorVersion;
    ULONG ImageSubsystemMinorVersion;
    ULONG_PTR ImageProcessAffinityMask;
    GDI_HANDLE_BUFFER GdiHandleBuffer;
    PVOID PostProcessInitRoutine;

    PVOID TlsExpansionBitmap;
    ULONG TlsExpansionBitmapBits[32];

    ULONG SessionId;

    ULARGE_INTEGER AppCompatFlags;
    ULARGE_INTEGER AppCompatFlagsUser;
    PVOID pShimData;
    PVOID AppCompatInfo;

    UNICODE_STRING CSDVersion;

    PVOID ActivationContextData;
    PVOID ProcessAssemblyStorageMap;
    PVOID SystemDefaultActivationContextData;
    PVOID SystemAssemblyStorageMap;

    SIZE_T MinimumStackCommit;

    PVOID *FlsCallback;
    LIST_ENTRY FlsListHead;
    PVOID FlsBitmap;
    ULONG FlsBitmapBits[FLS_MAXIMUM_AVAILABLE / (sizeof(ULONG) * 8)];
    ULONG FlsHighIndex;

    PVOID WerRegistrationData;
    PVOID WerShipAssertPtr;
    PVOID pContextData;
    PVOID pImageHeaderHash;
    union
    {
        ULONG TracingFlags;
        struct
        {
            ULONG HeapTracingEnabled : 1;
            ULONG CritSecTracingEnabled : 1;
            ULONG LibLoaderTracingEnabled : 1;
            ULONG SpareTracingBits : 29;
        };
    };
    ULONGLONG CsrServerReadOnlySharedMemoryBase;
	#if (NTDDI_VERSION >= NTDDI_WINBLUE)
	ULONG_PTR TppWorkerpListLock; // 0x388
	LIST_ENTRY TppWorkerpList; // 0x390
	PVOID WaitOnAddressHashTable[128]; // 0x3A0
	#endif
} PEB, *PPEB;
