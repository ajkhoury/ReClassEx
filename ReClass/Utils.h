#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "nt_ddk.h"

namespace Utils
{
	// forward declarations
	static void* GetLocalProcAddressA(HMODULE module, LPCSTR proc_name);
	static void* GetLocalProcAddressW(HMODULE module, LPCWSTR proc_name);
	static HMODULE GetLocalModuleHandleA(LPCSTR moduleName);
	static HMODULE GetLocalModuleHandleW(LPCWSTR moduleName);

	//static HMODULE GetRemoteModuleHandle(const char* moduleName);
	//static void* GetRemoteProcAddress(HMODULE module, const char *proc_name); 

#ifdef _DEBUG
	/*
	*	Console API stuff
	*/
	static FILE* ConsoleStream = 0;
	static bool CreateDbgConsole(const TCHAR* lpConsoleTitle)
	{
		if (!ConsoleStream)
		{
			if (!AllocConsole())
				return false;
			errno_t ret = freopen_s(&ConsoleStream, "CONOUT$", "w", stdout);
			if (ret != 0)
				return false;
			SetConsoleTitle(lpConsoleTitle);
		}
		return true;
	}
	static void FreeDbgConsole()
	{
		if (ConsoleStream)
		{
			fclose(ConsoleStream);
			FreeConsole();
			ConsoleStream = 0;
		}
	}
#endif

	// Align value
	static inline size_t Align(size_t val, size_t alignment) {
		return (val % alignment == 0) ? val : (val / alignment + 1) * alignment;
	}

