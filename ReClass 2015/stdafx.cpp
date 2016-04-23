#include "stdafx.h"
#include "ReClass2015.h"

//Globals
HANDLE g_hProcess = NULL;
DWORD g_ProcessID = NULL;
size_t g_AttachedProcessAddress = NULL;
DWORD g_AttachedProcessSize = NULL;

std::vector<MemMapInfo> MemMap;
std::vector<MemMapInfo> MemMapCode;
std::vector<MemMapInfo> MemMapData;
std::vector<MemMapInfo> MemMapModule;
std::vector<AddressName> Exports;
std::vector<AddressName> CustomNames;

DWORD NodeCreateIndex = 0;

COLORREF crBackground = RGB(255, 255, 255);
COLORREF crSelect = RGB(240, 240, 240);
COLORREF crHidden = RGB(240, 240, 240);

COLORREF crOffset = RGB(255, 0, 0);
COLORREF crAddress = RGB(0, 200, 0);
COLORREF crType = RGB(0, 0, 255);
COLORREF crName = RGB(32, 32, 128);
COLORREF crIndex = RGB(32, 200, 200);
COLORREF crValue = RGB(255, 128, 0);
COLORREF crComment = RGB(0, 200, 0);

COLORREF crVTable = RGB(0, 255, 0);
COLORREF crFunction = RGB(255, 0, 255);
COLORREF crChar = RGB(0, 0, 255);
COLORREF crBits = RGB(0, 0, 255);
COLORREF crCustom = RGB(64, 128, 64);
COLORREF crHex = RGB(0, 0, 0);

CFont Font;
//CFont SmallFont;

int FontWidth;
int FontHeight;

bool gbAddress = true;
bool gbOffset = true;
bool gbText = true;
bool gbRTTI = true;

bool gbFloat = true;
bool gbInt = true;
bool gbString = true;
bool gbPointers = true;

bool gbTop = true;
bool gbClassBrowser = true;
bool gbFilterProcesses = false;

CString tdHex;
CString tdInt64;
CString tdInt32;
CString tdInt16;
CString tdInt8;
CString tdDWORD;
CString tdWORD;
CString tdBYTE;
CString tdFloat;
CString tdDouble;
CString tdVec2;
CString tdVec3;
CString tdQuat;
CString tdMatrix;
CString tdPChar;
CString tdPWChar;

std::vector<HICON> Icons;

#pragma region Plugins
std::map<HMODULE, RECLASS_PLUGIN_INFO> LoadedPlugins;

MEMORY_OPERATION g_PluginOverrideMemoryWrite = nullptr;
MEMORY_OPERATION g_PluginOverrideMemoryRead = nullptr;
HANDLE_OPERATION g_PluginOverrideHandleProcess = nullptr;
HANDLE_OPERATION g_PluginOverrideHandleThread = nullptr;

BOOL PLUGIN_CC ReClassOverrideMemoryOperations( MEMORY_OPERATION MemWrite, MEMORY_OPERATION MemRead, BOOL bForceSet )
{ 
	if ( MemWrite == nullptr || MemRead == nullptr )
		return FALSE;
	if ( g_PluginOverrideMemoryRead != nullptr && g_PluginOverrideMemoryWrite != nullptr && !bForceSet )
		return FALSE;
	else {
		g_PluginOverrideMemoryWrite = MemWrite;
		g_PluginOverrideMemoryRead = MemRead;
		return TRUE;
	}
}

BOOL PLUGIN_CC ReClassOverrideHandleOperations( HANDLE_OPERATION HandleProcess, HANDLE_OPERATION HandleThread, BOOL bForceSet )
{
	if ( HandleProcess == nullptr || HandleThread == nullptr )
		return FALSE;
	if ( g_PluginOverrideHandleProcess != nullptr && g_PluginOverrideHandleThread != nullptr && !bForceSet )
		return FALSE;
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
	wchar_t buffer[ 6048 ];
	ZeroMemory( &buffer, sizeof( buffer ) );
	
	va_list va;
	va_start( va, format );
	vswprintf_s( buffer, format, va );
	va_end( va );

#ifndef UNICODE
	theApp.Console->PrintText( CW2A( buffer ) );
#else
	theApp.Console->PrintText( buffer );
#endif
}

