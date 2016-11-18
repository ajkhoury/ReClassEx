#include "stdafx.h"
#include "Utils.h"

namespace Utils
{

PVOID GetLocalModuleBaseW( LPCWSTR ModuleName )
{
	PVOID ModuleBase = 0;

	#if defined(_WIN64)
	TEB* teb = (TEB*)ReadGS( FIELD_OFFSET( NT_TIB, Self ) );
	#else
	TEB* teb = (TEB*)ReadFS( PcTeb );
	#endif
	PVOID peb = (PVOID)teb->ProcessEnvironmentBlock;
	#if defined(_WIN64)
	PPEB_LDR_DATA LdrData = (PPEB_LDR_DATA)((PPEB64)peb)->Ldr;
	#else
	PPEB_LDR_DATA LdrData = (PPEB_LDR_DATA)((PPEB32)peb)->Ldr;
	#endif

	// Search in InLoadOrderModuleList
	for (PLIST_ENTRY pListEntry = LdrData->InLoadOrderModuleList.Flink; pListEntry != &LdrData->InLoadOrderModuleList; pListEntry = pListEntry->Flink)
	{
		PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD( pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );
		if (pEntry != NULL)
		{
			wchar_t wcsBaseDllName[MAX_PATH] = { 0 };
			wcscpy_s( wcsBaseDllName, pEntry->BaseDllName.Buffer );
			if (_wcsicmp( wcsBaseDllName, ModuleName ) == 0)
			{
				ModuleBase = pEntry->DllBase;
				break;
			}
		}
	}
	return ModuleBase;
}

PVOID GetLocalModuleBaseA( LPCSTR ModuleName )
{
	wchar_t wcsModuleName[MAX_PATH] = { 0 };
	size_t converted = 0;
	mbstowcs_s( &converted, wcsModuleName, ModuleName, MAX_PATH );
	return GetLocalModuleBaseW( wcsModuleName );
}

PVOID GetLocalProcAddressA( PVOID ModuleBase, PCHAR ProcName )
{
	PIMAGE_NT_HEADERS NtHdr = NULL;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory = NULL;
	ULONG ExportSize = 0;
	ULONG_PTR Address = 0;

	if (ModuleBase == NULL)
		return NULL;

	NtHdr = (PIMAGE_NT_HEADERS)ImageNtHeader( ModuleBase );
	if (!NtHdr) // Not a PE file
		return NULL;

	// 64 bit image
	if (NtHdr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(((PIMAGE_NT_HEADERS64)NtHdr)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress + (ULONG_PTR)ModuleBase);
		ExportSize = ((PIMAGE_NT_HEADERS64)NtHdr)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	}
	// 32 bit image
	else
	{
		ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(((PIMAGE_NT_HEADERS32)NtHdr)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress + (ULONG_PTR)ModuleBase);
		ExportSize = ((PIMAGE_NT_HEADERS32)NtHdr)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	}

	PUSHORT OrdsTable = (PUSHORT)(ExportDirectory->AddressOfNameOrdinals + (ULONG_PTR)ModuleBase);
	PULONG  NamesTable = (PULONG)(ExportDirectory->AddressOfNames + (ULONG_PTR)ModuleBase);
	PULONG  FuncsTable = (PULONG)(ExportDirectory->AddressOfFunctions + (ULONG_PTR)ModuleBase);

	for (ULONG i = 0; i < ExportDirectory->NumberOfFunctions; ++i)
	{
		USHORT OrdIndex = 0xFFFF;
		PCHAR  pName = NULL;

		// Find by index
		if ((ULONG_PTR)ProcName <= 0xFFFF)
		{
			OrdIndex = (USHORT)i;
		}
		// Find by name
		else if ((ULONG_PTR)ProcName > 0xFFFF && i < ExportDirectory->NumberOfNames)
		{
			pName = (PCHAR)(NamesTable[i] + (ULONG_PTR)ModuleBase);
			OrdIndex = OrdsTable[i];
		}
		// Weird params
		else
			return NULL;

		if (((ULONG_PTR)ProcName <= 0xFFFF && (USHORT)((ULONG_PTR)ProcName) == OrdIndex + ExportDirectory->Base) ||
			((ULONG_PTR)ProcName > 0xFFFF && strcmp( pName, ProcName ) == 0))
		{
			// Found export address
			Address = FuncsTable[OrdIndex] + (ULONG_PTR)ModuleBase;

			// Check forwarded export
			if (Address >= (ULONG_PTR)ExportDirectory && Address <= (ULONG_PTR)ExportDirectory + ExportSize)
			{
				PCHAR forwarder = NULL;
				PCHAR import = NULL;
				CHAR dllName[256] = { 0 };
				PVOID forwardBase = NULL;

				// Duplicate string at address
				forwarder = _strdup( (PCHAR)Address );

				// Get import name
				import = strchr( forwarder, '.' );
				*import++ = '\0';

				// Get DLL name
				strcpy_s( dllName, 256, (PCHAR)forwarder );
				strcat_s( dllName, 256, ".dll" );

				// Get forwarded module base 
				forwardBase = GetLocalModuleBaseA( dllName );
				if (!forwardBase)
					forwardBase = LoadLibraryA( dllName );
				if (!forwardBase) // Not found
				{
					free( forwarder );
					return NULL;
				}

				if (strchr( import, '#' ) == NULL)
				{
					// forwarded by name
					Address = (ULONG_PTR)GetLocalProcAddressA( forwardBase, import );
				}
				else
				{
					// forwarded by ordinal
					PCHAR FuncOrd = (PCHAR)atoll( import + 1 );
					Address = (ULONG_PTR)GetLocalProcAddressA( forwardBase, FuncOrd );
				}

				free( forwarder );
			}

			break;

		}
	}

	return (PVOID)Address;
}

PVOID GetLocalProcAddressW( PVOID ModuleBase, LPCWSTR ProcName )
{
	char szProcName[MAX_PATH] = { 0 };
	size_t converted = 0;
	wcstombs_s( &converted, szProcName, ProcName, MAX_PATH );
	return GetLocalProcAddressA( ModuleBase, szProcName );
}


BOOLEAN SetPrivilege( HANDLE hToken, LPCTSTR lpPrivilege, BOOLEAN bEnablePrivilege )
{
	TOKEN_PRIVILEGES    tkp = { 0 };
	LUID                luid = { 0 };
	TOKEN_PRIVILEGES    tkpPrevious = { 0 };
	DWORD				cbPrevious = 0;
	NTSTATUS			status = STATUS_SUCCESS;
	//
	if (!LookupPrivilegeValue( NULL, lpPrivilege, &luid ))
		return FALSE;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = 0;
	cbPrevious = sizeof( TOKEN_PRIVILEGES );

	status = ntdll::NtAdjustPrivilegesToken( hToken, FALSE, &tkp, sizeof( TOKEN_PRIVILEGES ), &tkpPrevious, &cbPrevious );

	if (!NT_SUCCESS( status ))
		return FALSE;

	tkpPrevious.PrivilegeCount = 1;
	tkpPrevious.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tkpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	else
		tkpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tkpPrevious.Privileges[0].Attributes);

