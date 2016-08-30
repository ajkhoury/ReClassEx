#pragma once

#include "Utils.h"

//
// NTDLL
//
namespace ntdll
{
	extern HMODULE Base;
	extern tNtQuerySystemInformation NtQuerySystemInformation;
	extern tNtQueryInformationProcess NtQueryInformationProcess;
	extern tRtlGetVersion RtlGetVersion;
	extern tRtlGetNativeSystemInformation RtlGetNativeSystemInformation;
	extern tNtCreateThreadEx NtCreateThreadEx;

	static bool Init()
	{
		// Already initialized
		if (Base)
			return true;

		Base = Utils::GetLocalModuleHandle(_T("ntdll.dll"));
		if (Base)
		{
			NtQuerySystemInformation = reinterpret_cast<tNtQuerySystemInformation>(Utils::GetLocalProcAddress(Base, _T("NtQuerySystemInformation")));
			NtQueryInformationProcess = reinterpret_cast<tNtQueryInformationProcess>(Utils::GetLocalProcAddress(Base, _T("NtQueryInformationProcess")));
			NtCreateThreadEx = reinterpret_cast<tNtCreateThreadEx>(Utils::GetLocalProcAddress(Base, _T("NtCreateThreadEx")));
			RtlGetVersion = reinterpret_cast<tRtlGetVersion>(Utils::GetLocalProcAddress(Base, _T("RtlGetVersion")));
			RtlGetNativeSystemInformation = reinterpret_cast<tRtlGetNativeSystemInformation>(Utils::GetLocalProcAddress(Base, _T("RtlGetNativeSystemInformation")));
			return !(!NtQueryInformationProcess || !NtQueryInformationProcess || !NtCreateThreadEx || !RtlGetVersion || !RtlGetNativeSystemInformation);
		}
		return false;
	}
}
