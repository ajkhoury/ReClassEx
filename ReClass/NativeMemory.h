#pragma once

#include "NativeCommon.h"

///
/// < Native Memory Defines >
///

#define MMPFNLIST_ZERO 0
#define MMPFNLIST_FREE 1
#define MMPFNLIST_STANDBY 2
#define MMPFNLIST_MODIFIED 3
#define MMPFNLIST_MODIFIEDNOWRITE 4
#define MMPFNLIST_BAD 5
#define MMPFNLIST_ACTIVE 6
#define MMPFNLIST_TRANSITION 7

#define MMPFNUSE_PROCESSPRIVATE 0
#define MMPFNUSE_FILE 1
#define MMPFNUSE_PAGEFILEMAPPED 2
#define MMPFNUSE_PAGETABLE 3
#define MMPFNUSE_PAGEDPOOL 4
#define MMPFNUSE_NONPAGEDPOOL 5
#define MMPFNUSE_SYSTEMPTE 6
#define MMPFNUSE_SESSIONPRIVATE 7
#define MMPFNUSE_METAFILE 8
#define MMPFNUSE_AWEPAGE 9
#define MMPFNUSE_DRIVERLOCKPAGE 10

#define SEC_BASED 0x200000
#define SEC_NO_CHANGE 0x400000
#define SEC_GLOBAL 0x20000000

#define MEM_EXECUTE_OPTION_DISABLE 0x1
#define MEM_EXECUTE_OPTION_ENABLE 0x2
#define MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION 0x4
#define MEM_EXECUTE_OPTION_PERMANENT 0x8
#define MEM_EXECUTE_OPTION_EXECUTE_DISPATCH_ENABLE 0x10
#define MEM_EXECUTE_OPTION_IMAGE_DISPATCH_ENABLE 0x20
#define MEM_EXECUTE_OPTION_VALID_FLAGS 0x3F


///
/// < Memory Enums >
///