	status = ntdll::NtAdjustPrivilegesToken( hToken, FALSE, &tkpPrevious, cbPrevious, NULL, NULL );

	if (!NT_SUCCESS( status ))
		return FALSE;

	return TRUE;
}

BOOLEAN SetDebugPrivilege( BOOLEAN bEnable )
{
	HANDLE hToken = NULL;
	NTSTATUS status = ntdll::NtOpenProcessToken( GetCurrentProcess( ), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken );
	if (!NT_SUCCESS( status ))
		return FALSE;
	// Enable/Disable Debug Privilege
	if (!SetPrivilege( hToken, SE_DEBUG_NAME, bEnable ))
	{
		CloseHandle( hToken );
		return FALSE;
	}
	CloseHandle( hToken );
	return TRUE;
}


BOOLEAN IsWindowsVersionOrLater( OSType target )
{
	if (target == Windows10)
	{
		RTL_OSVERSIONINFOEXW verInfo = { 0 };
		verInfo.dwOSVersionInfoSize = sizeof( verInfo );

		if (ntdll::RtlGetVersion != nullptr && ntdll::RtlGetVersion( (PRTL_OSVERSIONINFOW)&verInfo ) == 0)
		{
			return (verInfo.dwMajorVersion == 10);
		}
		return FALSE;
	}

	OSVERSIONINFOEX info;
	memset( &info, 0, sizeof( OSVERSIONINFOEX ) );
	info.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

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

	VER_SET_CONDITION( mask, VER_MAJORVERSION, VER_GREATER_EQUAL );
	VER_SET_CONDITION( mask, VER_MINORVERSION, VER_GREATER_EQUAL );
	VER_SET_CONDITION( mask, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL );
	VER_SET_CONDITION( mask, VER_SERVICEPACKMINOR, VER_GREATER_EQUAL );

	return VerifyVersionInfo( &info, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR, mask ) != FALSE;
}

