#include "stdafx.h"

#include <memory>
#include <Psapi.h>

//Globals
HANDLE g_hProcess = NULL;
DWORD g_ProcessID = NULL;
ULONG_PTR g_AttachedProcessAddress = NULL;
DWORD g_AttachedProcessSize = NULL;
CString g_ProcessName;

std::vector<MemMapInfo> g_MemMap;
std::vector<MemMapInfo> g_MemMapCode;
std::vector<MemMapInfo> g_MemMapData;
std::vector<MemMapInfo> g_MemMapModules;
std::vector<AddressName> g_Exports;
std::vector<AddressName> g_CustomNames;

std::vector<HICON> g_Icons;

COLORREF g_crBackground = RGB( 255, 255, 255 );
COLORREF g_crSelect = RGB( 240, 240, 240 );
COLORREF g_crHidden = RGB( 240, 240, 240 );

COLORREF g_crOffset = RGB( 255, 0, 0 );
COLORREF g_crAddress = RGB( 0, 200, 0 );
COLORREF g_crType = RGB( 0, 0, 255 );
COLORREF g_crName = RGB( 32, 32, 128 );
COLORREF g_crIndex = RGB( 32, 200, 200 );
COLORREF g_crValue = RGB( 255, 128, 0 );
COLORREF g_crComment = RGB( 0, 200, 0 );

COLORREF g_crVTable = RGB( 0, 255, 0 );
COLORREF g_crFunction = RGB( 255, 0, 255 );
COLORREF g_crChar = RGB( 0, 0, 255 );
COLORREF g_crBits = RGB( 0, 0, 255 );
COLORREF g_crCustom = RGB( 64, 128, 64 );
COLORREF g_crHex = RGB( 0, 0, 0 );

CString g_ViewFontName;
CFont g_ViewFont;

int g_FontWidth;
int g_FontHeight;

bool g_bAddress = true;
bool g_bOffset = true;
bool g_bText = true;
bool g_bRTTI = true;
bool g_bRandomName = true;
bool g_bResizingFont = true;
bool g_bSymbolResolution = true;
bool g_bLoadModuleSymbol = false;

bool g_bFloat = true;
bool g_bInt = true;
bool g_bString = true;
bool g_bPointers = true;

bool g_bTop = true;
bool g_bClassBrowser = true;
bool g_bFilterProcesses = false;
bool g_bPrivatePadding = false;
bool g_bClipboardCopy = false;

CString g_tdHex;
CString g_tdInt64;
CString g_tdInt32;
CString g_tdInt16;
CString g_tdInt8;
CString g_tdQWORD;
CString g_tdDWORD;
CString g_tdWORD;
CString g_tdBYTE;
CString g_tdFloat;
CString g_tdDouble;
CString g_tdVec2;
CString g_tdVec3;
CString g_tdQuat;
CString g_tdMatrix;
CString g_tdPChar;
CString g_tdPWChar;

DWORD g_NodeCreateIndex = 0;

BOOL ReClassReadMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, PSIZE_T BytesRead )
{
	if (g_PluginOverrideReadMemory != NULL)
		return g_PluginOverrideReadMemory( Address, Buffer, Size, BytesRead );

	BOOL return_val = ReadProcessMemory( g_hProcess, (LPVOID)Address, Buffer, Size, BytesRead );
	if (!return_val) SecureZeroMemory( Buffer, Size );
	return return_val;
}

BOOL ReClassWriteMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, PSIZE_T BytesWritten )
{
	if (g_PluginOverrideWriteMemory != NULL)
		return g_PluginOverrideWriteMemory( Address, Buffer, Size, BytesWritten );

	DWORD OldProtect;
	VirtualProtectEx( g_hProcess, (void*)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect );
	BOOL ret = WriteProcessMemory( g_hProcess, (PVOID)Address, Buffer, Size, BytesWritten );
	VirtualProtectEx( g_hProcess, (void*)Address, Size, OldProtect, NULL );
	return ret;
}

HANDLE ReClassOpenProcess( DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessID )
{
	if (g_PluginOverrideOpenProcess != NULL)
		return g_PluginOverrideOpenProcess( dwDesiredAccess, bInheritHandle, dwProcessID );
	return OpenProcess( dwDesiredAccess, bInheritHandle, dwProcessID );
}

