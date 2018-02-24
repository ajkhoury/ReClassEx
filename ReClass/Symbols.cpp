#include "stdafx.h"
#include "Symbols.h"



#include <fstream>

Symbols::Symbols( ) :
	m_bInitialized( FALSE )
{
	ResolveSearchPath( );

	if (!WriteSymSrvDll( ))
		throw std::exception( "WriteSymSrvDll_failed" );

	if (!Init( ))
		throw std::exception( "init_failed" );

	ntdll::RtlInitializeCriticalSection( &m_CriticalSection );
}

Symbols::~Symbols( )
{
	ntdll::RtlDeleteCriticalSection( &m_CriticalSection );

	Cleanup( );
	DeleteFile( _T( "symsrv.dll" ) );
}

void Symbols::ResolveSearchPath( )
{
	CString searchPath;
	LRESULT lRet;
	HKEY hKey = NULL;

	//C:\Users\User\AppData\Local\Temp\SymbolCache
	for (int i = 14; i >= 8; i--)
	{
		CString regPath = _T( "Software\\Microsoft\\VisualStudio\\" );
		wchar_t version[4];
		_itow_s( i, version, 10 );

		#ifdef UNICODE
		regPath.Append( version );
		#else
		regPath.Append( CW2A( version ) );
		#endif

		regPath.Append( _T( ".0\\Debugger" ) );

		lRet = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.GetString( ), 0, KEY_READ, &hKey );
		if (hKey)
		{
			TCHAR szBuffer[MAX_PATH];
			DWORD dwBufferSize = MAX_PATH;
			lRet = RegQueryValueEx( hKey, _T( "SymbolCacheDir" ), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize );
			if (lRet == ERROR_SUCCESS && szBuffer)
			{
				searchPath = szBuffer;
				RegCloseKey( hKey );
				break;
			}
			RegCloseKey( hKey );
		}
	}

	if (!searchPath.IsEmpty( ))
	{
		m_strSearchPath.Format( _T( "srv*%s*http://msdl.microsoft.com/download/symbols" ), searchPath.GetString( ) );
		PrintOut( _T( "Symbol server path found from Visual Studio config: %s" ), searchPath.GetString( ) );
	}
	else
	{
		TCHAR szWindowsDir[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, szWindowsDir );
		m_strSearchPath.Format( _T( "srv*%s\\symbols*http://msdl.microsoft.com/download/symbols" ), szWindowsDir );
		PrintOut( _T( "Symbol server path not found, using windows dir: %s" ), szWindowsDir );
	}
}

BOOLEAN Symbols::WriteSymSrvDll( )
{
	HRSRC hSymSrvRes = NULL;
	HGLOBAL hGlobal = NULL;
	LPVOID pSymSrvData = NULL;
	DWORD dwSymSrvDataSize = 0;

	#ifdef _WIN64
	hSymSrvRes = FindResource( NULL, MAKEINTRESOURCE( IDR_RCDATA_SYMSRV64 ), RT_RCDATA );
	#else
	hSymSrvRes = FindResource( NULL, MAKEINTRESOURCE( IDR_RCDATA_SYMSRV32 ), RT_RCDATA );
	#endif
	if (hSymSrvRes != NULL)
	{
		hGlobal = LoadResource( NULL, hSymSrvRes );
		if (hGlobal != NULL)
		{
			pSymSrvData = LockResource( hGlobal );
			dwSymSrvDataSize = SizeofResource( NULL, hSymSrvRes );
			if (pSymSrvData != NULL)
			{
				std::ofstream fSymSrvDll( _T( "symsrv.dll" ), std::ios::binary );
				if (fSymSrvDll)
				{
					fSymSrvDll.write( (const char*)pSymSrvData, dwSymSrvDataSize );
					fSymSrvDll.close( );

					UnlockResource( hGlobal );
					FreeResource( hGlobal );

					return TRUE;
				}
			}
		}

		UnlockResource( hGlobal );
		FreeResource( hGlobal );
	}

	return FALSE;
}

void Symbols::Cleanup( )
{
	if (m_bInitialized == TRUE)
	{
		for (auto it = m_SymbolAddresses.begin( ); it != m_SymbolAddresses.end( ); ++it)
			delete it->second;
		m_SymbolAddresses.clear( );

		for (auto it = m_SymbolNames.begin( ); it != m_SymbolNames.end( ); ++it)
			delete it->second;
		m_SymbolNames.clear( );

		CoUninitialize( );

		m_bInitialized = FALSE;
	}
}

BOOLEAN Symbols::Init( )
{
	if (m_bInitialized == FALSE)
	{
		HRESULT hr = S_OK;
		hr = CoInitialize( NULL );
		if (FAILED( hr ))
		{
			PrintOut( _T( "[Symbols::Init] CoInitialize failed - HRESULT = %08X" ), hr );
			return FALSE;
		}
		m_bInitialized = TRUE;
	}
	return TRUE;
}