OSType GetOperatingSystemType( )
{
	const OSType types[] = { Windows10, Windows8, Windows7, WinVista, WinXP, Win2000 };
	for (int i = 0; i < _countof( types ); ++i)
		if (IsWindowsVersionOrLater( types[i] ))
			return types[i];
	return UnknownOS;
}

LONG GetProcessorArchitecture( )
{
	static LONG volatile nProcessorArchitecture = -1;
	if (nProcessorArchitecture == -1)
	{
		SYSTEM_PROCESSOR_INFORMATION sProcInfo;
		NTSTATUS nNtStatus;

		nNtStatus = ntdll::RtlGetNativeSystemInformation != nullptr ?
			ntdll::RtlGetNativeSystemInformation( (SYSTEM_INFORMATION_CLASS)SystemProcessorInformation, &sProcInfo, sizeof( sProcInfo ), NULL ) : STATUS_NOT_IMPLEMENTED;
		if (nNtStatus == STATUS_NOT_IMPLEMENTED)
		{
			//tNtQuerySystemInformation fnQuerySystemInformation = (tNtQuerySystemInformation)Utils::GetLocalProcAddress(Utils::GetLocalModuleHandle("ntdll.dll"), "NtQuerySystemInformation");
			nNtStatus = ntdll::NtQuerySystemInformation( (SYSTEM_INFORMATION_CLASS)SystemProcessorInformation, &sProcInfo, sizeof( sProcInfo ), NULL );
		}
		if (NT_SUCCESS( nNtStatus ))
			InterlockedExchange( &nProcessorArchitecture, (LONG)(sProcInfo.ProcessorArchitecture) );
	}
	return nProcessorArchitecture;
}

int GetProcessPlatform( HANDLE hProcess )
{
	if (hProcess == (HANDLE)((LONG_PTR)-1))
	{
		#if defined(_M_IX86)
		return 1; // ProcessPlatformX86;
		#elif defined(_M_X64)
		return 2; // ProcessPlatformX64
		#endif
	}
	switch (Utils::GetProcessorArchitecture( ))
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		return ProcessPlatformX86;
	case PROCESSOR_ARCHITECTURE_AMD64:
		//check on 64-bit platforms
		ULONG_PTR nWow64;
		NTSTATUS nNtStatus;

		nNtStatus = ntdll::NtQueryInformationProcess( hProcess, ProcessWow64Information, &nWow64, sizeof( nWow64 ), NULL );
		if (NT_SUCCESS( nNtStatus ))
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

HANDLE NtCreateThreadEx( HANDLE hProcess, LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD createFlags, DWORD* threadId )
{
	if (ntdll::NtCreateThreadEx == nullptr)
		return NULL;

	PS_ATTRIBUTE_LIST attrList = { 0 };
	CLIENT_ID cid = { 0 };

	attrList.Attributes[0].Attribute = ProcThreadAttributeValue( PsAttributeClientId, TRUE, FALSE, FALSE );
	attrList.Attributes[0].Size = sizeof( CLIENT_ID );
	attrList.Attributes[0].ValuePtr = (ULONG_PTR *)&cid;
	attrList.TotalLength = sizeof( PS_ATTRIBUTE_LIST );

	HANDLE hRemoteThread = NULL;
	HRESULT hRes = 0;

	if (!NT_SUCCESS( ntdll::NtCreateThreadEx( &hRemoteThread, THREAD_ALL_ACCESS, NULL, hProcess, lpRemoteThreadStart, lpParam, createFlags, 0, 0x1000, 0x100000, &attrList ) ))
		return NULL;

	if (threadId) *threadId = (DWORD)cid.UniqueThread;

	return hRemoteThread;
}

HANDLE NtCreateThread( LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD createFlags, DWORD* threadId = NULL )
{
	return NtCreateThreadEx( GetCurrentProcess( ), lpRemoteThreadStart, lpParam, createFlags, threadId );
}

}