LPHANDLE PLUGIN_CC ReClassGetProcessHandle( )
{
	return &g_hProcess;
}

HWND PLUGIN_CC ReClassMainWindow( )
{ 
	return *theApp.GetMainWnd( );
}
#pragma endregion

BOOL ReadMemory(LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_read)
{
	if ( g_PluginOverrideMemoryRead != nullptr )
		return g_PluginOverrideMemoryRead( Address, Buffer, Size, num_read );

	BOOL return_val = ReadProcessMemory( g_hProcess, (LPVOID) Address, Buffer, Size, num_read );
	if ( !return_val ) ZeroMemory( Buffer, Size );
	return return_val;
}

BOOL WriteMemory(LPVOID Address, LPVOID Buffer, SIZE_T Size, SIZE_T *num_wrote )
{
	if ( g_PluginOverrideMemoryWrite != nullptr )
		return g_PluginOverrideMemoryWrite( Address, Buffer, Size, num_wrote );

	DWORD OldProtect;
	VirtualProtectEx(g_hProcess, (void*)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
	BOOL ret = WriteProcessMemory( g_hProcess, (void*)Address, Buffer, Size, num_wrote );
	VirtualProtectEx(g_hProcess, (void*)Address, Size, OldProtect, NULL);
	return ret;
}

HANDLE ReClassOpenProcess( DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessID )
{
	if ( g_PluginOverrideHandleProcess != nullptr )
		return g_PluginOverrideHandleProcess( dwDesiredAccess, bInheritHandle, dwProcessID );
	return OpenProcess( dwDesiredAccess, bInheritHandle, dwProcessID );
}

HANDLE ReClassOpenThread( DWORD dwDesiredAccessFlags, BOOL bInheritHandle, DWORD dwThreadID )
{ 
	if ( g_PluginOverrideHandleThread != nullptr )
		return g_PluginOverrideHandleThread( dwDesiredAccessFlags, bInheritHandle, dwThreadID );
	return OpenThread( dwDesiredAccessFlags, bInheritHandle, dwThreadID );
}

CStringA ReadMemoryStringA(size_t address, SIZE_T max)
{
	auto buffer = std::make_unique<char[]>( max + 1 );
	SIZE_T bytesRead;
	
	if ( ReadMemory( (PVOID) address, buffer.get( ), max, &bytesRead ) != 0 )
	{
		for (int i = 0; i < bytesRead; i++)
		{
			if ( !( isprint( buffer[ i ] ) ) && buffer[ i ] != '\0' )
				buffer[i] = '.';
		}

		buffer[bytesRead] = '\0';

		return CStringA(buffer.get());
	} else {
#ifdef _DEBUG
		PrintOut( _T( "[ReadMemoryString]: Failed to read memory, GetLastError() = %s" ), Utils::GetLastErrorString( ).GetString( ) );
#endif
		return CStringA( ".." );
	}
}

CStringW ReadMemoryStringW( size_t address, SIZE_T max )
{
	auto buffer = std::make_unique<wchar_t[ ]>( max + 1 );
	SIZE_T bytesRead;

	if ( ReadMemory( (PVOID) address, buffer.get( ), max * sizeof( wchar_t ), &bytesRead ) != 0 )
	{
		bytesRead /= sizeof( wchar_t );
		
		for ( int i = 0; i < bytesRead; i++ )
		{
			if ( !( iswprint( buffer[ i ] ) ) && buffer[ i ] != '\0' )
				buffer[ i ] = '.';
		}
		
		buffer[ bytesRead ] = '\0';

		return CStringW( buffer.get( ) );
	} else {
#ifdef _DEBUG
		PrintOut( _T( "[ReadMemoryString]: Failed to read memory, GetLastError() = %s" ), Utils::GetLastErrorString( ).GetString( ) );
#endif
		return CStringW( L".." );
	}
}

bool PauseResumeThreadList(bool bResumeThread)
{
	if (g_hProcess == NULL)
		return 0;
	
	NTSTATUS status;
	PVOID SystemProcessInfo;
	ULONG bufferSize = 0x4000;
	HANDLE hHeap = GetProcessHeap();
	DWORD ProcessId = GetProcessId(g_hProcess);

	static HMODULE hNtDll = (HMODULE)Utils::GetLocalModuleHandle("ntdll.dll");
	static tNtQuerySystemInformation fnNTQSI = (tNtQuerySystemInformation)Utils::GetProcAddress(hNtDll, "NtQuerySystemInformation");

	SystemProcessInfo = HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, bufferSize);

	while (TRUE)
	{
		status = fnNTQSI(SystemProcessInformation, SystemProcessInfo, bufferSize, &bufferSize);
		if (status == STATUS_BUFFER_TOO_SMALL || status == STATUS_INFO_LENGTH_MISMATCH)
		{
			if (SystemProcessInfo)
				HeapFree(hHeap, 0, SystemProcessInfo);
			SystemProcessInfo = HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, bufferSize*2);
		}
		else
			break;
	}

	if (!NT_SUCCESS(status))
	{
		if (SystemProcessInfo)
			HeapFree(hHeap, 0, SystemProcessInfo);
		return 0;
	}

	PSYSTEM_PROCESS_INFORMATION process;
	PSYSTEM_THREAD_INFORMATION threads;
	ULONG numberOfThreads;

	process = PROCESS_INFORMATION_FIRST_PROCESS(SystemProcessInfo);
	do {
		if (process->UniqueProcessId == (HANDLE)ProcessId)
			break;
	} while (process = PROCESS_INFORMATION_NEXT_PROCESS(process));

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
		if (!thread) continue;
		DWORD thId = (DWORD)thread->ClientId.UniqueThread;
		if (!thId) continue;
		
		HANDLE hThread = ReClassOpenThread(THREAD_SUSPEND_RESUME, FALSE, thId);
		
		if (bResumeThread) {
			ResumeThread(hThread);
		} else {
			SuspendThread(hThread);
		}
		CloseHandle(hThread);
	}

	if (SystemProcessInfo)
		HeapFree(hHeap, 0, SystemProcessInfo);

	return 1;
}