BOOLEAN Symbols::LoadSymbolsForModule( CString ModulePath, ULONG_PTR ModuleBaseAddress, ULONG SizeOfModule )
{
	int idx = -1;
	CString ModuleName;
	const TCHAR* szSearchPath = NULL;
	SymbolReader* reader = NULL;
	BOOLEAN bSucc = FALSE;

	idx = ModulePath.ReverseFind( '/' );
	if (idx == -1)
		idx = ModulePath.ReverseFind( '\\' );
	ModuleName = ModulePath.Mid( ++idx );

	if (!m_strSearchPath.IsEmpty( ))
		szSearchPath = m_strSearchPath.GetString( );

	reader = new SymbolReader( );

	//ntdll::RtlEnterCriticalSection( &m_CriticalSection );

	bSucc = reader->LoadFile( ModuleName, ModulePath, ModuleBaseAddress, SizeOfModule, szSearchPath );

	//ntdll::RtlLeaveCriticalSection( &m_CriticalSection );

	if (bSucc)
	{
		PrintOut( _T( "[Symbols::LoadSymbolsForModule] Symbols for module %s loaded" ), ModuleName.GetString( ) );
		m_SymbolAddresses.insert( std::make_pair( ModuleBaseAddress, reader ) );
		return TRUE;
	}

	delete reader;

	return FALSE;
}

BOOLEAN Symbols::LoadSymbolsForPdb( CString PdbPath )
{
	int idx = -1;
	CString PdbFileName;
	const TCHAR* szSearchPath = NULL;
	SymbolReader* reader = NULL;
	BOOLEAN bSucc = FALSE;

	idx = PdbPath.ReverseFind( '/' );
	if (idx == -1)
		idx = PdbPath.ReverseFind( '\\' );
	PdbFileName = PdbPath.Mid( ++idx );

	if (!m_strSearchPath.IsEmpty( ))
		szSearchPath = m_strSearchPath.GetString( );

	reader = new SymbolReader( );

	ntdll::RtlEnterCriticalSection( &m_CriticalSection );

	bSucc = reader->LoadFile( PdbFileName, PdbPath, 0, 0, szSearchPath );

	ntdll::RtlLeaveCriticalSection( &m_CriticalSection );

	if (bSucc)
	{
		PrintOut( _T( "[Symbols::LoadSymbolsForPdb] Symbols for module %s loaded" ), PdbFileName.GetString( ) );
		m_SymbolNames.insert( std::make_pair( PdbFileName, reader ) );
		return TRUE;
	}

	delete reader;

	return FALSE;
}

//void Symbols::LoadModuleSymbols()
//{
//	PPROCESS_BASIC_INFORMATION pbi = NULL;
//	PEB peb;
//	PEB_LDR_DATA peb_ldr;
//
//	// Try to allocate buffer 
//	HANDLE	hHeap = GetProcessHeap();
//	DWORD dwSize = sizeof(PROCESS_BASIC_INFORMATION);
//	pbi = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSize);
//
//	ULONG dwSizeNeeded = 0;
//	NTSTATUS dwStatus = ntdll::NtQueryInformationProcess(g_hProcess, ProcessBasicInformation, pbi, dwSize, &dwSizeNeeded);
//	if (NT_SUCCESS(dwStatus) && dwSize < dwSizeNeeded)
//	{
//		if (pbi)
//			HeapFree(hHeap, 0, pbi);
//
//		pbi = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSizeNeeded);
//		if (!pbi) {
//			_tprintf(_T("[LoadModuleSymbols] Couldn't allocate heap buffer!\n"));
//			return;
//		}
//
//		dwStatus = ntdll::NtQueryInformationProcess(g_hProcess, ProcessBasicInformation, pbi, dwSizeNeeded, &dwSizeNeeded);
//	}
//
//	// Did we successfully get basic info on process
//	if (NT_SUCCESS(dwStatus))
//	{
//		// Read Process Environment Block (PEB)
//		if (pbi->PebBaseAddress)
//		{
//			SIZE_T dwBytesRead = 0;
//			if (ReClassReadMemory(pbi->PebBaseAddress, &peb, sizeof(peb), &dwBytesRead))
//			{
//				dwBytesRead = 0;
//				if (ReClassReadMemory(peb.Ldr, &peb_ldr, sizeof(peb_ldr), &dwBytesRead))
//				{
//					ULONG numOfEntries = peb_ldr.Length;
//					ULONG currentEntryNum = 0;
//					LIST_ENTRY *pLdrListHead = (LIST_ENTRY *)peb_ldr.InMemoryOrderModuleList.Flink;
//					LIST_ENTRY *pLdrCurrentNode = peb_ldr.InMemoryOrderModuleList.Flink;
//					do
//					{
//						LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
//						dwBytesRead = 0;
//						if (!ReClassReadMemory((LPVOID)pLdrCurrentNode, &lstEntry, sizeof(LDR_DATA_TABLE_ENTRY), &dwBytesRead))
//						{
//							_tprintf(_T("[LoadModuleSymbols] Could not read list entry from LDR list. Error: %s\n"), Utils::GetLastErrorAsString().c_str());
//							if (pbi)
//								HeapFree(hHeap, 0, pbi);
//							return;
//						}
//
//						pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;
//
//						wchar_t wcsFullDllName[MAX_PATH] = { 0 };
//						if (lstEntry.FullDllName.Buffer && lstEntry.FullDllName.Length > 0)
//						{
//							dwBytesRead = 0;
//							if (!ReClassReadMemory((LPVOID)lstEntry.FullDllName.Buffer, &wcsFullDllName, lstEntry.FullDllName.Length, &dwBytesRead))
//							{
//								_tprintf(_T("[LoadModuleSymbols] Could not read list entry DLL name. Error: %s\n"), Utils::GetLastErrorAsString().c_str());
//								if (pbi)
//									HeapFree(hHeap, 0, pbi);
//								return;
//							}
//						}
//
//						if (lstEntry.DllBase != 0 && lstEntry.SizeOfImage != 0)
//						{
//							if (LoadSymbolsForModule(wcsFullDllName, (size_t)lstEntry.DllBase, lstEntry.SizeOfImage)) {
//								PrintOut(_T("Symbol module %ls loaded"), wcsFullDllName);
//							}
//							currentEntryNum++;
//							float progress = ((float)currentEntryNum / (float)numOfEntries) * 100;
//							printf("[%d/%d] progress: %f\n", currentEntryNum, numOfEntries, progress);
//						}
//					} while (pLdrListHead != pLdrCurrentNode);
//
//				} // Get Ldr
//			} // Read PEB 
//		} // Check for PEB
//	}
//
//	if (pbi)
//		HeapFree(hHeap, 0, pbi);
//}

