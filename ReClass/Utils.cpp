#include "stdafx.h"
#include "Utils.h"

namespace Utils {

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
    
    if (ModuleBase == NULL)
        ModuleBase = LoadLibraryW( ModuleName );

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
    PIMAGE_DOS_HEADER DosHdr;
    PIMAGE_NT_HEADERS NtHdr;
    PIMAGE_EXPORT_DIRECTORY ExportDirectory;
    ULONG ExportSize;
    PUSHORT OrdsTable;
    PULONG NamesTable;
    PULONG FuncsTable;
    
    ULONG_PTR Address = 0;

    if (!ModuleBase)
        return NULL;

    DosHdr = (PIMAGE_DOS_HEADER)ModuleBase;
    if (DosHdr->e_magic != IMAGE_DOS_SIGNATURE)
        return NULL;
    NtHdr = (PIMAGE_NT_HEADERS)((PCHAR)ModuleBase + DosHdr->e_lfanew);
    if (NtHdr->Signature != IMAGE_NT_SIGNATURE) // Not a PE file
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

    OrdsTable  = (PUSHORT)((ULONG_PTR)ModuleBase + ExportDirectory->AddressOfNameOrdinals);
    NamesTable = (PULONG) ((ULONG_PTR)ModuleBase + ExportDirectory->AddressOfNames);
    FuncsTable = (PULONG) ((ULONG_PTR)ModuleBase + ExportDirectory->AddressOfFunctions);

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
            pName = (PCHAR)((ULONG_PTR)ModuleBase + NamesTable[i]);
            OrdIndex = OrdsTable[i];
        }
        // Weird params
        else
        {
            return NULL;
        }

        if (((ULONG_PTR)ProcName <= 0xFFFF && (USHORT)ProcName == OrdIndex + ExportDirectory->Base) ||
            ((ULONG_PTR)ProcName > 0xFFFF && strcmp( pName, ProcName ) == 0))
        {
            // Found export address
            Address = (ULONG_PTR)ModuleBase + FuncsTable[OrdIndex];

            // Check forwarded export
            if (Address >= (ULONG_PTR)ExportDirectory && Address <= (ULONG_PTR)ExportDirectory + ExportSize)
            {
                PCHAR pszForwarder = NULL;
                PCHAR pszImport = NULL;
                CHAR szDllName[256] = { 0 };
                PVOID ForwardBase = NULL;

                // Duplicate string at address
                pszForwarder = _strdup( (PCHAR)Address );

                // Get import name
                pszImport = strchr( pszForwarder, '.' );
                *pszImport++ = '\0';

                // Get DLL name
                strcpy_s( szDllName, 256, pszForwarder );
                strcat_s( szDllName, 256, ".dll" );

                // Get forwarded module base 
                ForwardBase = GetLocalModuleBaseA( szDllName );
                if (!ForwardBase) // Not found
                {
                    free( pszForwarder );
                    return NULL;
                }

                if (strchr( pszImport, '#' ) == NULL)
                {
                    // forwarded by name
                    Address = (ULONG_PTR)GetLocalProcAddressA( ForwardBase, pszImport );
                }
                else
                {
                    // forwarded by ordinal
                    PCHAR FuncOrd = (PCHAR)atol( pszImport + 1 );
                    Address = (ULONG_PTR)GetLocalProcAddressA( ForwardBase, FuncOrd );
                }

                free( pszForwarder );
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


bool SetPrivilege( HANDLE hToken, LPCTSTR lpPrivilege, bool bEnablePrivilege )
{
    NTSTATUS Status;
    LUID privLuid;
    TOKEN_PRIVILEGES tokenPriv;
    TOKEN_PRIVILEGES tokenPrivPrevious;
    DWORD cbPrevious;
    
    if (!LookupPrivilegeValue( NULL, lpPrivilege, &privLuid ))
        return false;

    ZeroMemory( &tokenPriv, sizeof( tokenPriv ) );
    tokenPriv.PrivilegeCount = 1;
    tokenPriv.Privileges[0].Luid = privLuid;
    tokenPriv.Privileges[0].Attributes = 0;

    cbPrevious = sizeof( TOKEN_PRIVILEGES );
    Status = ntdll::NtAdjustPrivilegesToken( hToken, FALSE, &tokenPriv, sizeof( TOKEN_PRIVILEGES ), &tokenPrivPrevious, &cbPrevious );
    if (!NT_SUCCESS( Status ))
        return false;

    tokenPrivPrevious.PrivilegeCount = 1;
    tokenPrivPrevious.Privileges[0].Luid = privLuid;
    if (bEnablePrivilege)
        tokenPrivPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    else
        tokenPrivPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tokenPrivPrevious.Privileges[0].Attributes);

    Status = ntdll::NtAdjustPrivilegesToken( hToken, FALSE, &tokenPrivPrevious, cbPrevious, NULL, NULL );
    if (!NT_SUCCESS( Status ))
        return false;

    return true;
}

bool SetDebugPrivilege( bool bEnable )
{
    NTSTATUS Status;
    HANDLE hToken;
    
    Status = ntdll::NtOpenProcessToken( GetCurrentProcess( ), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken );
    if (!NT_SUCCESS( Status ))
        return FALSE;

    // Enable/Disable Debug Privilege
    if (!SetPrivilege( hToken, SE_DEBUG_NAME, bEnable ))
    {
        CloseHandle( hToken );
        return false;
    }

    CloseHandle( hToken );
    return true;
}


bool IsWindowsVersionOrLater( OSType target )
{
    DWORDLONG Mask = 0;
    RTL_OSVERSIONINFOEXW versionInfo;

    ZeroMemory( &versionInfo, sizeof( versionInfo ) );
    versionInfo.dwOSVersionInfoSize = sizeof( RTL_OSVERSIONINFOEXW );

    if (target == Windows10)
    {
        if (ntdll::RtlGetVersion && ntdll::RtlGetVersion( (PRTL_OSVERSIONINFOW)&versionInfo ) == 0)
            return (versionInfo.dwMajorVersion == 10);

        return false;
    }

    if (target >= WinVista)
    {
        versionInfo.dwMajorVersion = 6;

        switch (target)
        {
        case WinVista:  versionInfo.dwMinorVersion = 0; break;
        case Windows7:  versionInfo.dwMinorVersion = 1; break;
        case Windows8:  versionInfo.dwMinorVersion = 2; break;
        default: 
            break;
        }
    }
    else
    {
        versionInfo.dwMajorVersion = 5;
        versionInfo.dwMinorVersion = (target >= WinXP) ? 1 : 0;
    }

    VER_SET_CONDITION( Mask, VER_MAJORVERSION, VER_GREATER_EQUAL );
    VER_SET_CONDITION( Mask, VER_MINORVERSION, VER_GREATER_EQUAL );
    VER_SET_CONDITION( Mask, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL );
    VER_SET_CONDITION( Mask, VER_SERVICEPACKMINOR, VER_GREATER_EQUAL );
    return VerifyVersionInfoW( &versionInfo, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR, Mask ) != FALSE;
}

OSType GetOperatingSystemType( )
{
    const OSType osTypes[] = { Windows10, Windows8, Windows7, WinVista, WinXP, Win2000 };
    for (int i = 0; i < _countof( osTypes ); ++i)
    {
        if (IsWindowsVersionOrLater( osTypes[i] ))
            return osTypes[i];
    }
    return UnknownOS;
}

LONG GetProcessorArchitecture( )
{
    static LONG volatile nProcessorArchitecture = -1;

    if (nProcessorArchitecture == -1)
    {
        SYSTEM_PROCESSOR_INFORMATION sProcInfo;
        NTSTATUS Status = STATUS_NOT_IMPLEMENTED;

        if (ntdll::RtlGetNativeSystemInformation != NULL)
        {
            Status = ntdll::RtlGetNativeSystemInformation( SystemProcessorInformation, &sProcInfo, sizeof( sProcInfo ), NULL );
        }

        if (Status == STATUS_NOT_IMPLEMENTED)
        {
            Status = ntdll::NtQuerySystemInformation( SystemProcessorInformation, &sProcInfo, sizeof( sProcInfo ), NULL );
        }

        if (NT_SUCCESS( Status ))
        {
            InterlockedExchange( &nProcessorArchitecture, (LONG)(sProcInfo.ProcessorArchitecture) );
        }
    }
    return nProcessorArchitecture;
}

int GetProcessPlatform( HANDLE hProcess )
{
    PVOID Wow64Info;
    NTSTATUS Status;

    if (hProcess == (HANDLE)((LONG_PTR)-1))
    {
        #if defined(_M_X64)
        return ProcessPlatformX64;
        #else
        return ProcessPlatformX86;
        #endif
    }

    switch (Utils::GetProcessorArchitecture( ))
    {
    case PROCESSOR_ARCHITECTURE_INTEL:
        return ProcessPlatformX86;
    case PROCESSOR_ARCHITECTURE_AMD64:
        //check on 64-bit platforms
        Status = ntdll::NtQueryInformationProcess( hProcess, ProcessWow64Information, &Wow64Info, sizeof( Wow64Info ), NULL );
        if (NT_SUCCESS( Status ))
        {
            #if defined(_M_X64)
            return (Wow64Info != NULL) ? ProcessPlatformX86 : ProcessPlatformX64;      	
            #else
            return (Wow64Info == NULL) ? ProcessPlatformX64 : ProcessPlatformX86;
            #endif
        }

        #if defined(_M_X64)
        return ProcessPlatformX64;      
        #else
        return ProcessPlatformX86;
        #endif

    //case PROCESSOR_ARCHITECTURE_IA64:
    //case PROCESSOR_ARCHITECTURE_ALPHA64:
    }

    return ProcessPlatformNotSupported;
}

HANDLE NtCreateThreadEx( HANDLE hProcess, LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD createFlags, DWORD* threadId )
{
    PS_ATTRIBUTE_LIST attrList;
    CLIENT_ID clientId;
    HANDLE hThread;

    if (ntdll::NtCreateThreadEx == NULL)
        return NULL;

    ZeroMemory( &attrList, sizeof( attrList ) );
    attrList.Attributes[0].Attribute = ProcThreadAttributeValue( PsAttributeClientId, TRUE, FALSE, FALSE );
    attrList.Attributes[0].Size = sizeof( CLIENT_ID );
    attrList.Attributes[0].ValuePtr = (ULONG_PTR *)&clientId;
    attrList.TotalLength = sizeof( PS_ATTRIBUTE_LIST );

    if (!NT_SUCCESS( ntdll::NtCreateThreadEx( &hThread, THREAD_ALL_ACCESS, NULL, hProcess, lpRemoteThreadStart, lpParam, createFlags, 0, 0x1000, 0x100000, &attrList ) ))
        return NULL;

    if (threadId) 
        *threadId = (DWORD)clientId.UniqueThread;

    return hThread;
}

HANDLE NtCreateThread( LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD createFlags, DWORD* threadId )
{
    return NtCreateThreadEx( NtCurrentProcess( ), lpRemoteThreadStart, lpParam, createFlags, threadId );
}

}