HANDLE ReClassOpenThread( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwThreadID )
{
	if (g_PluginOverrideOpenThread != NULL)
		return g_PluginOverrideOpenThread( dwDesiredAccessFlags, bInheritHandle, dwThreadID );
	return OpenThread( dwDesiredAccessFlags, bInheritHandle, dwThreadID );
}

CStringA ReadMemoryStringA( ULONG_PTR address, SIZE_T max )
{	
	SIZE_T bytesRead = 0;
	auto buffer = std::make_unique<char[]>( max + 1 );

	if (ReClassReadMemory( (PVOID)address, (LPVOID)buffer.get( ), max, &bytesRead ) != 0)
	{
		for (int i = 0; i < bytesRead; i++)
		{
			if (!(isprint( buffer[i] )) && buffer[i] != '\0')
				buffer[i] = '.';
		}
		buffer[bytesRead] = '\0';
		return CStringA( buffer.get( ) );
	}
	else
	{
		#ifdef _DEBUG
		PrintOut( _T( "[ReadMemoryString]: Failed to read memory, GetLastError() = %s" ), Utils::GetLastErrorString( ).GetString( ) );
		#endif
		return CStringA( ".." );
	}
}

CStringW ReadMemoryStringW( ULONG_PTR address, SIZE_T max )
{
	SIZE_T bytesRead = 0;
	auto buffer = std::make_unique<wchar_t[]>( max + 1 );
	
	if (ReClassReadMemory( (PVOID)address, (LPVOID)buffer.get( ), max * sizeof( wchar_t ), &bytesRead ) != 0)
	{
		bytesRead /= sizeof( wchar_t );

		for (int i = 0; i < bytesRead; i++)
		{
			if (!(iswprint( buffer[i] )) && buffer[i] != '\0')
				buffer[i] = '.';
		}
		buffer[bytesRead] = '\0';
		return CStringW( buffer.get( ) );
	}
	else
	{
		#ifdef _DEBUG
		PrintOut( _T( "[ReadMemoryString]: Failed to read memory, GetLastError() = %s" ), Utils::GetLastErrorString( ).GetString( ) );
		#endif
		return CStringW( L".." );
	}
}

BOOLEAN PauseResumeThreadList( BOOL bResumeThread )
{
	if (g_hProcess == NULL)
		return FALSE;

	NTSTATUS status = STATUS_SUCCESS;
	ULONG bufferSize = 0x4000;
	DWORD ProcessId = GetProcessId( g_hProcess );
	PVOID SystemProcessInfo = malloc( bufferSize );
	if (!SystemProcessInfo)
	{
		#ifdef _DEBUG
		PrintOut( _T( "[PauseResumeThreadList]: Couldn't allocate system process info buffer!" ) );
		#endif
		return FALSE;
	}

	while (TRUE)
	{
		status = ntdll::NtQuerySystemInformation( SystemProcessInformation, SystemProcessInfo, bufferSize, &bufferSize );
		if (status == STATUS_BUFFER_TOO_SMALL || status == STATUS_INFO_LENGTH_MISMATCH)
		{
			if (SystemProcessInfo)
				free( SystemProcessInfo );
			SystemProcessInfo = malloc( bufferSize * 2 );
			if (!SystemProcessInfo)
			{
				#ifdef _DEBUG
				PrintOut( _T( "[PauseResumeThreadList]: Couldn't allocate system process info buffer!" ) );
				#endif
				return FALSE;
			}
		}
		else
		{
			break;
		}
	}

	if (!NT_SUCCESS( status ))
	{
		if (SystemProcessInfo)
			free( SystemProcessInfo );
		#ifdef _DEBUG
		PrintOut( _T( "[PauseResumeThreadList]: NtQuerySystemInformation failed with status 0x%08X" ), status );
		#endif
		return FALSE;
	}

	PSYSTEM_PROCESS_INFORMATION process;
	PSYSTEM_THREAD_INFORMATION threads;
	ULONG numberOfThreads;

	process = PROCESS_INFORMATION_FIRST_PROCESS( SystemProcessInfo );
	do
	{
		if (process->UniqueProcessId == (HANDLE)ProcessId)
			break;
	} while (process = PROCESS_INFORMATION_NEXT_PROCESS( process ));

	if (!process)
	{
		// The process doesn't exist anymore :(
		return 0;
	}

	threads = process->Threads;
	numberOfThreads = process->NumberOfThreads;

	for (ULONG i = 0; i < numberOfThreads; i++)
	{
		PSYSTEM_THREAD_INFORMATION thread = &threads[i];
		if (!thread)
			continue;
		DWORD thId = (DWORD)thread->ClientId.UniqueThread;
		if (!thId)
			continue;

		HANDLE hThread = ReClassOpenThread( THREAD_SUSPEND_RESUME, FALSE, thId );

		if (bResumeThread)
			ResumeThread( hThread );
		else
			SuspendThread( hThread );

		CloseHandle( hThread );
	}

	if (SystemProcessInfo)
		free( SystemProcessInfo );

	return 1;
}