SymbolReader* Symbols::GetSymbolsForModuleAddress( ULONG_PTR ModuleAddress )
{
	SymbolReader* script = NULL;
	auto iter = m_SymbolAddresses.find( ModuleAddress );
	if (iter != m_SymbolAddresses.end( ))
		script = iter->second;
	return script;
}

SymbolReader* Symbols::GetSymbolsForModuleName( CString ModuleName )
{
	SymbolReader* script = NULL;
	auto iter = m_SymbolNames.find( ModuleName );
	if (iter != m_SymbolNames.end( ))
		script = iter->second;
	return script;
}

typedef void*( CDECL * Alloc_t )(unsigned int);
typedef void( CDECL * Free_t )(void *);
typedef PCHAR( CDECL *PUNDNAME )(char *, const char *, int, Alloc_t, Free_t, unsigned short);

void *CDECL AllocIt( unsigned int cb ) { return HeapAlloc( GetProcessHeap( ), 0, cb ); }
void CDECL FreeIt( void * p ) { HeapFree( GetProcessHeap( ), 0, p ); }

DWORD
WINAPI
UndecorateSymbolName(
    LPCSTR name,
    LPSTR outputString,
    DWORD maxStringLength,
    DWORD flags
)
{
    static HMODULE hMsvcrt = NULL;
    static PUNDNAME pfUnDname = NULL;

    DWORD rc;

    if (!hMsvcrt)
    {
        hMsvcrt = (HMODULE)Utils::GetLocalModuleBase( _T( "msvcrt.dll" ) );
        if (hMsvcrt)
            pfUnDname = (PUNDNAME)Utils::GetLocalProcAddress( hMsvcrt, _T( "__unDName" ) );
    }

    rc = 0;
    
    if (pfUnDname) 
    {
        if (name && outputString && maxStringLength >= 2)
        {
            if (pfUnDname( outputString, name, maxStringLength - 1, AllocIt, FreeIt, (USHORT)flags ))
            {
                rc = strlen( outputString );
            }
        }     
    }
    else
    {
        rc = strlen( strncpy( outputString, "Unable to load msvcrt!__unDName", maxStringLength ) );
        SetLastError( ERROR_MOD_NOT_FOUND );
    }

    if (!rc)
        SetLastError( ERROR_INVALID_PARAMETER );

    return rc;
}

DWORD
WINAPI
UndecorateSymbolNameUnicode(
    LPCWSTR name,
    LPWSTR outputString,
    DWORD maxStringLength,
    DWORD flags
)
{
    LPSTR AnsiName;
    LPSTR AnsiOutputString;
    int AnsiNameLength;
    DWORD rc;

    AnsiNameLength = WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, name, -1, NULL, 0, NULL, NULL );
    if (!AnsiNameLength)
        return 0;

    AnsiName = (char *)AllocIt( AnsiNameLength );
    if (!AnsiName)
    {
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        return 0;
    }

    ZeroMemory( AnsiName, 0, AnsiNameLength );
    if (!WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK | WC_SEPCHARS, name, -1, AnsiName, AnsiNameLength, NULL, NULL ))
    {
        FreeIt( AnsiName );
        return 0;
    }

    AnsiOutputString = (LPSTR)AllocIt( maxStringLength + 1 );
    if (!AnsiOutputString)
    {
        FreeIt( AnsiName );
        SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        return 0;
    }

    *AnsiOutputString = '\0';
    rc = UndecorateSymbolName( AnsiName, AnsiOutputString, maxStringLength, flags );
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, AnsiOutputString, -1, outputString, maxStringLength );

    FreeIt( AnsiName );
    FreeIt( AnsiOutputString );

    return rc;
}