typedef enum _SECTION_INHERIT
{
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT;


///
/// < Memory Structures >
///

typedef struct _MEMORY_FRAME_INFORMATION
{
	ULONGLONG UseDescription : 4; // MMPFNUSE_*
	ULONGLONG ListDescription : 3; // MMPFNLIST_*
	ULONGLONG Reserved0 : 1; // reserved for future expansion
	ULONGLONG Pinned : 1; // 1 - pinned, 0 - not pinned
	ULONGLONG DontUse : 48; // *_INFORMATION overlay
	ULONGLONG Priority : 3; // rev
	ULONGLONG Reserved : 4; // reserved for future expansion
} MEMORY_FRAME_INFORMATION;

typedef struct _FILEOFFSET_INFORMATION
{
	ULONGLONG DontUse : 9; // MEMORY_FRAME_INFORMATION overlay
	ULONGLONG Offset : 48; // mapped files
	ULONGLONG Reserved : 7; // reserved for future expansion
} FILEOFFSET_INFORMATION;

typedef struct _PAGEDIR_INFORMATION
{
	ULONGLONG DontUse : 9; // MEMORY_FRAME_INFORMATION overlay
	ULONGLONG PageDirectoryBase : 48; // private pages
	ULONGLONG Reserved : 7; // reserved for future expansion
} PAGEDIR_INFORMATION;

typedef struct _MMPFN_MEMSNAP_INFORMATION
{
	ULONG_PTR InitialPageFrameIndex;
	ULONG_PTR Count;
} MMPFN_MEMSNAP_INFORMATION, *PMMPFN_MEMSNAP_INFORMATION;

typedef struct _MMPFN_IDENTITY
{
    union
    {
        MEMORY_FRAME_INFORMATION e1; // all
        FILEOFFSET_INFORMATION e2; // mapped files
        PAGEDIR_INFORMATION e3; // private pages
    } DUMMYUNIONNAME;
    ULONG_PTR PageFrameIndex; // all
    union
    {
        PVOID FileObject; // mapped files
        PVOID VirtualAddress; // everything else
    } DUMMYUNIONNAME;
} MMPFN_IDENTITY, *PMMPFN_IDENTITY;


///
/// < Memory Routines >
///

typedef NTSTATUS (NTAPI *tNtAllocateVirtualMemory)(
    IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN ULONG_PTR ZeroBits,
	IN OUT PSIZE_T RegionSize,
	IN ULONG AllocationType,
	IN ULONG Protect
    );

typedef NTSTATUS (NTAPI *tNtFreeVirtualMemory)(
	IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN OUT PSIZE_T RegionSize,
	IN ULONG FreeType
    );

typedef NTSTATUS (NTAPI *tNtReadVirtualMemory)(
    IN HANDLE ProcessHandle,
	IN PVOID BaseAddress OPTIONAL,
    OUT PVOID Buffer,
    IN SIZE_T BufferSize,
    OUT PSIZE_T NumberOfBytesRead OPTIONAL
    );

typedef NTSTATUS(NTAPI *tNtWriteVirtualMemory)(
    IN HANDLE ProcessHandle,
	IN PVOID BaseAddress OPTIONAL,
    IN PVOID Buffer,
	IN SIZE_T BufferSize,
    OUT PSIZE_T NumberOfBytesWritten OPTIONAL
    );

typedef NTSTATUS(NTAPI *tNtProtectVirtualMemory)(
    IN HANDLE ProcessHandle,
	IN OUT PVOID *BaseAddress,
	IN OUT PSIZE_T RegionSize,
	IN ULONG NewProtect,
    OUT PULONG OldProtect
    );

typedef NTSTATUS(NTAPI *tNtLockVirtualMemory)(
	IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
	IN ULONG MapType
    );

typedef NTSTATUS(NTAPI *tNtUnlockVirtualMemory)(
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
	IN ULONG MapType
    );

typedef NTSTATUS(NTAPI *tNtCreateSection)(
    OUT PHANDLE SectionHandle,
	IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN PLARGE_INTEGER MaximumSize OPTIONAL,
    IN ULONG SectionPageProtection,
    IN ULONG AllocationAttributes,
    IN HANDLE FileHandle OPTIONAL
    );

typedef NTSTATUS(NTAPI *tNtOpenSection)(
    OUT PHANDLE SectionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes
    );

typedef NTSTATUS(NTAPI *tNtMapViewOfSection)(
    IN HANDLE SectionHandle,
    IN HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN ULONG_PTR ZeroBits,
    IN SIZE_T CommitSize,
	IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
	IN OUT PSIZE_T ViewSize,
    IN SECTION_INHERIT InheritDisposition,
    IN ULONG AllocationType,
    IN ULONG Win32Protect
    );

typedef NTSTATUS(NTAPI *tNtUnmapViewOfSection)(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress OPTIONAL
    );

#if (NTDDI_VERSION >= NTDDI_WIN8)
typedef NTSTATUS(NTAPI *tNtUnmapViewOfSectionEx)(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress OPTIONAL,
	IN ULONG Flags
    );
#endif

typedef NTSTATUS(NTAPI *tNtAreMappedFilesTheSame)(
    IN PVOID File1MappedAsAnImage,
    IN PVOID File2MappedAsFile
    );

#if (NTDDI_VERSION >= NTDDI_WINBLUE)
typedef NTSTATUS(NTAPI *tNtCreatePartition)(
    OUT PHANDLE PartitionHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN ULONG PreferredNode
    );

typedef NTSTATUS(NTAPI *tNtOpenPartition)(
    OUT PHANDLE PartitionHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
    );
#endif

// User physical pages
typedef NTSTATUS(NTAPI *tNtMapUserPhysicalPages)(
    IN PVOID VirtualAddress,
    IN ULONG_PTR NumberOfPages,
    IN PULONG_PTR UserPfnArray OPTIONAL
    );

typedef NTSTATUS(NTAPI *tNtMapUserPhysicalPagesScatter)(
    IN PVOID *VirtualAddresses,
    IN ULONG_PTR NumberOfPages,
    IN PULONG_PTR UserPfnArray OPTIONAL
    );

typedef NTSTATUS(NTAPI *tNtAllocateUserPhysicalPages)(
	IN HANDLE ProcessHandle,
    IN OUT PULONG_PTR NumberOfPages,
    OUT PULONG_PTR UserPfnArray
    );

typedef NTSTATUS(NTAPI *tNtFreeUserPhysicalPages)(
    IN HANDLE ProcessHandle,
    IN OUT PULONG_PTR NumberOfPages,
    IN PULONG_PTR UserPfnArray
    );

// Sessions
#if (NTDDI_VERSION >= NTDDI_VISTA)
typedef NTSTATUS(NTAPI *tNtOpenSession)(
    OUT PHANDLE SessionHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
    );
#endif

// Misc.
typedef NTSTATUS(NTAPI *tNtGetWriteWatch)(
    IN HANDLE ProcessHandle,
    IN ULONG Flags,
    IN PVOID BaseAddress,
    IN SIZE_T RegionSize,
	OUT PVOID *UserAddressArray,
    IN OUT PULONG_PTR EntriesInUserAddressArray,
    OUT PULONG Granularity
    );

typedef NTSTATUS(NTAPI *tNtResetWriteWatch)(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress,
    IN SIZE_T RegionSize
    );

typedef NTSTATUS(NTAPI *tNtCreatePagingFile)(
    IN PUNICODE_STRING PageFileName,
    IN PLARGE_INTEGER MinimumSize,
    IN PLARGE_INTEGER MaximumSize,
    IN ULONG Priority
    );

typedef NTSTATUS(NTAPI *tNtFlushInstructionCache)(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress OPTIONAL,
	IN SIZE_T Length
    );

typedef NTSTATUS(NTAPI *tNtFlushWriteBuffer)(
    VOID
    );