ULONG_PTR GetBase( )
{
	if (g_MemMap.size( ) > 1)
		return g_AttachedProcessAddress;
	#ifdef _WIN64
	return (ULONG_PTR)0x140000000;
	#else
	return (ULONG_PTR)0x400000;
	#endif
}

BOOLEAN IsCode( ULONG_PTR Address )
{
	for (size_t i = 0; i < g_MemMapCode.size( ); i++)
	{
		if ((Address >= g_MemMapCode[i].Start) && (Address <= g_MemMapCode[i].End))
			return TRUE;
	}
	return FALSE;
}

BOOLEAN IsData( ULONG_PTR Address )
{
	for (size_t i = 0; i < g_MemMapData.size( ); i++)
	{
		if ((Address >= g_MemMapData[i].Start) && (Address <= g_MemMapData[i].End))
			return true;
	}
	return false;
}

BOOLEAN IsMemory( ULONG_PTR Address )
{
	for (size_t i = 0; i < g_MemMap.size( ); i++)
	{
		if ((Address >= g_MemMap[i].Start) && (Address <= g_MemMap[i].End))
			return true;
	}
	return false;
}

BOOLEAN IsModule( ULONG_PTR Address )
{
	for (size_t i = 0; i < g_MemMapModules.size( ); i++)
	{
		if (Address >= g_MemMapModules[i].Start && Address <= g_MemMapModules[i].End)
			return true;
	}
	return false;
}

CString GetAddressName( ULONG_PTR Address, BOOLEAN bJustAddress )
{
	CString txt;
	size_t i = 0;

	for (i = 0; i < g_CustomNames.size( ); i++)
	{
		if (Address == g_CustomNames[i].Address)
		{
			#ifdef _WIN64
			txt.Format( _T( "%s.%IX" ), g_CustomNames[i].Name, Address );
			#else
			txt.Format( _T( "%s.%X" ), g_CustomNames[i].Name, Address );
			#endif
			return txt;
		}
	}

	for (i = 0; i < g_Exports.size( ); i++)
	{
		if (Address == g_Exports[i].Address)
		{
			#ifdef _WIN64
			txt.Format( _T( "%s.%IX" ), g_Exports[i].Name, Address );
			#else
			txt.Format( _T( "%s.%X" ), g_Exports[i].Name, Address );
			#endif
			return txt;
		}
	}

	for (i = 0; i < g_MemMapCode.size( ); i++)
	{
		if ((Address >= g_MemMapCode[i].Start) && (Address <= g_MemMapCode[i].End))
		{
			#ifdef _WIN64
			txt.Format( _T( "<CODE>%s.%IX" ), g_MemMapCode[i].Name, Address );
			#else
			txt.Format( _T( "<CODE>%s.%X" ), g_MemMapCode[i].Name, Address );
			#endif
			return txt;
		}
	}

	for (i = 0; i < g_MemMapData.size( ); i++)
	{
		if ((Address >= g_MemMapData[i].Start) && (Address <= g_MemMapData[i].End))
		{
			#ifdef _WIN64
			txt.Format( _T( "<DATA>%s.%IX" ), g_MemMapData[i].Name, Address );
			#else
			txt.Format( _T( "<DATA>%s.%X" ), g_MemMapData[i].Name, Address );
			#endif
			return txt;
		}
	}

	for (i = 0; i < g_MemMapModules.size( ); i++)
	{
		if ((Address >= g_MemMapModules[i].Start) && (Address <= g_MemMapModules[i].End))
		{
			#ifdef _WIN64
			txt.Format( _T( "%s.%IX" ), g_MemMapModules[i].Name, Address );
			#else
			txt.Format( _T( "%s.%X" ), g_MemMapModules[i].Name, Address );
			#endif
			return txt;
		}
	}

	for (i = 0; i < g_MemMap.size( ); i++)
	{
		if ((Address >= g_MemMap[i].Start) && (Address <= g_MemMap[i].End))
		{
			#ifdef _WIN64
			txt.Format( _T( "%IX" ), Address );
			#else
			txt.Format( _T( "%X" ), Address );
			#endif
			return txt;
		}
	}

	if (bJustAddress)
	{
		#ifdef _WIN64
		txt.Format( _T( "%IX" ), Address );
		#else
		txt.Format( _T( "%X" ), Address );
		#endif
	}

	return txt;
}