size_t GetBase()
{
	if (MemMap.size() > 1)
		return g_AttachedProcessAddress;
	#ifdef _WIN64
	return 0x140000000;
	#else
	return 0x400000;
	#endif
}

bool IsCode(size_t Address)
{
	for (UINT i = 0; i < MemMapCode.size(); i++) {
		if (Address >= MemMapCode[i].Start && Address <= MemMapCode[i].End)
			return true;
	}
	return false;
}

bool IsData(size_t Address)
{
	for (UINT i = 0; i < MemMapData.size(); i++) {
		if (Address >= MemMapData[i].Start && Address <= MemMapData[i].End)
			return true;
	}
	return false;
}

bool IsMemory(size_t Address)
{
	for (UINT i = 0; i < MemMap.size(); i++)
	{
		if (Address >= MemMap[i].Start && Address <= MemMap[i].End)
			return true;
	}
	return false;
}

bool IsModule(size_t Address)
{
	for (UINT i = 0; i < MemMapModule.size(); i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
			return true;
	}
	return false;
}

CString GetAddressName(size_t Address, bool bHEX)
{
	CString txt;
	for (UINT i = 0; i < CustomNames.size(); i++)
	{
		if (Address == CustomNames[i].Address)
		{
			#ifdef _WIN64
			txt.Format(_T("%s.%I64X"), CustomNames[i].Name, Address);
			#else
			txt.Format(_T("%s.%X"), CustomNames[i].Name, Address);
			#endif
			return txt;
		}
	}
	for (UINT i = 0; i < Exports.size(); i++)
	{
		if (Address == Exports[i].Address)
		{
			#ifdef _WIN64
			txt.Format(_T("%s.%I64X"), Exports[i].Name, Address);
			#else
			txt.Format(_T("%s.%X"), Exports[i].Name, Address);
			#endif
			return txt;
		}
	}
	for (UINT i = 0; i < MemMapCode.size(); i++)
	{
		if (Address >= MemMapCode[i].Start && Address <= MemMapCode[i].End)
		{
			#ifdef _WIN64
			txt.Format(_T("<CODE>%s.%I64X"), MemMapCode[i].Name, Address);
			#else
			txt.Format(_T("<CODE>%s.%X"), MemMapCode[i].Name, Address);
			#endif
			return txt;
		}
	}
	for (UINT i = 0; i < MemMapData.size(); i++)
	{
		if (Address >= MemMapData[i].Start && Address <= MemMapData[i].End)
		{
			#ifdef _WIN64
			txt.Format(_T("<DATA>%s.%I64X"), MemMapData[i].Name, Address);
			#else
			txt.Format(_T("<DATA>%s.%X"), MemMapData[i].Name, Address);
			#endif
			return txt;
		}
	}
	for (UINT i = 0; i < MemMapModule.size(); i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
		{
			#ifdef _WIN64
			txt.Format(_T("%s.%I64X"), MemMapModule[i].Name, Address);
			#else
			txt.Format(_T("%s.%X"), MemMapModule[i].Name, Address);
			#endif
			return txt;
		}
	}
	for (UINT i = 0; i < MemMap.size(); i++)
	{
		if (Address >= MemMap[i].Start && Address <= MemMap[i].End)
		{
			#ifdef _WIN64
			txt.Format(_T("%I64X"), Address);
			#else
			txt.Format(_T("%X"), Address);
			#endif
			return txt;
		}
	}

	if (bHEX)
	{
		#ifdef _WIN64
		txt.Format(_T("%I64X"), Address);
		#else
		txt.Format(_T("%X"), Address);
		#endif
	}


	return txt;
}

