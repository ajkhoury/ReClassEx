#include "stdafx.h"
#include "Utils.h"

namespace Utils
{
	bool IsWindowsVersionOrLater(OSType target)
	{
		if (target == Windows10)
		{
			RTL_OSVERSIONINFOEXW verInfo = { 0 };
			verInfo.dwOSVersionInfoSize = sizeof(verInfo);

			if (ntdll::RtlGetVersion != nullptr && ntdll::RtlGetVersion((PRTL_OSVERSIONINFOW)&verInfo) == 0)
			{
				return (verInfo.dwMajorVersion == 10);
			}
			return false;
		}

		OSVERSIONINFOEX info;
		memset(&info, 0, sizeof(OSVERSIONINFOEX));
		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if (target >= WinVista)
		{
			info.dwMajorVersion = 6;

			switch (target)
			{
			case WinVista:   info.dwMinorVersion = 0; break;
			case Windows7:   info.dwMinorVersion = 1; break;
			case Windows8:	  info.dwMinorVersion = 2; break;
			default: break;
			}
		}
		else
		{
			info.dwMajorVersion = 5;
			info.dwMinorVersion = target >= WinXP ? 1 : 0;
		}

		DWORDLONG mask = 0;

		VER_SET_CONDITION(mask, VER_MAJORVERSION, VER_GREATER_EQUAL);
		VER_SET_CONDITION(mask, VER_MINORVERSION, VER_GREATER_EQUAL);
		VER_SET_CONDITION(mask, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
		VER_SET_CONDITION(mask, VER_SERVICEPACKMINOR, VER_GREATER_EQUAL);

		return VerifyVersionInfo(&info, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR, mask) != FALSE;
	}

	OSType GetOperatingSystemType()
	{
		const OSType types[] = { Windows10, Windows8, Windows7, WinVista, WinXP, Win2000 };
		for (int i = 0; i < _countof(types); ++i)
			if (IsWindowsVersionOrLater(types[i]))
				return types[i];
		return UnknownOS;
	}

	LONG GetProcessorArchitecture()
	{
		static LONG volatile nProcessorArchitecture = -1;
		if (nProcessorArchitecture == -1)
		{
			SYSTEM_PROCESSOR_INFORMATION sProcInfo;
			NTSTATUS nNtStatus;

			nNtStatus = ntdll::RtlGetNativeSystemInformation != nullptr ?
				ntdll::RtlGetNativeSystemInformation((SYSTEM_INFORMATION_CLASS)SystemProcessorInformation, &sProcInfo, sizeof(sProcInfo), NULL) : STATUS_NOT_IMPLEMENTED;
			if (nNtStatus == STATUS_NOT_IMPLEMENTED)
			{
				//tNtQuerySystemInformation fnQuerySystemInformation = (tNtQuerySystemInformation)Utils::GetLocalProcAddress(Utils::GetLocalModuleHandle("ntdll.dll"), "NtQuerySystemInformation");
				nNtStatus = ntdll::NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemProcessorInformation, &sProcInfo, sizeof(sProcInfo), NULL);
			}
			if (NT_SUCCESS(nNtStatus))
				InterlockedExchange(&nProcessorArchitecture, (LONG)(sProcInfo.ProcessorArchitecture));
		}
		return nProcessorArchitecture;
	}

	int GetProcessPlatform(HANDLE hProcess)
	{
		if (hProcess == (HANDLE)((LONG_PTR)-1))
		{
#if defined(_M_IX86)
			return 1; // ProcessPlatformX86;
#elif defined(_M_X64)
			return 2; // ProcessPlatformX64
#endif
		}
		switch (Utils::GetProcessorArchitecture())
		{
		case PROCESSOR_ARCHITECTURE_INTEL:
			return ProcessPlatformX86;
		case PROCESSOR_ARCHITECTURE_AMD64:
			//check on 64-bit platforms
			ULONG_PTR nWow64;
			NTSTATUS nNtStatus;

			nNtStatus = ntdll::NtQueryInformationProcess(hProcess, ProcessWow64Information, &nWow64, sizeof(nWow64), NULL);
			if (NT_SUCCESS(nNtStatus))
			{
#ifdef _WIN64
				return (nWow64 != 0) ? ProcessPlatformX86 : ProcessPlatformX64;
#else
				return (nWow64 == 0) ? ProcessPlatformX64 : ProcessPlatformX86;
#endif
			}
#ifdef _WIN64
			return ProcessPlatformX64;
#else
			return ProcessPlatformX86;
#endif
			break;
			//case PROCESSOR_ARCHITECTURE_IA64:
			//case PROCESSOR_ARCHITECTURE_ALPHA64:
		}
		return STATUS_NOT_SUPPORTED;
	}

	HANDLE NtCreateThreadEx(HANDLE hProcess, LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD* threadId)
	{
		if (ntdll::NtCreateThreadEx == nullptr) 
			return NULL;

		PS_ATTRIBUTE_LIST attrList;
		ZeroMemory(&attrList, sizeof(PS_ATTRIBUTE_LIST));
		CLIENT_ID cid;
		ZeroMemory(&cid, sizeof(CLIENT_ID));
		OBJECT_ATTRIBUTES64 thrdAttr;
		InitializeObjectAttributes64(&thrdAttr, NULL, 0, NULL, NULL);

		attrList.Attributes[0].Attribute = ProcThreadAttributeValue(PsAttributeClientId, TRUE, FALSE, FALSE);
		attrList.Attributes[0].Size = sizeof(CLIENT_ID);
		attrList.Attributes[0].ValuePtr = (ULONG_PTR *)&cid;

		attrList.TotalLength = sizeof(PS_ATTRIBUTE_LIST);

		HANDLE hRemoteThread = NULL;
		HRESULT hRes = 0;

		if (!NT_SUCCESS(ntdll::NtCreateThreadEx(&hRemoteThread, THREAD_ALL_ACCESS, NULL, hProcess, lpRemoteThreadStart, lpParam, 0, 0, 0x1000, 0x100000, &attrList)))
			return NULL;

		if (threadId) *threadId = (DWORD)cid.UniqueThread;

		return hRemoteThread;
	}

}