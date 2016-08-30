#include "stdafx.h"

#include "NtDll.h"

namespace ntdll
{
	HMODULE Base = nullptr;
	tNtQuerySystemInformation NtQuerySystemInformation = nullptr;
	tNtQueryInformationProcess NtQueryInformationProcess = nullptr;
	tRtlGetVersion RtlGetVersion = nullptr;
	tRtlGetNativeSystemInformation RtlGetNativeSystemInformation = nullptr;
	tNtCreateThreadEx NtCreateThreadEx = nullptr;
}