CString GetModuleName(size_t Address)
{
	for (unsigned int i = 0; i < MemMapModule.size(); i++) {
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
			return MemMapModule[i].Name;
	}
	return _T("<unknown>");
}

size_t GetAddressFromName(CString moduleName)
{
	size_t moduleAddress = 0;
	for (unsigned int i = 0; i < MemMapModule.size(); i++) {
		if (MemMapModule[i].Name == moduleName) {
			moduleAddress = MemMapModule[i].Start;
			break;
		}
	}
	return moduleAddress;
}

bool IsProcHandleValid(HANDLE hProc)
{
	if (!hProc)
		return false;
	const DWORD RetVal = WaitForSingleObject(hProc, 0);
	if (RetVal == WAIT_FAILED)
		return false;
	return (RetVal == WAIT_TIMEOUT);
}

bool UpdateMemoryMap(void)
{
	MemMap.clear();
	MemMapCode.clear();
	MemMapData.clear();
	MemMapModule.clear();
	Exports.clear();
	CustomNames.clear();

	if (g_hProcess == NULL)
	{
		#ifdef _DEBUG
		PrintOut(_T("[UpdateMemoryMap]: Process handle NULL!"));
		#endif
		return 0;
	}

	if (!IsProcHandleValid(g_hProcess))
	{
		g_hProcess = NULL;
		#ifdef _DEBUG
		PrintOut(_T("[UpdateMemoryMap]: Process handle invalid!"));
		#endif
		return 0;
	}

	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);

	MEMORY_BASIC_INFORMATION MemInfo;
	size_t pMemory = (size_t)SysInfo.lpMinimumApplicationAddress;
	while (pMemory < (size_t)SysInfo.lpMaximumApplicationAddress)
	{
		if (VirtualQueryEx(g_hProcess, (LPCVOID)pMemory, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
		{
			if (MemInfo.State == MEM_COMMIT /*&& MemInfo.Type == MEM_PRIVATE*/)
			{
				MemMapInfo Mem;
				Mem.Start = (size_t)pMemory;
				Mem.End = (size_t)pMemory + MemInfo.RegionSize - 1;
				MemMap.push_back(Mem);
			}
			pMemory = (ULONG_PTR)MemInfo.BaseAddress + MemInfo.RegionSize;
		}
		else
		{
			pMemory += 1024;
		}
	}

	static HMODULE hNtDll = (HMODULE)Utils::GetLocalModuleHandle("ntdll.dll");
	static tNtQueryInformationProcess NtQueryInformationProcess = (tNtQueryInformationProcess)Utils::GetProcAddress(hNtDll, "NtQueryInformationProcess");

	PPROCESS_BASIC_INFORMATION ProcessInfo = NULL;
	PEB Peb;
	PEB_LDR_DATA LdrData;

	// Try to allocate buffer 
	HANDLE hHeap = GetProcessHeap();
	DWORD dwSize = sizeof(PROCESS_BASIC_INFORMATION);
	ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, dwSize);

	ULONG dwSizeNeeded = 0;
	NTSTATUS status = NtQueryInformationProcess(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSize, &dwSizeNeeded);
	if (status >= 0 && dwSize < dwSizeNeeded)
	{
		if (ProcessInfo)
			HeapFree(hHeap, 0, ProcessInfo);

		ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, dwSizeNeeded);
		if (!ProcessInfo)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateMemoryMap]: Couldn't allocate heap buffer!"));
			#endif
			return 0;
		}

		status = NtQueryInformationProcess(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSizeNeeded, &dwSizeNeeded);
	}

	// Did we successfully get basic info on process
	if (NT_SUCCESS(status))
	{	
		// Check for PEB
		if (!ProcessInfo->PebBaseAddress)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateMemoryMap]: PEB is null! Aborting UpdateExports!"));
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Read Process Environment Block (PEB)
		SIZE_T dwBytesRead = 0;
		if ( ReadMemory(  ProcessInfo->PebBaseAddress, &Peb, sizeof(PEB), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateMemoryMap]: Failed to read PEB! Aborting UpdateExports!"));
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Get Ldr
		dwBytesRead = 0;
		if ( ReadMemory( Peb.Ldr, &LdrData, sizeof(LdrData), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateMemoryMap]: Failed to read PEB Ldr Data! Aborting UpdateExports!"));
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		LIST_ENTRY *pLdrListHead = (LIST_ENTRY *)LdrData.InLoadOrderModuleList.Flink;
		LIST_ENTRY *pLdrCurrentNode = LdrData.InLoadOrderModuleList.Flink;
		do
		{
			LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
			dwBytesRead = 0;
			if (! ReadMemory( (void*)pLdrCurrentNode, &lstEntry, sizeof(LDR_DATA_TABLE_ENTRY), &dwBytesRead))
			{
				#ifdef _DEBUG
				PrintOut(_T("[UpdateMemoryMap]: Could not read list entry from LDR list. Error = %s"), Utils::GetLastErrorString().GetString());
				#endif
				if (ProcessInfo)
					HeapFree(hHeap, 0, ProcessInfo);
				return 0;
			}

			pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;

			if (lstEntry.DllBase != NULL /*&& lstEntry.SizeOfImage != 0*/)
			{
				unsigned char* ModuleBase = (unsigned char*)lstEntry.DllBase;
				DWORD ModuleSize = lstEntry.SizeOfImage;
				wchar_t wcsFullDllName[MAX_PATH] = { 0 };
				wchar_t* wcsModule = 0;
				if (lstEntry.FullDllName.Length > 0)
				{
					dwBytesRead = 0;
					if ( ReadMemory( (LPVOID)lstEntry.FullDllName.Buffer, &wcsFullDllName, lstEntry.FullDllName.Length, &dwBytesRead))
					{
						wcsModule = wcsrchr(wcsFullDllName, L'\\');
						if (!wcsModule)
							wcsModule = wcsrchr(wcsFullDllName, L'/');
						wcsModule++;

						if (g_AttachedProcessAddress == NULL)
						{
							wchar_t filename[MAX_PATH];
							GetModuleFileNameExW(g_hProcess, NULL, filename, MAX_PATH);
							if (_wcsicmp(filename, wcsFullDllName) == 0)
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
				Mem.Name = wcsModule;
				Mem.Path = wcsFullDllName;

				//PrintOut(_T("%s: %IX"), Mem.Name.GetBuffer(), Mem.Start);

				MemMapModule.push_back(Mem);

				// module code
				IMAGE_DOS_HEADER DosHdr;
				IMAGE_NT_HEADERS NtHdr;

				ReadMemory( ModuleBase, &DosHdr, sizeof(IMAGE_DOS_HEADER), NULL);
				ReadMemory( ModuleBase + DosHdr.e_lfanew, &NtHdr, sizeof(IMAGE_NT_HEADERS), NULL);
				DWORD sectionsSize = (DWORD)NtHdr.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
				PIMAGE_SECTION_HEADER sections = (PIMAGE_SECTION_HEADER)malloc(sectionsSize);
				ReadMemory( ModuleBase + DosHdr.e_lfanew + sizeof(IMAGE_NT_HEADERS), sections, sectionsSize, NULL);
				for (int i = 0; i < NtHdr.FileHeader.NumberOfSections; i++)
				{
					CString txt;
					MemMapInfo Mem;
					txt.Format(_T("%.8s"), sections[i].Name); txt.MakeLower();
					if (txt == ".text" || txt == "code")
					{
						Mem.Start = (size_t)ModuleBase + sections[i].VirtualAddress;
						Mem.End = Mem.Start + sections[i].Misc.VirtualSize;
						Mem.Name = wcsModule;
						MemMapCode.push_back(Mem);
					}
					if (txt == ".data" || txt == "data" || txt == ".rdata" || txt == ".idata")
					{
						Mem.Start = (size_t)ModuleBase + sections[i].VirtualAddress;
						Mem.End = Mem.Start + sections[i].Misc.VirtualSize;
						Mem.Name = wcsModule;
						MemMapData.push_back(Mem);
					}
				}
				delete sections;
			}

		} while (pLdrListHead != pLdrCurrentNode);
	}
	else
	{
		#ifdef _DEBUG
		PrintOut(_T("[UpdateExports]: NtQueryInformationProcess failed! Aborting..."));
		#endif
		if (ProcessInfo)
			HeapFree(hHeap, 0, ProcessInfo);
		return 0;
	}

	if (ProcessInfo)
		HeapFree(hHeap, 0, ProcessInfo);

	for (UINT i = 0; i < MemMap.size(); i++)
	{
		if (IsModule(MemMap[i].Start)) 
			MemMap[i].Name = GetModuleName(MemMap[i].Start);
	}

	return 1;
}

bool UpdateExports()
{
	Exports.clear();
	//if (!gbExports) 
	//	return;

	static HMODULE hNtDll = (HMODULE)Utils::GetLocalModuleHandle("ntdll.dll");
	static tNtQueryInformationProcess NtQueryInformationProcess = (tNtQueryInformationProcess)Utils::GetProcAddress(hNtDll, "NtQueryInformationProcess");

	PPROCESS_BASIC_INFORMATION ProcessInfo = NULL;
	PEB Peb;
	PEB_LDR_DATA LdrData;

	// Try to allocate buffer 
	HANDLE hHeap = GetProcessHeap();
	DWORD dwSize = sizeof(PROCESS_BASIC_INFORMATION);
	ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, dwSize);

	ULONG dwSizeNeeded = 0;
	NTSTATUS status = NtQueryInformationProcess(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSize, &dwSizeNeeded);
	if (status >= 0 && dwSize < dwSizeNeeded)
	{
		if (ProcessInfo)
			HeapFree(hHeap, 0, ProcessInfo);

		ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, dwSizeNeeded);
		if (!ProcessInfo)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateExports]: Couldn't allocate heap buffer!"));
			#endif
			return 0;
		}

		status = NtQueryInformationProcess(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSizeNeeded, &dwSizeNeeded);
	}

	// Did we successfully get basic info on process
	if (NT_SUCCESS(status))
	{	
		// Check for PEB
		if (!ProcessInfo->PebBaseAddress)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateExports]: PEB is null! Aborting..."));
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Read Process Environment Block (PEB)
		SIZE_T dwBytesRead = 0;
		if ( ReadMemory( ProcessInfo->PebBaseAddress, &Peb, sizeof(PEB), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateExports]: Failed to read PEB! Aborting UpdateExports."));
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Get Ldr
		dwBytesRead = 0;
		if ( ReadMemory( Peb.Ldr, &LdrData, sizeof(LdrData), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			PrintOut(_T("[UpdateExports]: Failed to read PEB Ldr Data! Aborting UpdateExports."));
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		LIST_ENTRY *pLdrListHead = (LIST_ENTRY *)LdrData.InLoadOrderModuleList.Flink;
		LIST_ENTRY *pLdrCurrentNode = LdrData.InLoadOrderModuleList.Flink;
		do
		{
			LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
			dwBytesRead = 0;
			if (! ReadMemory( (void*)pLdrCurrentNode, &lstEntry, sizeof(LDR_DATA_TABLE_ENTRY), &dwBytesRead))
			{
				#ifdef _DEBUG
				PrintOut(_T("[UpdateExports]: Could not read list entry from LDR list. Error = %s"), Utils::GetLastErrorString().GetString());
				#endif
				if (ProcessInfo)
					HeapFree(hHeap, 0, ProcessInfo);
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
					if ( ReadMemory( (LPVOID)lstEntry.BaseDllName.Buffer, &wcsDllName, lstEntry.BaseDllName.Length, &dwBytesRead))
					{
						wcscpy_s(ModuleName, wcsDllName);
					}
				}			

				IMAGE_DOS_HEADER DosHdr;
				IMAGE_NT_HEADERS NtHdr;

				ReadMemory( ModuleHandle, &DosHdr, sizeof(DosHdr), NULL);
				ReadMemory( ModuleHandle + DosHdr.e_lfanew, &NtHdr, sizeof(IMAGE_NT_HEADERS), NULL);
				if (NtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != 0)
				{
					IMAGE_EXPORT_DIRECTORY ExpDir;
					ReadMemory( (LPVOID)(ModuleHandle + NtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress), &ExpDir, sizeof(ExpDir), NULL);
					PVOID pName		= (PVOID)(ModuleHandle + ExpDir.AddressOfNames);
					PVOID pOrd		= (PVOID)(ModuleHandle + ExpDir.AddressOfNameOrdinals);
					PVOID pAddress	= (PVOID)(ModuleHandle + ExpDir.AddressOfFunctions);

					ULONG aNames[MAX_EXPORTS];
					WORD aOrds[MAX_EXPORTS];
					ULONG aAddresses[MAX_EXPORTS];
					ReadMemory( (LPVOID)pName, aNames, ExpDir.NumberOfNames * sizeof(aNames[0]), NULL);
					ReadMemory( (LPVOID)pOrd, aOrds, ExpDir.NumberOfNames * sizeof(aOrds[0]), NULL);
					ReadMemory( (LPVOID)pAddress, aAddresses, ExpDir.NumberOfFunctions * sizeof(aAddresses[0]), NULL);

					for (DWORD i = 0; i < ExpDir.NumberOfNames; i++)
					{
						char ExportName[256];
						ReadMemory( (LPVOID)(ModuleHandle + aNames[i]), ExportName, 256, NULL);
						DWORD_PTR Address = (DWORD_PTR)ModuleHandle + aAddresses[aOrds[i]];

						AddressName Entry;
						Entry.Name.Format(_T("%ls.%hs"), ModuleName, ExportName);
						Entry.Address = Address;
						// Add export entry to array
						Exports.push_back(Entry);
					}

				}
			}
		} while (pLdrListHead != pLdrCurrentNode);
	}
	else
	{
		#ifdef _DEBUG
		PrintOut(_T("[UpdateExports]: NtQueryInformationProcess failed! Aborting..."));
		#endif
		if (ProcessInfo)
			HeapFree(hHeap, 0, ProcessInfo);
		return 0;
	}

	if (ProcessInfo)
		HeapFree(hHeap, 0, ProcessInfo);

	return 1;
}

