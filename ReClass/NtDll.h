#pragma once

#include "Utils.h"

//
// NTDLL
//
namespace ntdll {

extern PVOID Base;

extern tNtQuerySystemInformation NtQuerySystemInformation;
extern tNtQueryInformationProcess NtQueryInformationProcess;

extern tNtCreateThreadEx NtCreateThreadEx;

extern tNtOpenProcessToken NtOpenProcessToken;
extern tNtAdjustPrivilegesToken NtAdjustPrivilegesToken;

extern tRtlInitializeCriticalSection RtlInitializeCriticalSection;
extern tRtlDeleteCriticalSection RtlDeleteCriticalSection;
extern tRtlEnterCriticalSection RtlEnterCriticalSection;
extern tRtlLeaveCriticalSection RtlLeaveCriticalSection;

extern tRtlGetVersion RtlGetVersion;
extern tRtlGetNativeSystemInformation RtlGetNativeSystemInformation;

BOOL Init( VOID );

}
