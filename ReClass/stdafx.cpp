#include "stdafx.h"
//#include "CMainFrame.h"

//Globals
HANDLE g_hProcess = NULL;
DWORD g_ProcessID = NULL;
size_t g_AttachedProcessAddress = NULL;
DWORD g_AttachedProcessSize = NULL;
CString g_ProcessName;
Symbols* g_SymLoader = nullptr;

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

CFont g_ViewFont;

int g_FontWidth;
int g_FontHeight;

bool g_bAddress = true;
bool g_bOffset = true;
bool g_bText = true;
bool g_bRTTI = true;
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

#pragma region Plugins
tMemoryOperation g_PluginOverrideMemoryWrite = nullptr;
tMemoryOperation g_PluginOverrideMemoryRead = nullptr;
tHandleOperation g_PluginOverrideHandleProcess = nullptr;
tHandleOperation g_PluginOverrideHandleThread = nullptr;

std::vector<RECLASS_PLUGINS> g_LoadedPlugins;

void LoadPlugins( )
{
	WIN32_FIND_DATA file_data;
	ZeroMemory( &file_data, sizeof( WIN32_FIND_DATA ) );

	#ifdef _WIN64
	HANDLE findfile_tree = FindFirstFile( _T( "plugins\\*.rc-plugin64" ), &file_data );
	#else
	HANDLE findfile_tree = FindFirstFile( _T( "plugins\\*.rc-plugin" ), &file_data );
	#endif

	if (findfile_tree != INVALID_HANDLE_VALUE)
	{
		CString message;

		do
		{
			HMODULE plugin_base = LoadLibrary( CString( _T( "plugins\\" ) ) + file_data.cFileName );
			if (plugin_base == NULL)
			{
				message.Format( _T( "plugin %s was not able to be loaded!" ), file_data.cFileName );
				PrintOut( message );
				continue;
			}

			auto pfnPluginInit = reinterpret_cast<tPluginInit>(GetProcAddress( plugin_base, "PluginInit" ));
			if (pfnPluginInit == nullptr)
			{
				message.Format( _T( "%s is not a reclass plugin!" ), file_data.cFileName );
				PrintOut( message );
				FreeLibrary( plugin_base );
				continue;
			}

			auto pfnPluginStateChange = reinterpret_cast<tPluginStateChange>(GetProcAddress( plugin_base, "PluginStateChange" ));
			if (pfnPluginStateChange == nullptr)
			{
				message.Format( _T( "%s doesnt have exported state change function! Unable to disable plugin on request, stop reclass and delete the plugin to disable it" ), file_data.cFileName );
				PrintOut( message );
			}

			auto pfnPluginSettingDlgProc = reinterpret_cast<DLGPROC>(GetProcAddress( plugin_base, "PluginSettingsDlg" ));

			RECLASS_PLUGINS plugin;
			ZeroMemory( &plugin, sizeof RECLASS_PLUGINS );
			wcscpy_s( plugin.FileName, file_data.cFileName );
			plugin.LoadedBase = plugin_base;
			plugin.InitFnc = pfnPluginInit;
			plugin.SettingDlgFnc = pfnPluginSettingDlgProc;
			plugin.StateChangeFnc = pfnPluginStateChange;

			if (pfnPluginInit( &plugin.Info ))
			{
				#ifdef UNICODE
				plugin.State = g_ReClassApp.GetProfileInt( L"PluginState", plugin.Info.Name, 1 ) == 1;
				#else
				plugin.State = g_ReClassApp.GetProfileInt( "PluginState", CW2A( plugin.Info.Name ), 1 ) == 1;
				#endif
				if (plugin.Info.DialogID == -1)
					plugin.SettingDlgFnc = nullptr;
				PrintOut( _T( "Loaded plugin %s (%ls version %ls) - %ls" ), file_data.cFileName, plugin.Info.Name, plugin.Info.Version, plugin.Info.About );
				if (plugin.StateChangeFnc != nullptr)
					plugin.StateChangeFnc( plugin.State );
				g_LoadedPlugins.push_back( plugin );
			}
			else
			{
				message.Format( _T( "Failed to load plugin %s" ), file_data.cFileName );
				PrintOut( message );
				FreeLibrary( plugin_base );
			}
		} while (FindNextFile( findfile_tree, &file_data ));
	}
}