// Unused. Currently using _tcstoui64 as it is a lot faster.
__int64 StrToNum(const TCHAR *udata, int udatalen, int base)
{
	long index;
	const TCHAR numdigits[] = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	long digitValue = 0;
	__int64 RetVal = 0;
	TCHAR digits[sizeof(numdigits) + 1];
	TCHAR *dataVal;
	TCHAR data[512];
	//copy the data to our variable
	_tcscpy(data, udata);
	//convert it to upper case
	_tcsupr(data);
	ZeroMemory(digits, sizeof(digits));
	//copy the number of digits supported by base in digits
	_tcsncpy(digits, numdigits, base);
	for (index = 0; index < udatalen; index++)
	{
		dataVal = _tcschr(digits, data[index]);
		if (dataVal != 0)
		{
			//if it is subtract where to start point
			digitValue = long(dataVal - digits);
			//increment Retval with digitvalue
			RetVal = RetVal * base + digitValue;
		}
	}
	return RetVal;
}

int SplitString(const CString& input, const CString& delimiter, CStringArray& results)
{
	int iPos = 0;
	int newPos = -1;
	int sizeS2 = delimiter.GetLength();
	int isize = input.GetLength();

	CArray<INT, int> positions;

	newPos = input.Find(delimiter, 0);

	if (newPos < 0)
		return 0;

	int numFound = 0;

	while (newPos > iPos)
	{
		numFound++;
		positions.Add(newPos);
		iPos = newPos;
		newPos = input.Find(delimiter, iPos + sizeS2 + 1);
	}

	for (int i = 0; i <= positions.GetSize(); i++)
	{
		CString s;
		if (i == 0)
			s = input.Mid(i, positions[i]);
		else
		{
			int offset = positions[i - 1] + sizeS2;
			if (offset < isize)
			{
				if (i == positions.GetSize())
					s = input.Mid(offset);
				else if (i > 0)
					s = input.Mid(positions[i - 1] + sizeS2, positions[i] - positions[i - 1] - sizeS2);
			}
		}
		if (s.GetLength() > 0)
			results.Add(s);
	}
	return numFound;
}