CString GetModuleName( ULONG_PTR Address )
{
	for (size_t i = 0; i < g_MemMapModules.size( ); i++)
	{
		if ((Address >= g_MemMapModules[i].Start) && (Address <= g_MemMapModules[i].End))
			return g_MemMapModules[i].Name;
	}
	return CString( "<unknown>" );
}

ULONG_PTR GetAddressFromName( CString moduleName )
{
	ULONG_PTR moduleAddress = 0;
	for (size_t i = 0; i < g_MemMapModules.size( ); i++)
	{
		if (g_MemMapModules[i].Name == moduleName)
		{
			moduleAddress = g_MemMapModules[i].Start;
			break;
		}
	}
	return moduleAddress;
}

BOOLEAN IsProcessHandleValid( HANDLE hProc )
{
	DWORD RetVal = WAIT_FAILED;
	if (!hProc)
		return FALSE;
	RetVal = WaitForSingleObject( hProc, 0 );
	if (RetVal == WAIT_FAILED)
		return FALSE;
	return (RetVal == WAIT_TIMEOUT) ? TRUE : FALSE;
}

BOOLEAN UpdateMemoryMap( void )
{
	g_MemMap.clear( );
	g_MemMapCode.clear( );
	g_MemMapData.clear( );
	g_MemMapModules.clear( );
	g_Exports.clear( );
	g_CustomNames.clear( );

	if (g_hProcess == NULL)
		return FALSE;

	if (!IsProcessHandleValid( g_hProcess ))
	{
		g_hProcess = NULL;
		return FALSE;
	}

	SYSTEM_INFO SysInfo;
	GetSystemInfo( &SysInfo );

	MEMORY_BASIC_INFORMATION MemInfo;
	ULONG_PTR pMemory = (ULONG_PTR)SysInfo.lpMinimumApplicationAddress;
	while (pMemory < (ULONG_PTR)SysInfo.lpMaximumApplicationAddress)
	{
		if (VirtualQueryEx( g_hProcess, (LPCVOID)pMemory, &MemInfo, sizeof( MEMORY_BASIC_INFORMATION ) ) != FALSE)
		{
			if (MemInfo.State == MEM_COMMIT /*&& MemInfo.Type == MEM_PRIVATE*/)
			{
				MemMapInfo Mem;
				Mem.Start = (ULONG_PTR)pMemory;
				Mem.End = (ULONG_PTR)pMemory + MemInfo.RegionSize - 1;
				g_MemMap.push_back( Mem );
			}
			pMemory = (ULONG_PTR)MemInfo.BaseAddress + MemInfo.RegionSize;
		}
		else
		{
			pMemory += 1024;
		}
	}

	PPROCESS_BASIC_INFORMATION ProcessInfo = NULL;
	PEB Peb;
	PEB_LDR_DATA LdrData;

	// Try to allocate buffer 
	DWORD dwSize = sizeof( PROCESS_BASIC_INFORMATION );
	ProcessInfo = (PPROCESS_BASIC_INFORMATION)malloc( dwSize );
	if (!ProcessInfo)
	{
		#ifdef _DEBUG
		PrintOut( _T( "[UpdateMemoryMap]: Couldn't allocate process info buffer!" ) );
		#endif
		return FALSE;
	}

	ULONG dwSizeNeeded = 0;
	NTSTATUS status = ntdll::NtQueryInformationProcess( g_hProcess, ProcessBasicInformation, ProcessInfo, dwSize, &dwSizeNeeded );
	if (status >= 0 && dwSize < dwSizeNeeded)
	{
		if (ProcessInfo)
			free( ProcessInfo );

		ProcessInfo = (PPROCESS_BASIC_INFORMATION)malloc( dwSizeNeeded );
		if (!ProcessInfo)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateMemoryMap]: Couldn't allocate process info buffer!" ) );
			#endif
			return FALSE;
		}

		status = ntdll::NtQueryInformationProcess( g_hProcess, ProcessBasicInformation, ProcessInfo, dwSizeNeeded, &dwSizeNeeded );
	}

	// Did we successfully get basic info on process
	if (NT_SUCCESS( status ))
	{
		// Check for PEB
		if (!ProcessInfo->PebBaseAddress)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateMemoryMap]: PEB is null! Aborting UpdateExports!" ) );
			#endif
			if (ProcessInfo)
				free( ProcessInfo );
			return false;
		}

		// Read Process Environment Block (PEB)
		SIZE_T dwBytesRead = 0;
		if (ReClassReadMemory( (LPVOID)ProcessInfo->PebBaseAddress, &Peb, sizeof( PEB ), &dwBytesRead ) == 0)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateMemoryMap]: Failed to read PEB! Aborting UpdateExports!" ) );
			#endif
			if (ProcessInfo)
				free( ProcessInfo );
			return false;
		}

		// Get Ldr
		dwBytesRead = 0;
		if (ReClassReadMemory( (LPVOID)Peb.Ldr, &LdrData, sizeof( LdrData ), &dwBytesRead ) == 0)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateMemoryMap]: Failed to read PEB Ldr Data! Aborting UpdateExports!" ) );
			#endif
			if (ProcessInfo)
				free( ProcessInfo );
			return false;
		}

		LIST_ENTRY *pLdrListHead = (LIST_ENTRY *)LdrData.InLoadOrderModuleList.Flink;
		LIST_ENTRY *pLdrCurrentNode = LdrData.InLoadOrderModuleList.Flink;
		do
		{
			LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
			dwBytesRead = 0;
			if (!ReClassReadMemory( (LPVOID)pLdrCurrentNode, &lstEntry, sizeof( LDR_DATA_TABLE_ENTRY ), &dwBytesRead ))
			{
				#ifdef _DEBUG
				PrintOut( _T( "[UpdateMemoryMap]: Could not read list entry from LDR list. Error = %s" ), Utils::GetLastErrorString( ).GetString( ) );
				#endif
				if (ProcessInfo)
					free( ProcessInfo );
				return false;
			}

			pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;

			if (lstEntry.DllBase != NULL /*&& lstEntry.SizeOfImage != 0*/)
			{
				UCHAR* ModuleBase = (UCHAR*)lstEntry.DllBase;
				DWORD ModuleSize = lstEntry.SizeOfImage;

				WCHAR wcsModulePath[MAX_PATH] = { 0 };
				WCHAR* wcsModuleName = NULL;

				if (lstEntry.FullDllName.Length > 0)
				{
					dwBytesRead = 0;
					if (ReClassReadMemory( (LPVOID)lstEntry.FullDllName.Buffer, &wcsModulePath, lstEntry.FullDllName.Length, &dwBytesRead ))
					{
						wcsModuleName = wcsrchr( wcsModulePath, L'\\' );
						if (!wcsModuleName)
							wcsModuleName = wcsrchr( wcsModulePath, L'/' );
						wcsModuleName++;

						if (g_AttachedProcessAddress == NULL)
						{
							WCHAR wcsModuleFilename[MAX_PATH] = { 0 };
							GetModuleFileNameExW( g_hProcess, NULL, wcsModuleFilename, MAX_PATH );
							if (_wcsicmp( wcsModuleFilename, wcsModulePath ) == 0)
							{
								g_AttachedProcessAddress = (ULONG_PTR)ModuleBase;
								g_AttachedProcessSize = ModuleSize;
							}
						}
					}
				}

				// module info
				MemMapInfo Mem;
				Mem.Start = (ULONG_PTR)ModuleBase;
				Mem.End = Mem.Start + ModuleSize;
				Mem.Size = ModuleSize;
				#ifdef UNICODE
				Mem.Name = wcsModuleName;
				Mem.Path = wcsModulePath;
				#else
				Mem.Name = CW2A( wcsModule );
				Mem.Path = CW2A( wcsFullDllName );
				#endif
				g_MemMapModules.push_back( Mem );

				// module code
				IMAGE_DOS_HEADER DosHdr;
				IMAGE_NT_HEADERS NtHdr;

				ReClassReadMemory( ModuleBase, &DosHdr, sizeof( IMAGE_DOS_HEADER ), NULL );
				ReClassReadMemory( ModuleBase + DosHdr.e_lfanew, &NtHdr, sizeof( IMAGE_NT_HEADERS ), NULL );
				DWORD sectionsSize = (DWORD)NtHdr.FileHeader.NumberOfSections * sizeof( IMAGE_SECTION_HEADER );
				PIMAGE_SECTION_HEADER sections = (PIMAGE_SECTION_HEADER)malloc( sectionsSize );
				ReClassReadMemory( ModuleBase + DosHdr.e_lfanew + sizeof( IMAGE_NT_HEADERS ), sections, sectionsSize, NULL );
				for (int i = 0; i < NtHdr.FileHeader.NumberOfSections; i++)
				{
					CString txt;
					MemMapInfo Mem;
					txt.Format( _T( "%.8s" ), sections[i].Name ); txt.MakeLower( );
					if (txt == ".text" || txt == "code")
					{
						Mem.Start = (ULONG_PTR)ModuleBase + sections[i].VirtualAddress;
						Mem.End = Mem.Start + sections[i].Misc.VirtualSize;
						Mem.Name = wcsModuleName;
						g_MemMapCode.push_back( Mem );
					}
					else if (txt == ".data" || txt == "data" || txt == ".rdata" || txt == ".idata")
					{
						Mem.Start = (ULONG_PTR)ModuleBase + sections[i].VirtualAddress;
						Mem.End = Mem.Start + sections[i].Misc.VirtualSize;
						Mem.Name = wcsModuleName;
						g_MemMapData.push_back( Mem );
					}
				}
				// Free sections
				free( sections );
			}

		} while (pLdrListHead != pLdrCurrentNode);
	}
	else
	{
		#ifdef _DEBUG
		PrintOut( _T( "[UpdateExports]: NtQueryInformationProcess failed! Aborting..." ) );
		#endif
		if (ProcessInfo)
			free( ProcessInfo );
		return 0;
	}

	if (ProcessInfo)
		free( ProcessInfo );

	for (UINT i = 0; i < g_MemMap.size( ); i++)
	{
		if (IsModule( g_MemMap[i].Start ))
			g_MemMap[i].Name = GetModuleName( g_MemMap[i].Start );
	}

	return true;
}

