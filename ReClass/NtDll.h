#pragma once

#include "Utils.h"

//
// NTDLL
//
namespace ntdll
{

extern PVOID Base;
extern tNtQuerySystemInformation NtQuerySystemInformation;
extern tNtQueryInformationProcess NtQueryInformationProcess;
extern tRtlGetVersion RtlGetVersion;
extern tRtlGetNativeSystemInformation RtlGetNativeSystemInformation;
extern tNtOpenProcessToken NtOpenProcessToken;
extern tNtAdjustPrivilegesToken NtAdjustPrivilegesToken;
extern tNtCreateThreadEx NtCreateThreadEx;

bool Init( );

}