size_t ConvertStrToAddress(CString str)
{
	//int foundIdx = -1;
	//if ((foundIdx = Spot.Text.FindOneOf(_T("+-"))) != -1)
	//{
	//	CString moduleName = Spot.Text.Left(foundIdx);
	//	foundIdx = Spot.Text.GetLength() - foundIdx;
	//	CString remainder = Spot.Text.Right(foundIdx);
	//	foundIdx = 0;
	//	while (1)
	//	{
	//		if (isspace(remainder[foundIdx]) || remainder[foundIdx] == _T('+'))
	//			foundIdx++;
	//		else
	//			break;
	//	}
	//	CString offsetString = remainder.GetBuffer() + foundIdx;
	//
	//	//offset = strtoul(Spot.Text,NULL,16);
	//
	//}

	CStringArray chunks;
	if (SplitString(str, "+", chunks) == 0)
		chunks.Add(str);

	size_t Final = 0;

	for (UINT i = 0; i < (UINT)chunks.GetCount(); i++)
	{
		CString a = chunks[i];

		a.MakeLower();  // Make all lowercase
		a.Trim();		// Trim whitespace
		a.Remove(_T('\"')); // Remove quotes

		bool bPointer = false;
		bool bMod = false;

		if (a.Find(_T(".exe")) != -1 || a.Find(_T(".dll")) != -1)
			bMod = true;

		if (a[0] == _T('*'))
		{
			bPointer = true;
			a = a.Mid(1);
		}
		else if (a[0] == _T('&'))
		{
			bMod = true;
			a = a.Mid(1);
		}

		size_t curadd = 0;

		if (bMod)
		{
			for (UINT i = 0; i < MemMapModule.size(); i++)
			{
				CString ModName = MemMapModule[i].Name;
				ModName.MakeLower();
				if (StrStr(ModName, a) != NULL)
				{
					curadd = MemMapModule[i].Start;
					bMod = true;
					break;
				}
			}
		} else {
			curadd = (size_t)_tcstoui64(a.GetBuffer(), NULL, 16);
		}

		Final += curadd;

		if (bPointer)
		{
			if ( ReadMemory( (void*)Final, &Final, sizeof(Final), NULL) == 0)
			{
				PrintOut(_T("[ConvertStrToAddress]: Failed to read memory GetLastError() = %s"), Utils::GetLastErrorString().GetString());
			}
		}
	}

	return Final;
}