	static CString GetLastErrorString()
	{
		TCHAR buf[256];
		size_t size = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		return CString(buf, size);
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

	static HMODULE GetLocalModuleHandleW(LPCWSTR moduleName)
	{
		PVOID ModuleBase = 0;

		static PEB_T* peb = (PEB_T*)NtCurrentPeb();
		PEB_LDR_DATA_T* ldrData = (PEB_LDR_DATA_T*)peb->Ldr;
		PLDR_DATA_ENTRY cursor = (PLDR_DATA_ENTRY)ldrData->InInitializationOrderModuleList.Flink;
		while (cursor->BaseAddress)
		{
			wchar_t wcsBaseDllName[MAX_PATH] = { 0 };
			wcscpy_s(wcsBaseDllName, cursor->BaseDllName.Buffer);
			if (_wcsicmp(wcsBaseDllName, moduleName) == 0) 
			{ 
				ModuleBase = cursor->BaseAddress;
				break;
			}
			cursor = (PLDR_DATA_ENTRY)cursor->InMemoryOrderModuleList.Flink;
		}
		return (HMODULE)ModuleBase;
	}

	static HMODULE GetLocalModuleHandleA(LPCSTR moduleName)
	{
		wchar_t wcsModuleName[MAX_PATH] = { 0 };
		size_t converted = 0;
		mbstowcs_s(&converted, wcsModuleName, moduleName, MAX_PATH);
		return GetLocalModuleHandleW(wcsModuleName);
	}

#ifdef UNICODE
#define GetLocalModuleHandle GetLocalModuleHandleW
#else
#define GetLocalModuleHandle GetLocalModuleHandleA
#endif

	static PVOID GetLocalProcAddressA(HMODULE module, LPCSTR proc_name)
	{
		PCHAR modb = (PCHAR)module;
		if (!modb)
			return NULL;

		IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)modb;
		IMAGE_NT_HEADERS *nt_headers = (IMAGE_NT_HEADERS *)((size_t)modb + dos_header->e_lfanew);

		IMAGE_OPTIONAL_HEADER *opt_header = &nt_headers->OptionalHeader;
		IMAGE_DATA_DIRECTORY *exp_entry = (IMAGE_DATA_DIRECTORY *)(&opt_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
		IMAGE_EXPORT_DIRECTORY *exp_dir = (IMAGE_EXPORT_DIRECTORY *)((size_t)modb + exp_entry->VirtualAddress);

		DWORD* func_table	= (DWORD*)((size_t)modb + exp_dir->AddressOfFunctions);
		WORD*  ord_table	= (WORD*)((size_t)modb + exp_dir->AddressOfNameOrdinals);
		DWORD* name_table	= (DWORD*)((size_t)modb + exp_dir->AddressOfNames);

		PVOID address = NULL;
		DWORD i = 0;

		if (((ULONG_PTR)proc_name >> 16) == 0) // Ordinal
		{
			WORD ordinal = LOWORD(proc_name);
			ULONG_PTR ord_base = exp_dir->Base;
			if (ordinal < ord_base || ordinal > ord_base + exp_dir->NumberOfFunctions)
				return NULL;
			address = (PVOID)((size_t)modb + func_table[ordinal - ord_base]);
		}
		else
		{
			// import by name 
			for (i = 0; i < exp_dir->NumberOfNames; i++)
			{
				// name table pointers are rvas
				PCHAR procEntryName = (PCHAR)((size_t)modb + name_table[i]);
				if (_stricmp(proc_name, procEntryName) == 0)
				{
					address = (PVOID)((size_t)modb + func_table[ord_table[i]]);
					break;
				}
			}
		}
		// forwarded
		if ((PCHAR)address >= (PCHAR)exp_dir && (PCHAR)address < (PCHAR)exp_dir + exp_entry->Size)
		{
			HMODULE frwd_module = 0;
			PCHAR dll_name = 0;
			PCHAR func_name = 0;
			CHAR dllName[MAX_PATH] = { 0 };

			dll_name = _strdup((PCHAR)address);
			if (!dll_name)
				return NULL;

			func_name = strchr(dll_name, '.');
			if (!func_name) // Something is wrong...
			{
				free(dll_name);
				return NULL;
			}
			*func_name++ = '\0';

			strcpy_s(dllName, dll_name);
			strcat_s(dllName, strlen(dll_name) + 4 + 1, ".dll");
			free(dll_name); // Free dll_name as we dont need it anymore

			address = 0;

			frwd_module = (HMODULE)Utils::GetLocalModuleHandleA(dllName);
			if (!frwd_module)
				frwd_module = LoadLibraryA(dllName);
			if (!frwd_module)
			{
				#ifdef _DEBUG
				MessageBox(0, _T("Utils::GetProcAddress failed to load module using Utils::GetLocalModuleHandle and LoadLibrary!"), _T("Reclass 2015"), MB_ICONERROR);
				#endif
				return NULL;
			}

			bool forwardByOrd = strchr(func_name, '#') == 0 ? false : true;
			if (forwardByOrd) // forwarded by ordinal
			{
				WORD func_ord = (WORD)atol(func_name + 1);
				address = Utils::GetLocalProcAddressA(frwd_module, (const char*)func_ord);
			}
			else
			{
				address = Utils::GetLocalProcAddressA(frwd_module, func_name);
			}
		}
		return address;
	}

	static PVOID GetLocalProcAddressW(HMODULE module, LPCWSTR proc_name)
	{
		char szProcName[MAX_PATH] = { 0 };
		size_t converted = 0;
		wcstombs_s(&converted, szProcName, proc_name, MAX_PATH);
		return GetLocalProcAddressA(module, szProcName);
	}

#ifdef UNICODE
#define GetLocalProcAddress GetLocalProcAddressW
#else
#define GetLocalProcAddress GetLocalProcAddressA
#endif

	static size_t FindPattern(size_t start_offset, DWORD size, unsigned char pattern[], int n)
	{
		//printf("n = %i\n", n);
		char mask[256];
		for (int i = 0; i <= n; i++)
		{
			if (i == n) mask[i] = '\0';
			else if (pattern[i] == 0xCC) mask[i] = '?';
			else mask[i] = 'x';
		}

		int pos = 0;
		int searchLen = (int)strlen(mask) - 1;

		for (size_t retAddress = start_offset; retAddress < start_offset + size; retAddress++)
		{
			if (*(BYTE*)retAddress == pattern[pos] || mask[pos] == '?')
			{
				if (mask[pos + 1] == '\0')
					return (retAddress - searchLen);
				pos++;
			}
			else
				pos = 0;
		}
		return NULL;
	}

	template<int N> __inline size_t FindPattern(size_t start_offset, DWORD size, unsigned char(&pattern)[N])
	{
		return FindPattern(start_offset, size, pattern, N);
	}

	static CString GetVersionInfo(const TCHAR* versionKey)
	{
		CString strResult;
		HRSRC hVersion = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), VS_FILE_INFO);
		if (hVersion != NULL)
		{
			HGLOBAL hGlobal = LoadResource(NULL, hVersion);
			if (hGlobal != NULL)
			{
				LPVOID versionInfo = LockResource(hGlobal);
				if (versionInfo != NULL)
				{
					struct LANGANDCODEPAGE {
						WORD wLanguage;
						WORD wCodePage;
					} *lpTranslate;

					UINT uiSize;

					if (VerQueryValue(versionInfo, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &uiSize) && uiSize > 0)
					{
						// Version information
						BYTE* lpb;
						CString strQuery;
						strQuery.Format(_T("\\StringFileInfo\\%04x%04x\\%s"), lpTranslate->wLanguage, lpTranslate->wCodePage, versionKey);
						if (VerQueryValue(versionInfo, strQuery.GetString(), (LPVOID*)&lpb, &uiSize) && uiSize > 0)
						{
							strResult = (LPCTSTR)lpb;
						}
					}
				}
			}

			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}

		return strResult;
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

	enum PlatformType
	{
		UnknownPlatform = 0,
		ProcessPlatformX86 = 1,
		ProcessPlatformX64 = 2
	};

	bool IsWindowsVersionOrLater(OSType target);
	LONG GetProcessorArchitecture();
	OSType GetOperatingSystemType();
	int GetProcessPlatform(HANDLE hProcess);
	HANDLE NtCreateThreadEx(HANDLE hProcess, LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD* threadId);
};