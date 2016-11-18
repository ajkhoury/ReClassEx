#include "stdafx.h"

#include "NtDll.h"

namespace ntdll
{

PVOID Base = nullptr;
tNtQuerySystemInformation NtQuerySystemInformation = nullptr;
tNtQueryInformationProcess NtQueryInformationProcess = nullptr;
tRtlGetVersion RtlGetVersion = nullptr;
tRtlGetNativeSystemInformation RtlGetNativeSystemInformation = nullptr;
tNtOpenProcessToken NtOpenProcessToken = nullptr;
tNtAdjustPrivilegesToken NtAdjustPrivilegesToken = nullptr;
tNtCreateThreadEx NtCreateThreadEx = nullptr;


bool Init( )
{
	// Already initialized
	if (Base)
		return true;

	Base = Utils::GetLocalModuleBase( _T( "ntdll.dll" ) );
	if (Base)
	{
		NtQuerySystemInformation = reinterpret_cast<tNtQuerySystemInformation>(Utils::GetLocalProcAddress( Base, _T( "NtQuerySystemInformation" ) ));
		NtQueryInformationProcess = reinterpret_cast<tNtQueryInformationProcess>(Utils::GetLocalProcAddress( Base, _T( "NtQueryInformationProcess" ) ));
		RtlGetVersion = reinterpret_cast<tRtlGetVersion>(Utils::GetLocalProcAddress( Base, _T( "RtlGetVersion" ) ));
		RtlGetNativeSystemInformation = reinterpret_cast<tRtlGetNativeSystemInformation>(Utils::GetLocalProcAddress( Base, _T( "RtlGetNativeSystemInformation" ) ));
		NtOpenProcessToken = reinterpret_cast<tNtOpenProcessToken>(Utils::GetLocalProcAddress( Base, _T( "NtOpenProcessToken" ) ));
		NtAdjustPrivilegesToken = reinterpret_cast<tNtAdjustPrivilegesToken>(Utils::GetLocalProcAddress( Base, _T( "NtAdjustPrivilegesToken" ) ));
		NtCreateThreadEx = reinterpret_cast<tNtCreateThreadEx>(Utils::GetLocalProcAddress( Base, _T( "NtCreateThreadEx" ) ));


		return !(
			!NtQueryInformationProcess ||
			!NtQueryInformationProcess ||
			!RtlGetVersion ||
			!RtlGetNativeSystemInformation ||
			!NtOpenProcessToken ||
			!NtAdjustPrivilegesToken ||
			!NtCreateThreadEx



			);
	}
	return false;
}


};