BOOLEAN UpdateExports( )
{
	g_Exports.clear( );

	//if (!g_bExports) 
	//	return;

	PPROCESS_BASIC_INFORMATION ProcessInfo = NULL;
	PEB Peb;
	PEB_LDR_DATA LdrData;

	// Try to allocate buffer 
	DWORD dwSize = sizeof( PROCESS_BASIC_INFORMATION );
	ProcessInfo = (PPROCESS_BASIC_INFORMATION)malloc( dwSize );
	if (!ProcessInfo)
	{
		#ifdef _DEBUG
		PrintOut( _T( "[UpdateExports]: Couldn't allocate heap buffer!" ) );
		#endif
		return FALSE;
	}

	ULONG dwSizeNeeded = 0;
	NTSTATUS status = ntdll::NtQueryInformationProcess( g_hProcess, ProcessBasicInformation, ProcessInfo, dwSize, &dwSizeNeeded );
	if (status >= 0 && dwSize < dwSizeNeeded)
	{
		if (ProcessInfo)
			free( ProcessInfo );

		ProcessInfo = (PPROCESS_BASIC_INFORMATION)malloc( dwSizeNeeded );
		if (!ProcessInfo)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateExports]: Couldn't allocate heap buffer!" ) );
			#endif
			return FALSE;
		}

		status = ntdll::NtQueryInformationProcess( g_hProcess, ProcessBasicInformation, ProcessInfo, dwSizeNeeded, &dwSizeNeeded );
	}

	// Did we successfully get basic info on process
	if (NT_SUCCESS( status ))
	{
		// Check for PEB
		if (!ProcessInfo->PebBaseAddress)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateExports]: PEB is null! Aborting..." ) );
			#endif
			if (ProcessInfo)
				free( ProcessInfo );
			return FALSE;
		}

		// Read Process Environment Block (PEB)
		SIZE_T dwBytesRead = 0;
		if (ReClassReadMemory( (LPVOID)ProcessInfo->PebBaseAddress, &Peb, sizeof( PEB ), &dwBytesRead ) == 0)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateExports]: Failed to read PEB! Aborting UpdateExports." ) );
			#endif
			if (ProcessInfo)
				free( ProcessInfo );
			return FALSE;
		}

		// Get Ldr
		dwBytesRead = 0;
		if (ReClassReadMemory( (LPVOID)Peb.Ldr, &LdrData, sizeof( LdrData ), &dwBytesRead ) == 0)
		{
			#ifdef _DEBUG
			PrintOut( _T( "[UpdateExports]: Failed to read PEB Ldr Data! Aborting UpdateExports." ) );
			#endif
			if (ProcessInfo)
				free( ProcessInfo );
			return 0;
		}

		LIST_ENTRY *pLdrListHead = (LIST_ENTRY *)LdrData.InLoadOrderModuleList.Flink;
		LIST_ENTRY *pLdrCurrentNode = LdrData.InLoadOrderModuleList.Flink;
		do
		{
			LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
			dwBytesRead = 0;
			if (!ReClassReadMemory( (void*)pLdrCurrentNode, &lstEntry, sizeof( LDR_DATA_TABLE_ENTRY ), &dwBytesRead ))
			{
				#ifdef _DEBUG
				PrintOut( _T( "[UpdateExports]: Could not read list entry from LDR list. Error = %s" ), Utils::GetLastErrorString( ).GetString( ) );
				#endif
				if (ProcessInfo)
					free( ProcessInfo );
				return 0;
			}

			pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;

			if (lstEntry.DllBase != nullptr && lstEntry.SizeOfImage != 0)
			{
				unsigned char* ModuleHandle = (unsigned char*)lstEntry.DllBase;
				wchar_t wcsDllName[MAX_PATH] = { 0 };
				wchar_t ModuleName[MAX_PATH] = { 0 };
				if (lstEntry.BaseDllName.Length > 0)
				{
					dwBytesRead = 0;
					if (ReClassReadMemory( (LPVOID)lstEntry.BaseDllName.Buffer, &wcsDllName, lstEntry.BaseDllName.Length, &dwBytesRead ))
					{
						wcscpy_s( ModuleName, wcsDllName );
					}
				}

				IMAGE_DOS_HEADER DosHdr;
				IMAGE_NT_HEADERS NtHdr;

				ReClassReadMemory( ModuleHandle, &DosHdr, sizeof( DosHdr ), NULL );
				ReClassReadMemory( ModuleHandle + DosHdr.e_lfanew, &NtHdr, sizeof( IMAGE_NT_HEADERS ), NULL );
				if (NtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != 0)
				{
					IMAGE_EXPORT_DIRECTORY ExpDir;
					ReClassReadMemory( (LPVOID)(ModuleHandle + NtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress), &ExpDir, sizeof( ExpDir ), NULL );
					PVOID pName = (PVOID)(ModuleHandle + ExpDir.AddressOfNames);
					PVOID pOrd = (PVOID)(ModuleHandle + ExpDir.AddressOfNameOrdinals);
					PVOID pAddress = (PVOID)(ModuleHandle + ExpDir.AddressOfFunctions);

					ULONG aNames[MAX_EXPORTS];
					WORD aOrds[MAX_EXPORTS];
					ULONG aAddresses[MAX_EXPORTS];
					ReClassReadMemory( (LPVOID)pName, aNames, ExpDir.NumberOfNames * sizeof( aNames[0] ), NULL );
					ReClassReadMemory( (LPVOID)pOrd, aOrds, ExpDir.NumberOfNames * sizeof( aOrds[0] ), NULL );
					ReClassReadMemory( (LPVOID)pAddress, aAddresses, ExpDir.NumberOfFunctions * sizeof( aAddresses[0] ), NULL );

					for (DWORD i = 0; i < ExpDir.NumberOfNames; i++)
					{
						char ExportName[256];
						ReClassReadMemory( (LPVOID)(ModuleHandle + aNames[i]), ExportName, 256, NULL );
						DWORD_PTR Address = (DWORD_PTR)ModuleHandle + aAddresses[aOrds[i]];

						AddressName Entry;
						Entry.Name.Format( _T( "%ls.%hs" ), ModuleName, ExportName );
						Entry.Address = Address;
						// Add export entry to array
						g_Exports.push_back( Entry );
					}

				}
			}
		} while (pLdrListHead != pLdrCurrentNode);
	}
	else
	{
		#ifdef _DEBUG
		PrintOut( _T( "[UpdateExports]: NtQueryInformationProcess failed! Aborting..." ) );
		#endif
		if (ProcessInfo)
			free( ProcessInfo );
		return 0;
	}

	if (ProcessInfo)
		free( ProcessInfo );

	return 1;
}