BOOL PLUGIN_CC ReClassOverrideMemoryOperations( tMemoryOperation MemWrite, tMemoryOperation MemRead, BOOL bForceSet )
{
	if (MemWrite == nullptr || MemRead == nullptr)
		return FALSE;

	if (g_PluginOverrideMemoryRead != nullptr && g_PluginOverrideMemoryWrite != nullptr && !bForceSet)
	{
		return FALSE;
	}
	else
	{
		g_PluginOverrideMemoryWrite = MemWrite;
		g_PluginOverrideMemoryRead = MemRead;
		return TRUE;
	}
}

BOOL PLUGIN_CC ReClassOverrideHandleOperations( tHandleOperation HandleProcess, tHandleOperation HandleThread, BOOL bForceSet )
{
	if (HandleProcess == nullptr || HandleThread == nullptr)
		return FALSE;
	if (g_PluginOverrideHandleProcess != nullptr && g_PluginOverrideHandleThread != nullptr && !bForceSet)
	{
		return FALSE;
	}
	else
	{
		g_PluginOverrideHandleProcess = HandleProcess;
		g_PluginOverrideHandleThread = HandleThread;
		return TRUE;
	}
	return FALSE;
}

void PLUGIN_CC ReClassPrintConsole( const wchar_t *format, ... )
{
	wchar_t buffer[6048];
	ZeroMemory( &buffer, sizeof( buffer ) );

	va_list va;
	va_start( va, format );
	vswprintf_s( buffer, format, va );
	va_end( va );

	#ifndef UNICODE
	g_ReClassApp.Console->PrintText( CW2A( buffer ) );
	#else
	g_ReClassApp.Console->PrintText( buffer );
	#endif
}

LPHANDLE PLUGIN_CC ReClassGetProcessHandle( )
{
	return &g_hProcess;
}

HWND PLUGIN_CC ReClassMainWindow( )
{
	return *g_ReClassApp.GetMainWnd( );
}

CMFCRibbonBar* PLUGIN_CC ReClassRibbonInterface( )
{
	return g_ReClassApp.GetRibbonBar( );
}
#pragma endregion

BOOL ReClassReadMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_read )
{
	if (g_PluginOverrideMemoryRead != nullptr)
		return g_PluginOverrideMemoryRead( Address, Buffer, Size, num_read );

	BOOL return_val = ReadProcessMemory( g_hProcess, (LPVOID)Address, Buffer, Size, num_read );
	if (!return_val) ZeroMemory( Buffer, Size );
	return return_val;
}

BOOL ReClassWriteMemory( LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_wrote )
{
	if (g_PluginOverrideMemoryWrite != nullptr)
		return g_PluginOverrideMemoryWrite( Address, Buffer, Size, num_wrote );

	DWORD OldProtect;
	VirtualProtectEx( g_hProcess, (void*)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect );
	BOOL ret = WriteProcessMemory( g_hProcess, (void*)Address, Buffer, Size, num_wrote );
	VirtualProtectEx( g_hProcess, (void*)Address, Size, OldProtect, NULL );
	return ret;
}

HANDLE ReClassOpenProcess( DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessID )
{
	if (g_PluginOverrideHandleProcess != nullptr)
		return g_PluginOverrideHandleProcess( dwDesiredAccess, bInheritHandle, dwProcessID );
	return OpenProcess( dwDesiredAccess, bInheritHandle, dwProcessID );
}

HANDLE ReClassOpenThread( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwThreadID )
{
	if (g_PluginOverrideHandleThread != nullptr)
		return g_PluginOverrideHandleThread( dwDesiredAccessFlags, bInheritHandle, dwThreadID );
	return OpenThread( dwDesiredAccessFlags, bInheritHandle, dwThreadID );
}

