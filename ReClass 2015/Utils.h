#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "nt_ddk.h"

namespace Utils
{
	// forward declarations
	static void* GetProcAddress(HMODULE module, const char *proc_name);
	static HMODULE GetLocalModuleHandle(const char* moduleName);
	static HMODULE GetRemoteModuleHandle(const char* moduleName);
	static void* GetRemoteProcAddress(HMODULE module, const char *proc_name); 

	// Align value
	static inline size_t Align(size_t val, size_t alignment)
	{
		return (val % alignment == 0) ? val : (val / alignment + 1) * alignment;
	}

	template<class T> 
	__forceinline int NumDigits(T number)
	{
		int digits = 0;
		// remove this if '-' counts as a digit
		if (number < 0)
			digits = 1;
		while (number)
		{
			number /= 0x16;
			digits++;
		}
		return digits;
	}

	wchar_t* GetFilePath(wchar_t* append)
	{
		static wchar_t szFile[MAX_PATH] = {};
		GetModuleFileName(GetModuleHandle(0), szFile, MAX_PATH);
		for (int i = 0; i < (int)wcslen(szFile); i++)
		{
			if (szFile[wcslen(szFile) - i] == L'\\')
			{
				szFile[(wcslen(szFile) - i) + 1] = L'\0';
				wcscat_s(szFile, append);
				return szFile;
			}
		}

		return NULL;
	}

	static BOOL IsElevated()
	{
		BOOL fRet = false;
		HANDLE hToken = NULL;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		{
			TOKEN_ELEVATION Elevation;
			DWORD cbSize = sizeof(TOKEN_ELEVATION);
			if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
				fRet = Elevation.TokenIsElevated;
		}
		if (hToken)
			CloseHandle(hToken);
		return fRet;
	}