int SplitString( const CString& input, const CString& delimiter, CStringArray& results )
{
	int iPos = 0;
	int newPos = -1;
	int sizeS2 = delimiter.GetLength( );
	int isize = input.GetLength( );

	CArray<INT, int> positions;

	newPos = input.Find( delimiter, 0 );
	if (newPos < 0)
		return 0;

	int numFound = 0;
	while (newPos > iPos)
	{
		numFound++;
		positions.Add( newPos );
		iPos = newPos;
		newPos = input.Find( delimiter, iPos + sizeS2 + 1 );
	}

	for (int i = 0; i <= positions.GetSize( ); i++)
	{
		CString s;
		if (i == 0)
		{
			s = input.Mid( i, positions[i] );
		}
		else
		{
			int offset = positions[i - 1] + sizeS2;
			if (offset < isize)
			{
				if (i == positions.GetSize( ))
					s = input.Mid( offset );
				else if (i > 0)
					s = input.Mid( positions[i - 1] + sizeS2, positions[i] - positions[i - 1] - sizeS2 );
			}
		}

		if (s.GetLength( ) > 0)
		{
			results.Add( s );
		}
	}
	return numFound;
}

ULONG_PTR ConvertStrToAddress( CString str )
{
	CStringArray chunks;
	if (SplitString( str, "+", chunks ) == 0)
		chunks.Add( str );

	ULONG_PTR Final = 0;

	for (UINT i = 0; i < (UINT)chunks.GetCount( ); i++)
	{
		CString a = chunks[i];

		a.MakeLower( );  // Make all lowercase
		a.Trim( );		// Trim whitespace
		a.Remove( _T( '\"' ) ); // Remove quotes

		bool bPointer = false;
		bool bMod = false;

		if (a.Find( _T( ".exe" ) ) != -1 || a.Find( _T( ".dll" ) ) != -1)
		{
			bMod = true;
		}

		if (a[0] == _T( '*' ))
		{
			bPointer = true;
			a = a.Mid( 1 );
		}
		else if (a[0] == _T( '&' ))
		{
			bMod = true;
			a = a.Mid( 1 );
		}

		ULONG_PTR curadd = 0;

		if (bMod)
		{
			for (UINT i = 0; i < g_MemMapModules.size( ); i++)
			{
				CString ModName = g_MemMapModules[i].Name;
				ModName.MakeLower( );
				if (StrStr( ModName, a ) != NULL)
				{
					curadd = g_MemMapModules[i].Start;
					bMod = true;
					break;
				}
			}
		}
		else
		{
			curadd = (ULONG_PTR)_tcstoui64( a.GetBuffer( ), NULL, 16 ); //StrToNum
		}

		Final += curadd;

		if (bPointer)
		{
			if (!ReClassReadMemory( (LPVOID)Final, &Final, sizeof( Final ), NULL ))
			{
				// Causing memory leaks when Final doesnt point to a valid address.
				//#ifdef _DEBUG
				// PrintOut(_T("[ConvertStrToAddress]: Failed to read memory. Error: %s"), Utils::GetLastErrorString().c_str());
				//#endif
				return 0xDEADBEEF;
			}
		}
	}

	return Final;
}