CStringA ReadMemoryStringA( size_t address, SIZE_T max )
{
	auto buffer = std::make_unique<char[]>( max + 1 );
	SIZE_T bytesRead;

	if (ReClassReadMemory( (PVOID)address, buffer.get( ), max, &bytesRead ) != 0)
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

CStringW ReadMemoryStringW( size_t address, SIZE_T max )
{
	auto buffer = std::make_unique<wchar_t[]>( max + 1 );
	SIZE_T bytesRead;
	if (ReClassReadMemory( (PVOID)address, buffer.get( ), max * sizeof( wchar_t ), &bytesRead ) != 0)
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

size_t GetBase( )
{
	if (g_MemMap.size( ) > 1)
		return g_AttachedProcessAddress;
	#ifdef _WIN64
	return 0x140000000;
	#else
	return 0x400000;
	#endif
}

bool IsCode( size_t Address )
{
	for (UINT i = 0; i < g_MemMapCode.size( ); i++)
	{
		if ((Address >= g_MemMapCode[i].Start) && (Address <= g_MemMapCode[i].End))
			return true;
	}
	return false;
}

bool IsData( size_t Address )
{
	for (UINT i = 0; i < g_MemMapData.size( ); i++)
	{
		if ((Address >= g_MemMapData[i].Start) && (Address <= g_MemMapData[i].End))
			return true;
	}
	return false;
}

bool IsMemory( size_t Address )
{
	for (UINT i = 0; i < g_MemMap.size( ); i++)
	{
		if ((Address >= g_MemMap[i].Start) && (Address <= g_MemMap[i].End))
			return true;
	}
	return false;
}

bool IsModule( size_t Address )
{
	for (UINT i = 0; i < g_MemMapModules.size( ); i++)
	{
		if (Address >= g_MemMapModules[i].Start && Address <= g_MemMapModules[i].End)
			return true;
	}
	return false;
}

CString GetAddressName( size_t Address, bool bHEX )
{
	CString txt;

	for (UINT i = 0; i < g_CustomNames.size( ); i++)
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

	for (UINT i = 0; i < g_Exports.size( ); i++)
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

	for (UINT i = 0; i < g_MemMapCode.size( ); i++)
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

	for (UINT i = 0; i < g_MemMapData.size( ); i++)
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

	for (UINT i = 0; i < g_MemMapModules.size( ); i++)
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

	for (UINT i = 0; i < g_MemMap.size( ); i++)
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

	if (bHEX)
	{
		#ifdef _WIN64
		txt.Format( _T( "%IX" ), Address );
		#else
		txt.Format( _T( "%X" ), Address );
		#endif
	}


	return txt;
}

CString GetModuleName( size_t Address )
{
	for (size_t i = 0; i < g_MemMapModules.size( ); i++)
	{
		if ((Address >= g_MemMapModules[i].Start) && (Address <= g_MemMapModules[i].End))
			return g_MemMapModules[i].Name;
	}
	return CString( );
}

size_t GetAddressFromName( CString moduleName )
{
	size_t moduleAddress = 0;
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
	if (!hProc)
		return FALSE;
	const DWORD RetVal = WaitForSingleObject( hProc, 0 );
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
	size_t pMemory = (size_t)SysInfo.lpMinimumApplicationAddress;
	while (pMemory < (size_t)SysInfo.lpMaximumApplicationAddress)
	{
		if (VirtualQueryEx( g_hProcess, (LPCVOID)pMemory, &MemInfo, sizeof( MEMORY_BASIC_INFORMATION ) ) != FALSE)
		{
			if (MemInfo.State == MEM_COMMIT /*&& MemInfo.Type == MEM_PRIVATE*/)
			{
				MemMapInfo Mem;
				Mem.Start = (size_t)pMemory;
				Mem.End = (size_t)pMemory + MemInfo.RegionSize - 1;
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
				unsigned char* ModuleBase = (unsigned char*)lstEntry.DllBase;
				DWORD ModuleSize = lstEntry.SizeOfImage;

				wchar_t wcsModulePath[MAX_PATH] = { 0 };
				wchar_t* wcsModuleName = NULL;

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
							wchar_t wcsModuleFilename[MAX_PATH] = { 0 };
							GetModuleFileNameExW( g_hProcess, NULL, wcsModuleFilename, MAX_PATH );
							if (_wcsicmp( wcsModuleFilename, wcsModulePath ) == 0)
							{
								g_AttachedProcessAddress = (size_t)ModuleBase;
								g_AttachedProcessSize = ModuleSize;
							}
						}
					}
				}

				// module info
				MemMapInfo Mem;
				Mem.Start = (size_t)ModuleBase;
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
						Mem.Start = (size_t)ModuleBase + sections[i].VirtualAddress;
						Mem.End = Mem.Start + sections[i].Misc.VirtualSize;
						Mem.Name = wcsModuleName;
						g_MemMapCode.push_back( Mem );
					}
					else if (txt == ".data" || txt == "data" || txt == ".rdata" || txt == ".idata")
					{
						Mem.Start = (size_t)ModuleBase + sections[i].VirtualAddress;
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
		if (ReClassReadMemory( ProcessInfo->PebBaseAddress, &Peb, sizeof( PEB ), &dwBytesRead ) == 0)
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

size_t ConvertStrToAddress( CString str )
{
	CStringArray chunks;
	if (SplitString( str, "+", chunks ) == 0)
		chunks.Add( str );

	size_t Final = 0;

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

		size_t curadd = 0;

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
			curadd = (size_t)_tcstoui64( a.GetBuffer( ), NULL, 16 ); //StrToNum
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
