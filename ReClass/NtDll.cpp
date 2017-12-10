#include "stdafx.h"

#include "NtDll.h"

namespace ntdll {

PVOID Base = nullptr;

tNtQuerySystemInformation NtQuerySystemInformation = nullptr;
tNtQueryInformationProcess NtQueryInformationProcess = nullptr;

tNtCreateThreadEx NtCreateThreadEx = nullptr;

tNtOpenProcessToken NtOpenProcessToken = nullptr;
tNtAdjustPrivilegesToken NtAdjustPrivilegesToken = nullptr;

tRtlInitializeCriticalSection RtlInitializeCriticalSection = nullptr;
tRtlDeleteCriticalSection RtlDeleteCriticalSection = nullptr;
tRtlEnterCriticalSection RtlEnterCriticalSection = nullptr;
tRtlLeaveCriticalSection RtlLeaveCriticalSection = nullptr;

tRtlGetVersion RtlGetVersion = nullptr;
tRtlGetNativeSystemInformation RtlGetNativeSystemInformation = nullptr;

BOOL Init( VOID )
{
	// Already initialized
	if (Base)
		return true;

	Base = Utils::GetLocalModuleBase( _T( "ntdll.dll" ) );
	if (Base)
	{
		NtQuerySystemInformation = (tNtQuerySystemInformation)Utils::GetLocalProcAddress( Base, _T( "NtQuerySystemInformation" ) );
		NtQueryInformationProcess = (tNtQueryInformationProcess)Utils::GetLocalProcAddress( Base, _T( "NtQueryInformationProcess" ) );
		
		NtCreateThreadEx = (tNtCreateThreadEx)Utils::GetLocalProcAddress( Base, _T( "NtCreateThreadEx" ) );

		NtOpenProcessToken = (tNtOpenProcessToken)Utils::GetLocalProcAddress( Base, _T( "NtOpenProcessToken" ) );
		NtAdjustPrivilegesToken = (tNtAdjustPrivilegesToken)Utils::GetLocalProcAddress( Base, _T( "NtAdjustPrivilegesToken" ) );

		RtlInitializeCriticalSection = (tRtlInitializeCriticalSection)Utils::GetLocalProcAddress( Base, _T( "RtlInitializeCriticalSection" ) );
		RtlDeleteCriticalSection = (tRtlDeleteCriticalSection)Utils::GetLocalProcAddress( Base, _T( "RtlDeleteCriticalSection" ) );
		RtlEnterCriticalSection = (tRtlEnterCriticalSection)Utils::GetLocalProcAddress( Base, _T( "RtlEnterCriticalSection" ) );
		RtlLeaveCriticalSection = (tRtlLeaveCriticalSection)Utils::GetLocalProcAddress( Base, _T( "RtlLeaveCriticalSection" ) );

		RtlGetVersion = (tRtlGetVersion)Utils::GetLocalProcAddress( Base, _T( "RtlGetVersion" ) );
		RtlGetNativeSystemInformation = (tRtlGetNativeSystemInformation)Utils::GetLocalProcAddress( Base, _T( "RtlGetNativeSystemInformation" ) );

		return !(
			!NtQueryInformationProcess ||
			!NtQueryInformationProcess ||

			!NtCreateThreadEx ||	

			!NtOpenProcessToken ||
			!NtAdjustPrivilegesToken ||

			!RtlInitializeCriticalSection ||
			!RtlDeleteCriticalSection ||
			!RtlEnterCriticalSection ||
			!RtlLeaveCriticalSection ||

			!RtlGetVersion ||
			!RtlGetNativeSystemInformation

			);
	}
	return false;
}


};