	static BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpPrivilege, BOOL bEnablePrivilege)
	{
		TOKEN_PRIVILEGES    tkp = { 0 };
		LUID                luid = { 0 };
		TOKEN_PRIVILEGES    tkpPrevious = { 0 };
		DWORD              cbPrevious = 0;

		//
		if (!LookupPrivilegeValue(NULL, lpPrivilege, &luid))
			return FALSE;
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = luid;
		tkp.Privileges[0].Attributes = 0;
		cbPrevious = sizeof(TOKEN_PRIVILEGES);
		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), &tkpPrevious, &cbPrevious);
		if (GetLastError() != ERROR_SUCCESS)
			return FALSE;

		tkpPrevious.PrivilegeCount = 1;
		tkpPrevious.Privileges[0].Luid = luid;
		if (bEnablePrivilege)
			tkpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
		else
			tkpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tkpPrevious.Privileges[0].Attributes);
		AdjustTokenPrivileges(hToken, FALSE, &tkpPrevious, cbPrevious, NULL, NULL);
		if (GetLastError() != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	static BOOL SetDebugPrivilege(BOOL bEnable)
	{
		HANDLE hToken = NULL;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return FALSE;
		// Enable/Disable Debug Privilege
		if (!SetPrivilege(hToken, SE_DEBUG_NAME, bEnable))
		{
			CloseHandle(hToken);
			return FALSE;
		}
		CloseHandle(hToken);
		return TRUE;
	}

	static void CreateConsole()
	{
		int hConHandle = 0;   
		HANDLE lStdHandle = 0;  
		FILE *fp = 0;
		AllocConsole( );
		lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		hConHandle = _open_osfhandle(PtrToUlong( lStdHandle ), _O_TEXT);
		fp = _fdopen(hConHandle, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);
	}

	static HMODULE GetLocalModuleHandle(const char* moduleName)
	{
		void* dwModuleHandle = 0;

		struct _TEB* teb = (struct _TEB*)NtCurrentTeb();
		_PEB* peb = (_PEB*)teb->ProcessEnvironmentBlock;
		PPEB_LDR_DATA ldrData = peb->Ldr;
		PLDR_DATA_ENTRY cursor = (PLDR_DATA_ENTRY)ldrData->InInitializationOrderModuleList.Flink;

		while (cursor->BaseAddress)  
		{
			char strBaseDllName[MAX_PATH] = { 0 };
			size_t bytesCopied = 0;
			wcstombs_s(&bytesCopied, strBaseDllName, cursor->BaseDllName.Buffer, MAX_PATH);
			if (_stricmp(strBaseDllName, moduleName) == 0) {
				dwModuleHandle = cursor->BaseAddress;
				break;
			}
			cursor = (PLDR_DATA_ENTRY)cursor->InMemoryOrderModuleList.Flink;
		}
		return (HMODULE)dwModuleHandle;
	}

	static void* GetProcAddress(HMODULE module, const char *proc_name)
	{
		char *modb = (char *)module;

		IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)modb;
		IMAGE_NT_HEADERS *nt_headers = (IMAGE_NT_HEADERS *)((size_t)modb + dos_header->e_lfanew);

		IMAGE_OPTIONAL_HEADER *opt_header = &nt_headers->OptionalHeader;
		IMAGE_DATA_DIRECTORY *exp_entry = (IMAGE_DATA_DIRECTORY *)(&opt_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
		IMAGE_EXPORT_DIRECTORY *exp_dir = (IMAGE_EXPORT_DIRECTORY *)((size_t)modb + exp_entry->VirtualAddress);

		DWORD* func_table = (DWORD*)((size_t)modb + exp_dir->AddressOfFunctions);
		WORD* ord_table = (WORD *)((size_t)modb + exp_dir->AddressOfNameOrdinals);
		DWORD* name_table = (DWORD*)((size_t)modb + exp_dir->AddressOfNames);

		void *address = NULL;
		DWORD i;

		if (((ULONG_PTR)proc_name >> 16) == 0) // Ordinal
		{
			WORD ordinal = LOWORD(proc_name);
			ULONG_PTR ord_base = exp_dir->Base;
			if (ordinal < ord_base || ordinal > ord_base + exp_dir->NumberOfFunctions)
				return NULL;

			address = (void*)((size_t)modb + func_table[ordinal - ord_base]);
		}
		else
		{
			// import by name 
			for (i = 0; i < exp_dir->NumberOfNames; i++)
			{
				// name table pointers are rvas
				char* procEntryName = (char*)((size_t)modb + name_table[i]);
				if (_stricmp(proc_name, procEntryName) == 0)
				{
					address = (void*)((size_t)modb + func_table[ord_table[i]]);
					break;
				}
			}
		}
		if ((char *)address >= (char*)exp_dir && (char*)address < (char*)exp_dir + exp_entry->Size)
		{
			HMODULE frwd_module = 0;

			char* dll_name = _strdup((char*)address);
			if (!dll_name)
				return NULL;
			char* func_name = strchr(dll_name, '.');
			*func_name++ = 0;

			address = NULL;

			char dllName[256];
			strcpy_s(dllName, dll_name);
			strcat_s(dllName, strlen(dll_name) + 4 + 1, ".dll");

			frwd_module = (HMODULE)Utils::GetLocalModuleHandle(dllName);
			if (!frwd_module)
				frwd_module = LoadLibraryA(dllName);
			if (!frwd_module)
			{
				//MessageBox(0, _T("GetProcAddress failed to load module using GetModuleHandle and LoadLibrary!"), _T("Reclass 2015"), MB_ICONERROR);
				return NULL;
			}

			bool forwardByOrd = strchr(func_name, '#') == 0 ? false : true;
			if (forwardByOrd) // forwarded by ordinal
			{
				WORD func_ord = atoi(func_name + 1);
				address = Utils::GetProcAddress(frwd_module, (const char*)func_ord);
			}
			else
			{
				address = Utils::GetProcAddress(frwd_module, func_name);
			}

			free(dll_name);
		}
		return address;
	}

	enum OSType
	{
		UnknownOS = 0,
		Win2000 = 0x4105,
		WinXP = 0x4106,
		WinVista = 0x4107,
		Windows7 = 0x4108,
		Windows8 = 0x4109,
		Windows10 = 0x4110,

		Windows = 0x4000,   /**< To test whether any version of Windows is running,
							you can use the expression ((getOperatingSystemType() & Windows) != 0). */
	};

	static bool IsWindowsVersionOrLater(OSType target)
	{
		if (target == Windows10)
		{
			typedef LONG(__stdcall* fnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);
			static fnRtlGetVersion RtlGetVersion = (fnRtlGetVersion)Utils::GetProcAddress((HMODULE)Utils::GetLocalModuleHandle("ntdll.dll"), "RtlGetVersion");

			RTL_OSVERSIONINFOEXW verInfo = { 0 };
			verInfo.dwOSVersionInfoSize = sizeof(verInfo);

			if (RtlGetVersion != 0 && RtlGetVersion((PRTL_OSVERSIONINFOW)&verInfo) == 0)
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

	static OSType GetOperatingSystemType()
	{
		const OSType types[] = { Windows10, Windows8, Windows7, WinVista, WinXP, Win2000 };
		for (int i = 0; i < 6; ++i)
			if (IsWindowsVersionOrLater(types[i]))
				return types[i];
		return UnknownOS;
	}
	
	static LONG GetProcessorArchitecture()
	{
		static LONG volatile nProcessorArchitecture = -1;
		if (nProcessorArchitecture == -1)
		{
			SYSTEM_PROCESSOR_INFORMATION sProcInfo;
			NTSTATUS nNtStatus;

			tRtlGetNativeSystemInformation fnRtlGetNativeSystemInformation = (tRtlGetNativeSystemInformation)Utils::GetProcAddress((HMODULE)Utils::GetLocalModuleHandle("ntdll.dll"), "RtlGetNativeSystemInformation");
			nNtStatus = fnRtlGetNativeSystemInformation((SYSTEM_INFORMATION_CLASS)SystemProcessorInformation, &sProcInfo, sizeof(sProcInfo), NULL);
			if (nNtStatus == STATUS_NOT_IMPLEMENTED)
			{
				tNtQuerySystemInformation fnQuerySystemInformation = (tNtQuerySystemInformation)Utils::GetProcAddress(Utils::GetLocalModuleHandle("ntdll.dll"), "NtQuerySystemInformation");
				nNtStatus = fnQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemProcessorInformation, &sProcInfo, sizeof(sProcInfo), NULL);
			}
			if (NT_SUCCESS(nNtStatus))
				InterlockedExchange(&nProcessorArchitecture, (LONG)(sProcInfo.ProcessorArchitecture));
		}
		return nProcessorArchitecture;
	}

	enum PlatformType
	{
		UnknownPlatform = 0,
		ProcessPlatformX86 = 1,
		ProcessPlatformX64 = 2
	};

	static int GetProcessPlatform(HANDLE hProcess)
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
	
			static HMODULE hNtDll = (HMODULE)Utils::GetLocalModuleHandle("ntdll.dll");
			static tNtQueryInformationProcess fnNTQIP = (tNtQueryInformationProcess)Utils::GetProcAddress(hNtDll, "NtQueryInformationProcess");

			nNtStatus = fnNTQIP(hProcess, ProcessWow64Information, &nWow64, sizeof(nWow64), NULL);
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

	static HANDLE NtCreateThreadEx(HANDLE hProcess, LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD* threadId)
	{
		tNtCreateThreadEx NtCreateThreadEx = (tNtCreateThreadEx)Utils::GetProcAddress(Utils::GetLocalModuleHandle("ntdll.dll"), "NtCreateThreadEx");
		if (NtCreateThreadEx == NULL)
			return NULL;

		PS_ATTRIBUTE_LIST attrList;
		ZeroMemory(&attrList, sizeof(PS_ATTRIBUTE_LIST));
		CLIENT_ID cid;
		ZeroMemory(&cid, sizeof(CLIENT_ID));
		OBJECT_ATTRIBUTES64 thrdAttr;
		InitializeObjectAttributes64(&thrdAttr, NULL, 0, NULL, NULL);

		attrList.Attributes[0].Attribute = ProcThreadAttributeValue(PsAttributeClientId, TRUE, FALSE, FALSE);
		attrList.Attributes[0].Size = sizeof(CLIENT_ID);
		attrList.Attributes[0].ValuePtr = (ULONG_PTR*)&cid;

		attrList.TotalLength = sizeof(PS_ATTRIBUTE_LIST);

		HANDLE hRemoteThread = NULL;
		HRESULT hRes = 0;

		if (!NT_SUCCESS(NtCreateThreadEx(&hRemoteThread, THREAD_ALL_ACCESS, NULL, hProcess, lpRemoteThreadStart, lpParam, 0, 0, 0x1000, 0x100000, &attrList)))
			return NULL;

		if (threadId)
			*threadId = (DWORD)cid.UniqueThread;

		return hRemoteThread;
	}

};