#pragma once

#include "NativePeb.h"


///
/// < TEB Macros and Definitions >
///

#define GDI_BATCH_BUFFER_SIZE 310

///
/// < TEB Structures >
///

typedef struct _GDI_TEB_BATCH
{
    ULONG Offset;
    ULONG_PTR HDC;
    ULONG Buffer[GDI_BATCH_BUFFER_SIZE];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _GDI_TEB_BATCH64
{
    ULONG Offset;
    ULONGLONG HDC; // ULONG_PTR
    ULONG Buffer[GDI_BATCH_BUFFER_SIZE];
} GDI_TEB_BATCH64, *PGDI_TEB_BATCH64;

typedef struct _GDI_TEB_BATCH32
{
    ULONG Offset;
    ULONG HDC; // ULONG_PTR
    ULONG Buffer[GDI_BATCH_BUFFER_SIZE];
} GDI_TEB_BATCH32, *PGDI_TEB_BATCH32;

typedef struct _TEB_ACTIVE_FRAME_CONTEXT
{
    ULONG Flags;
    PSTR FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME
{
    ULONG Flags;
    struct _TEB_ACTIVE_FRAME* Previous;
    struct _TEB_ACTIVE_FRAME_CONTEXT* Context;
} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

typedef struct _TEB64
{
    NT_TIB64 NtTib; // 0x0
    ULONG64 EnvironmentPointer; // 0x38
    CLIENT_ID64 ClientId; // 0x40
    ULONG64 ActiveRpcHandle; // 0x50
    ULONG64 ThreadLocalStoragePointer; // 0x58
    ULONG64 ProcessEnvironmentBlock; // 0x60
    ULONG LastErrorValue; // 0x68
    ULONG CountOfOwnedCriticalSections; // 0x6C
    ULONG64 CsrClientThread; // 0x70
    ULONG64 Win32ThreadInfo; // 0x78
    ULONG User32Reserved[26]; // 0x80
    ULONG UserReserved[5]; // 0xE8
    ULONG64 WOW32Reserved; // 0x100
    ULONG CurrentLocale; // 0x108
    ULONG FpSoftwareStatusRegister; // 0x10C
    ULONG64 ReservedForDebuggerInstrumentation[16]; // 0x110
    ULONG64 SystemReserved1[38]; // 0x190
    LONG ExceptionCode; // 0x2C0
    UCHAR Padding0[4]; // 0x2C4
    ULONG64 ActivationContextStackPointer; // 0x2C8
    ULONG64 InstrumentationCallbackSp; // 0x2D0
    ULONG64 InstrumentationCallbackPreviousPc; // 0x2D8
    ULONG64 InstrumentationCallbackPreviousSp; // 0x2E0
    ULONG TxFsContext; // 0x2E8
    UCHAR InstrumentationCallbackDisabled; // 0x2EC
    UCHAR Padding1[3]; // 0x2ED
    GDI_TEB_BATCH64 GdiTebBatch; // 0x2F0
    CLIENT_ID64 RealClientId; // 0x7D8
    ULONG64 GdiCachedProcessHandle; // 0x7E8
    ULONG GdiClientPID; // 0x7F0
    ULONG GdiClientTID; // 0x7F4
    ULONG64 GdiThreadLocalInfo; // 0x7F8
    ULONG64 Win32ClientInfo[62]; // 0x800
    ULONG64 glDispatchTable[233]; // 0x9F0
    ULONG64 glReserved1[29]; // 0x1138
    ULONG64 glReserved2; // 0x1220
    ULONG64 glSectionInfo; // 0x1228
    ULONG64 glSection; // 0x1230
    ULONG64 glTable; // 0x1238
    ULONG64 glCurrentRC; // 0x1240
    ULONG64 glContext; // 0x1248
    ULONG LastStatusValue; // 0x1250
    UCHAR Padding2[4]; // 0x1254
    STRING64 StaticUnicodeString; // 0x1258
    WCHAR StaticUnicodeBuffer[261]; // 0x1268
    UCHAR Padding3[6]; // 0x1472
    ULONG64 DeallocationStack; // 0x1478
    ULONG64 TlsSlots[64]; // 0x1480
    LIST_ENTRY64 TlsLinks; // 0x1680
    ULONG64 Vdm; // 0x1690
    ULONG64 ReservedForNtRpc; // 0x1698
    ULONG64 DbgSsReserved[2]; // 0x16A0
    ULONG HardErrorMode; // 0x16B0
    UCHAR Padding4[4]; // 0x16B4
    ULONG64 Instrumentation[11]; // 0x16B8
    GUID ActivityId; // 0x1710
    ULONG64 SubProcessTag; // 0x1720
    ULONG64 PerflibData; // 0x1728
    ULONG64 EtwTraceData; // 0x1730
    ULONG64 WinSockData; // 0x1738
    ULONG GdiBatchCount; // 0x1740
    _PROCESSOR_NUMBER CurrentIdealProcessor; // 0x1744
    ULONG IdealProcessorValue; // 0x1744
    UCHAR ReservedPad0; // 0x1744
    UCHAR ReservedPad1; // 0x1745
    UCHAR ReservedPad2; // 0x1746
    UCHAR IdealProcessor; // 0x1747
    ULONG GuaranteedStackBytes; // 0x1748
    UCHAR Padding5[4]; // 0x174C
    ULONG64 ReservedForPerf; // 0x1750
    ULONG64 ReservedForOle; // 0x1758
    ULONG WaitingOnLoaderLock; // 0x1760
    UCHAR Padding6[4]; // 0x1764
    ULONG64 SavedPriorityState; // 0x1768
    ULONG64 ReservedForCodeCoverage; // 0x1770
    ULONG64 ThreadPoolData; // 0x1778
    ULONG64 TlsExpansionSlots; // 0x1780
    ULONG64 DeallocationBStore; // 0x1788
    ULONG64 BStoreLimit; // 0x1790
    ULONG MuiGeneration; // 0x1798
    ULONG IsImpersonating; // 0x179C
    ULONG64 NlsCache; // 0x17A0
    ULONG64 pShimData; // 0x17A8
    USHORT HeapVirtualAffinity; // 0x17B0
    USHORT LowFragHeapDataSlot; // 0x17B2
    UCHAR Padding7[4]; // 0x17B4
    ULONG64 CurrentTransactionHandle; // 0x17B8
    ULONG64 ActiveFrame; // 0x17C0
    ULONG64 FlsData; // 0x17C8
    ULONG64 PreferredLanguages; // 0x17D0
    ULONG64 UserPrefLanguages; // 0x17D8
    ULONG64 MergedPrefLanguages; // 0x17E0
    ULONG MuiImpersonation; // 0x17E8
    union
    {
        USHORT CrossTebFlags; // 0x17EC
        struct
        {
            USHORT SpareCrossTebBits : 16; // 0x17EC
        };
    };
    union
    {
        USHORT SameTebFlags; // 0x17EE
        struct
        {
            USHORT SafeThunkCall : 1; // 0x17EE
            USHORT InDebugPrint : 1; // 0x17EE
            USHORT HasFiberData : 1; // 0x17EE
            USHORT SkipThreadAttach : 1; // 0x17EE
            USHORT WerInShipAssertCode : 1; // 0x17EE
            USHORT RanProcessInit : 1; // 0x17EE
            USHORT ClonedThread : 1; // 0x17EE
            USHORT SuppressDebugMsg : 1; // 0x17EE
            USHORT DisableUserStackWalk : 1; // 0x17EE
            USHORT RtlExceptionAttached : 1; // 0x17EE
            USHORT InitialThread : 1; // 0x17EE
            USHORT SessionAware : 1; // 0x17EE
            USHORT LoadOwner : 1; // 0x17EE
            USHORT LoaderWorker : 1; // 0x17EE
            USHORT SpareSameTebBits : 2; // 0x17EE
        };
    };
    ULONG64 TxnScopeEnterCallback; // 0x17F0
    ULONG64 TxnScopeExitCallback; // 0x17F8
    ULONG64 TxnScopeContext; // 0x1800
    ULONG LockCount; // 0x1808
    LONG WowTebOffset; // 0x180C
    ULONG64 ResourceRetValue; // 0x1810
    ULONG64 ReservedForWdf; // 0x1818
    ULONG64 ReservedForCrt; // 0x1820
    GUID EffectiveContainerId; // 0x1828
} TEB64, *PTEB64;

typedef struct _TEB32
{
    NT_TIB32 NtTib; // 0x0
    ULONG EnvironmentPointer; // 0x1C
    CLIENT_ID32 ClientId; // 0x20
    ULONG ActiveRpcHandle; // 0x28
    ULONG ThreadLocalStoragePointer; // 0x2C
    ULONG ProcessEnvironmentBlock; // 0x30
    ULONG LastErrorValue; // 0x34
    ULONG CountOfOwnedCriticalSections; // 0x38
    ULONG CsrClientThread; // 0x3C
    ULONG Win32ThreadInfo; // 0x40
    ULONG User32Reserved[26]; // 0x44
    ULONG UserReserved[5]; // 0xAC
    ULONG WOW32Reserved; // 0xC0
    ULONG CurrentLocale; // 0xC4
    ULONG FpSoftwareStatusRegister; // 0xC8
    ULONG ReservedForDebuggerInstrumentation[16]; // 0xCC
    ULONG SystemReserved1[38]; // 0x10C
    LONG ExceptionCode; // 0x1A4
    ULONG ActivationContextStackPointer; // 0x1A8
    ULONG InstrumentationCallbackSp; // 0x1AC
    ULONG InstrumentationCallbackPreviousPc; // 0x1B0
    ULONG InstrumentationCallbackPreviousSp; // 0x1B4
    UCHAR InstrumentationCallbackDisabled; // 0x1B8
    UCHAR SpareBytes[23]; // 0x1B9
    ULONG TxFsContext; // 0x1D0
    GDI_TEB_BATCH32 GdiTebBatch; // 0x1D4
    CLIENT_ID32 RealClientId; // 0x6B4
    ULONG GdiCachedProcessHandle; // 0x6BC
    ULONG GdiClientPID; // 0x6C0
    ULONG GdiClientTID; // 0x6C4
    ULONG GdiThreadLocalInfo; // 0x6C8
    ULONG Win32ClientInfo[62]; // 0x6CC
    ULONG glDispatchTable[233]; // 0x7C4
    ULONG glReserved1[29]; // 0xB68
    ULONG glReserved2; // 0xBDC
    ULONG glSectionInfo; // 0xBE0
    ULONG glSection; // 0xBE4
    ULONG glTable; // 0xBE8
    ULONG glCurrentRC; // 0xBEC
    ULONG glContext; // 0xBF0
    ULONG LastStatusValue; // 0xBF4
    STRING32 StaticUnicodeString; // 0xBF8
    WCHAR StaticUnicodeBuffer[261]; // 0xC00
    ULONG DeallocationStack; // 0xE0C
    ULONG TlsSlots[64]; // 0xE10
    LIST_ENTRY32 TlsLinks; // 0xF10
    ULONG Vdm; // 0xF18
    ULONG ReservedForNtRpc; // 0xF1C
    ULONG DbgSsReserved[2]; // 0xF20
    ULONG HardErrorMode; // 0xF28
    ULONG Instrumentation[9]; // 0xF2C
    GUID ActivityId; // 0xF50
    ULONG SubProcessTag; // 0xF60
    ULONG PerflibData; // 0xF64
    ULONG EtwTraceData; // 0xF68
    ULONG WinSockData; // 0xF6C
    ULONG GdiBatchCount; // 0xF70
    PROCESSOR_NUMBER CurrentIdealProcessor; // 0xF74
    ULONG IdealProcessorValue; // 0xF74
    UCHAR ReservedPad0; // 0xF74
    UCHAR ReservedPad1; // 0xF75
    UCHAR ReservedPad2; // 0xF76
    UCHAR IdealProcessor; // 0xF77
    ULONG GuaranteedStackBytes; // 0xF78
    ULONG ReservedForPerf; // 0xF7C
    ULONG ReservedForOle; // 0xF80
    ULONG WaitingOnLoaderLock; // 0xF84
    ULONG SavedPriorityState; // 0xF88
    ULONG ReservedForCodeCoverage; // 0xF8C
    ULONG ThreadPoolData; // 0xF90
    ULONG TlsExpansionSlots; // 0xF94
    ULONG MuiGeneration; // 0xF98
    ULONG IsImpersonating; // 0xF9C
    ULONG NlsCache; // 0xFA0
    ULONG pShimData; // 0xFA4
    USHORT HeapVirtualAffinity; // 0xFA8
    USHORT LowFragHeapDataSlot; // 0xFAA
    ULONG CurrentTransactionHandle; // 0xFAC
    ULONG ActiveFrame; // 0xFB0
    ULONG FlsData; // 0xFB4
    ULONG PreferredLanguages; // 0xFB8
    ULONG UserPrefLanguages; // 0xFBC
    ULONG MergedPrefLanguages; // 0xFC0
    ULONG MuiImpersonation; // 0xFC4
    union
    {
        USHORT CrossTebFlags; // 0xFC8
        struct
        {
            USHORT SpareCrossTebBits : 16; // 0xFC8
        };
    };
    union
    {
        USHORT SameTebFlags; // 0xFCA
        struct
        {
            USHORT SafeThunkCall : 1; // 0xFCA
            USHORT InDebugPrint : 1; // 0xFCA
            USHORT HasFiberData : 1; // 0xFCA
            USHORT SkipThreadAttach : 1; // 0xFCA
            USHORT WerInShipAssertCode : 1; // 0xFCA
            USHORT RanProcessInit : 1; // 0xFCA
            USHORT ClonedThread : 1; // 0xFCA
            USHORT SuppressDebugMsg : 1; // 0xFCA
            USHORT DisableUserStackWalk : 1; // 0xFCA
            USHORT RtlExceptionAttached : 1; // 0xFCA
            USHORT InitialThread : 1; // 0xFCA
            USHORT SessionAware : 1; // 0xFCA
            USHORT LoadOwner : 1; // 0xFCA
            USHORT LoaderWorker : 1; // 0xFCA
            USHORT SpareSameTebBits : 2; // 0xFCA
        };
    };
    ULONG TxnScopeEnterCallback; // 0xFCC
    ULONG TxnScopeExitCallback; // 0xFD0
    ULONG TxnScopeContext; // 0xFD4
    ULONG LockCount; // 0xFD8
    LONG WowTebOffset; // 0xFDC
    ULONG ResourceRetValue; // 0xFE0
    ULONG ReservedForWdf; // 0xFE4
    ULONG64 ReservedForCrt; // 0xFE8
    GUID EffectiveContainerId; // 0xFF0
} TEB32, *PTEB32;

typedef struct _TEB
{
    struct _NT_TIB NtTib; // 0x0
    PVOID EnvironmentPointer; // 0x38
    struct _CLIENT_ID ClientId; // 0x40
    PVOID ActiveRpcHandle; // 0x50
    PVOID ThreadLocalStoragePointer; // 0x58
    struct _PEB* ProcessEnvironmentBlock; // 0x60
    ULONG LastErrorValue; // 0x68
    ULONG CountOfOwnedCriticalSections; // 0x6C
    PVOID CsrClientThread; // 0x70
    PVOID Win32ThreadInfo; // 0x78
    ULONG User32Reserved[26]; // 0x80
    ULONG UserReserved[5]; // 0xE8
    PVOID WOW32Reserved; // 0x100
    ULONG CurrentLocale; // 0x108
    ULONG FpSoftwareStatusRegister; // 0x10C
    PVOID ReservedForDebuggerInstrumentation[16]; // 0x110
    PVOID SystemReserved1[38]; // 0x190
    LONG ExceptionCode; // 0x2C0
    UCHAR Padding0[4]; // 0x2C4
    struct _ACTIVATION_CONTEXT_STACK* ActivationContextStackPointer; // 0x2C8
    ULONG_PTR InstrumentationCallbackSp; // 0x2D0
    ULONG_PTR InstrumentationCallbackPreviousPc; // 0x2D8
    ULONG_PTR InstrumentationCallbackPreviousSp; // 0x2E0
    ULONG TxFsContext; // 0x2E8
    UCHAR InstrumentationCallbackDisabled; // 0x2EC
    UCHAR Padding1[3]; // 0x2ED
    struct _GDI_TEB_BATCH GdiTebBatch; // 0x2F0
    struct _CLIENT_ID RealClientId; // 0x7D8
    PVOID GdiCachedProcessHandle; // 0x7E8
    ULONG GdiClientPID; // 0x7F0
    ULONG GdiClientTID; // 0x7F4
    PVOID GdiThreadLocalInfo; // 0x7F8
    ULONG_PTR Win32ClientInfo[62]; // 0x800
    PVOID glDispatchTable[233]; // 0x9F0
    ULONG_PTR glReserved1[29]; // 0x1138
    PVOID glReserved2; // 0x1220
    PVOID glSectionInfo; // 0x1228
    PVOID glSection; // 0x1230
    PVOID glTable; // 0x1238
    PVOID glCurrentRC; // 0x1240
    PVOID glContext; // 0x1248
    ULONG LastStatusValue; // 0x1250
    UCHAR Padding2[4]; // 0x1254
    struct _UNICODE_STRING StaticUnicodeString; // 0x1258
    WCHAR StaticUnicodeBuffer[261]; // 0x1268
    UCHAR Padding3[6]; // 0x1472
    PVOID DeallocationStack; // 0x1478
    PVOID TlsSlots[64]; // 0x1480
    struct _LIST_ENTRY TlsLinks; // 0x1680
    PVOID Vdm; // 0x1690
    PVOID ReservedForNtRpc; // 0x1698
    PVOID DbgSsReserved[2]; // 0x16A0
    ULONG HardErrorMode; // 0x16B0
    UCHAR Padding4[4]; // 0x16B4
    PVOID Instrumentation[11]; // 0x16B8
    struct _GUID ActivityId; // 0x1710
    PVOID SubProcessTag; // 0x1720
    PVOID PerflibData; // 0x1728
    PVOID EtwTraceData; // 0x1730
    PVOID WinSockData; // 0x1738
    ULONG GdiBatchCount; // 0x1740
    struct _PROCESSOR_NUMBER CurrentIdealProcessor; // 0x1744
    ULONG IdealProcessorValue; // 0x1744
    UCHAR ReservedPad0; // 0x1744
    UCHAR ReservedPad1; // 0x1745
    UCHAR ReservedPad2; // 0x1746
    UCHAR IdealProcessor; // 0x1747
    ULONG GuaranteedStackBytes; // 0x1748
    UCHAR Padding5[4]; // 0x174C
    PVOID ReservedForPerf; // 0x1750
    PVOID ReservedForOle; // 0x1758
    ULONG WaitingOnLoaderLock; // 0x1760
    UCHAR Padding6[4]; // 0x1764
    PVOID SavedPriorityState; // 0x1768
    ULONG_PTR ReservedForCodeCoverage; // 0x1770
    PVOID ThreadPoolData; // 0x1778
    PVOID* TlsExpansionSlots; // 0x1780
    PVOID DeallocationBStore; // 0x1788
    PVOID BStoreLimit; // 0x1790
    ULONG MuiGeneration; // 0x1798
    ULONG IsImpersonating; // 0x179C
    PVOID NlsCache; // 0x17A0
    PVOID pShimData; // 0x17A8
    USHORT HeapVirtualAffinity; // 0x17B0
    USHORT LowFragHeapDataSlot; // 0x17B2
    UCHAR Padding7[4]; // 0x17B4
    PVOID CurrentTransactionHandle; // 0x17B8
    struct _TEB_ACTIVE_FRAME* ActiveFrame; // 0x17C0
    PVOID FlsData; // 0x17C8
    PVOID PreferredLanguages; // 0x17D0
    PVOID UserPrefLanguages; // 0x17D8
    PVOID MergedPrefLanguages; // 0x17E0
    ULONG MuiImpersonation; // 0x17E8
    union
    {
        USHORT CrossTebFlags; // 0x17EC
        struct
        {
            USHORT SpareCrossTebBits : 16; // 0x17EC
        };
    };
    union
    {
        USHORT SameTebFlags; // 0x17EE
        struct
        {
            USHORT SafeThunkCall : 1; // 0x17EE
            USHORT InDebugPrint : 1; // 0x17EE
            USHORT HasFiberData : 1; // 0x17EE
            USHORT SkipThreadAttach : 1; // 0x17EE
            USHORT WerInShipAssertCode : 1; // 0x17EE
            USHORT RanProcessInit : 1; // 0x17EE
            USHORT ClonedThread : 1; // 0x17EE
            USHORT SuppressDebugMsg : 1; // 0x17EE
            USHORT DisableUserStackWalk : 1; // 0x17EE
            USHORT RtlExceptionAttached : 1; // 0x17EE
            USHORT InitialThread : 1; // 0x17EE
            USHORT SessionAware : 1; // 0x17EE
            USHORT LoadOwner : 1; // 0x17EE
            USHORT LoaderWorker : 1; // 0x17EE
            USHORT SpareSameTebBits : 2; // 0x17EE
        };
    };
    PVOID TxnScopeEnterCallback; // 0x17F0
    PVOID TxnScopeExitCallback; // 0x17F8
    PVOID TxnScopeContext; // 0x1800
    ULONG LockCount; // 0x1808
    LONG WowTebOffset; // 0x180C
    PVOID ResourceRetValue; // 0x1810
    PVOID ReservedForWdf; // 0x1818
    #if (NTDDI_VERSION >= NTDDI_WIN10)
    ULONG64 ReservedForCrt; // 0x1820
    struct _GUID EffectiveContainerId; // 0x1828
    #endif
} TEB, *PTEB;

typedef struct _INITIAL_TEB
{
    struct
    {
        PVOID OldStackBase;
        PVOID OldStackLimit;
    } OldInitialTeb;
    PVOID StackBase;
    PVOID StackLimit;
    PVOID StackAllocationBase;
} INITIAL_TEB, *PINITIAL_TEB;