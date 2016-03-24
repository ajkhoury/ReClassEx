#include "stdafx.h"

//Globals
HANDLE g_hProcess = NULL;
DWORD ProcessID = NULL;

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
bool gbFloat = true;
bool gbInt = true;
bool gbString = true;
bool gbPointers = true;

bool gbTop = true;
bool gbClassBrowser = true;
bool gbFilterProcesses = false;

CString tdHex("char");
CString tdInt64("__int64");
CString tdInt32("__int32");
CString tdInt16("__int16");
CString tdInt8("__int8");
CString tdDWORD("DWORD");
CString tdWORD("WORD");
CString tdBYTE("unsigned char");
CString tdFloat("float");
CString tdDouble("double");
CString tdVec2("Vector2");
CString tdVec3("Vector3");
CString tdQuat("Vector4");
CString tdMatrix("matrix3x4_t");
CString tdPChar("char*");

std::vector<HICON> Icons;

void ReadMemory(DWORD_PTR Address, void* Buffer, DWORD Size)
{
	if (ReadProcessMemory(g_hProcess, (void*)Address, Buffer, Size, NULL) == 0)
	{
		// printf( "[!] Failed to read memory @ %I64x GetLastError( ) = %p\n", Address, GetLastError( ) );
		ZeroMemory(Buffer, Size);
	}
}

void WriteMemory(DWORD_PTR Address, void* Buffer, DWORD Size)
{
	DWORD OldProtect;
	//VirtualProtectEx(hProcess,(void*)Address,Size,PAGE_READWRITE,&OldProtect); <- srsly PAGE_READWRITE? O_o
	VirtualProtectEx(g_hProcess, (void*)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
	WriteProcessMemory(g_hProcess, (void*)Address, Buffer, Size, NULL);
	VirtualProtectEx(g_hProcess, (void*)Address, Size, OldProtect, NULL);
	//{
	//	CString e;
	//	e.Format("Error: %0.8X (%i) %0.8X<-%i",GetLastError(),GetLastError(),Address,((BYTE*)Buffer)[0]);
	//	MessageBox(NULL,e,"Info",MB_OK);
	//}
}

CString ReadMemoryString(DWORD_PTR address, SIZE_T max)
{
	char	buffer[1024];
	SIZE_T	bytesRead;

	if (ReadProcessMemory(g_hProcess, (PVOID)address, buffer, max, &bytesRead) != 0)
	{
		//printf( "Read: %p\n", address );
		//printf( "Bytes Read: %d\n", bytesRead );
		//printf( "String %s\n", buffer );

		for (int i = 0; i < bytesRead; i++)
		{
			// If not a printable character and is not null terminator replace with '.'
			if (!(isprint(buffer[i] & 0xFF)) && buffer[i] != '\0')
			{
				buffer[i] = '.';
			}
		}

		// Terminate at max
		buffer[bytesRead] = '\0';

		return buffer;
	}
	else
	{
		//printf( "Failed to read memory, GetLastError( )=%p\n", GetLastError( ) );
		return "..";
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

	// Look for new threads and update existing ones.
	for (ULONG i = 0; i < numberOfThreads; i++)
	{
		SYSTEM_THREAD_INFORMATION thread = threads[i];
		//if (!thread)
			//continue;
		DWORD thId = (DWORD)thread.ClientId.UniqueThread;
		HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thId);
		if (bResumeThread)
			ResumeThread(hThread);
		else
			SuspendThread(hThread);
		CloseHandle(hThread);
	}

	return 1;
}

std::vector<MemMapInfo> MemMap;
std::vector<MemMapInfo> MemMapCode;
std::vector<MemMapInfo> MemMapData;
std::vector<MemMapInfo> MemMapModule;
std::vector<AddressName> Exports;
std::vector<AddressName> CustomNames;

bool IsCode(DWORD_PTR Address)
{
	for (UINT i = 0; i < MemMapCode.size(); i++)
	{
		if (Address >= MemMapCode[i].Start && Address <= MemMapCode[i].End)
			return true;
	}
	return false;
}

bool IsData(DWORD_PTR Address)
{
	for (UINT i = 0; i < MemMapData.size(); i++)
	{
		if (Address >= MemMapData[i].Start && Address <= MemMapData[i].End)
			return true;
	}
	return false;
}

DWORD_PTR GetBase()
{
	if (MemMap.size() > 1)
		MemMap.at(0).Start;

	//for (UINT i=0; i < MemMap.size();i++)
	//{
	//	if ( Address >= MemMap[i].Start && Address <= MemMap[i].End )
	//		return true;
	//}
#ifdef _WIN64
	return 0x140000000;
#else
	return 0x400000;
#endif
}

bool IsMemory(DWORD_PTR Address)
{
	for (UINT i = 0; i < MemMap.size(); i++)
	{
		if (Address >= MemMap[i].Start && Address <= MemMap[i].End)
			return true;
	}
	return false;
}

bool IsModule(DWORD_PTR Address)
{
	for (UINT i = 0; i < MemMapModule.size(); i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
			return true;
	}
	return false;
}

CString GetAddressName(DWORD_PTR Address, bool bHEX)
{
	CString txt;
	for (UINT i = 0; i < CustomNames.size(); i++)
	{
		if (Address == CustomNames[i].Address)
		{
#ifdef _WIN64
			txt.Format("%s.%I64X", CustomNames[i].Name, Address);
#else
			txt.Format("%s.%X", CustomNames[i].Name, Address);
#endif

			return txt;
		}
	}
	for (UINT i = 0; i < Exports.size(); i++)
	{
		if (Address == Exports[i].Address)
		{
#ifdef _WIN64
			txt.Format("%s.%I64X", Exports[i].Name, Address);
#else
			txt.Format("%s.%X", Exports[i].Name, Address);
#endif

			return txt;
		}
	}
	for (UINT i = 0; i < MemMapCode.size(); i++)
	{
		if (Address >= MemMapCode[i].Start && Address <= MemMapCode[i].End)
		{
#ifdef _WIN64
			txt.Format("<CODE>%s.%I64X", MemMapCode[i].Name, Address);
#else
			txt.Format("<CODE>%s.%X", MemMapCode[i].Name, Address);
#endif

			return txt;
		}
	}
	for (UINT i = 0; i < MemMapData.size(); i++)
	{
		if (Address >= MemMapData[i].Start && Address <= MemMapData[i].End)
		{
#ifdef _WIN64
			txt.Format("<DATA>%s.%I64X", MemMapData[i].Name, Address);
#else
			txt.Format("<DATA>%s.%X", MemMapData[i].Name, Address);
#endif

			return txt;
		}
	}
	for (UINT i = 0; i < MemMapModule.size(); i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
		{
#ifdef _WIN64
			txt.Format("%s.%I64X", MemMapModule[i].Name, Address);
#else
			txt.Format("%s.%X", MemMapModule[i].Name, Address);
#endif

			return txt;
		}
	}
	for (UINT i = 0; i < MemMap.size(); i++)
	{
		if (Address >= MemMap[i].Start && Address <= MemMap[i].End)
		{
#ifdef _WIN64
			txt.Format("%I64X", Address);
#else
			txt.Format("%X", Address);
#endif

			return txt;
		}
	}

	if (bHEX)
	{
#ifdef _WIN64
		txt.Format("%I64X", Address);
#else
		txt.Format("%X", Address);
#endif
	}


	return txt;
}

CString GetModuleName(DWORD_PTR Address)
{
	for (unsigned int i = 0; i < MemMapModule.size(); i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
			return MemMapModule[i].Name;
	}
	return "<unknown>";
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
		printf("Reclass[UpdateMemoryMap]: Process handle NULL!\n");
		#endif
		return 0;
	}
	if (!IsProcHandleValid(g_hProcess))
	{
		g_hProcess = NULL;
		#ifdef _DEBUG
		printf("Reclass[UpdateMemoryMap]: Process handle invalid!\n");
		#endif
		return 0;
	}

	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);

	MEMORY_BASIC_INFORMATION MemInfo;
	ULONG_PTR pMemory = (ULONG_PTR)SysInfo.lpMinimumApplicationAddress;
	while (pMemory < (ULONG_PTR)SysInfo.lpMaximumApplicationAddress)
	{
		if (VirtualQueryEx(g_hProcess, (LPCVOID)pMemory, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
		{
			if (MemInfo.State == MEM_COMMIT /*&& MBI.Type == MEM_PRIVATE*/)
			{
				MemMapInfo Mem;
				Mem.Start = (DWORD_PTR)pMemory;
				Mem.End = (DWORD_PTR)pMemory + MemInfo.RegionSize - 1;
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
	static tNtQueryInformationProcess fnNTQIP = (tNtQueryInformationProcess)Utils::GetProcAddress(hNtDll, "NtQueryInformationProcess");

	PPROCESS_BASIC_INFORMATION ProcessInfo = NULL;
	PEB Peb;
	PEB_LDR_DATA LdrData;

	// Try to allocate buffer 
	HANDLE hHeap = GetProcessHeap();
	DWORD dwSize = sizeof(PROCESS_BASIC_INFORMATION);
	ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSize);

	ULONG dwSizeNeeded = 0;
	NTSTATUS status = fnNTQIP(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSize, &dwSizeNeeded);
	if (status >= 0 && dwSize < dwSizeNeeded)
	{
		if (ProcessInfo)
			HeapFree(hHeap, 0, ProcessInfo);

		ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSizeNeeded);
		if (!ProcessInfo)
		{
			#ifdef _DEBUG
			printf("Reclass[UpdateMemoryMap]: Couldn't allocate heap buffer!\n");
			#endif
			return 0;
		}

		status = fnNTQIP(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSizeNeeded, &dwSizeNeeded);
	}

	// Did we successfully get basic info on process
	if (NT_SUCCESS(status))
	{	
		// Check for PEB
		if (!ProcessInfo->PebBaseAddress)
		{
			#ifdef _DEBUG
			printf("Reclass[UpdateMemoryMap]: PEB is null! Aborting UpdateExports.\n");
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Read Process Environment Block (PEB)
		SIZE_T dwBytesRead = 0;
		if (ReadProcessMemory(g_hProcess, ProcessInfo->PebBaseAddress, &Peb, sizeof(PEB), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			printf("Reclass[UpdateMemoryMap]: Failed to read PEB! Aborting UpdateExports.\n");
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Get Ldr
		dwBytesRead = 0;
		if (ReadProcessMemory(g_hProcess, Peb.Ldr, &LdrData, sizeof(LdrData), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			printf("Reclass[UpdateMemoryMap]: Failed to read PEB Ldr Data! Aborting UpdateExports.\n");
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
			if (!ReadProcessMemory(g_hProcess, (void*)pLdrCurrentNode, &lstEntry, sizeof(LDR_DATA_TABLE_ENTRY), &dwBytesRead))
			{
				#ifdef _DEBUG
				printf("Reclass[UpdateMemoryMap]: Could not read list entry from LDR list. Error = %X\n", GetLastError());
				#endif
				if (ProcessInfo)
					HeapFree(hHeap, 0, ProcessInfo);
				return 0;
			}

			pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;

			if (lstEntry.DllBase != nullptr && lstEntry.SizeOfImage != 0)
			{

				unsigned char* ModuleBase = (unsigned char*)lstEntry.DllBase;
				DWORD ModuleSize = lstEntry.SizeOfImage;
				wchar_t wcsDllName[MAX_PATH] = { 0 };
				char szModule[MAX_PATH] = { 0 };
				if (lstEntry.BaseDllName.Length > 0)
				{
					dwBytesRead = 0;
					if (ReadProcessMemory(g_hProcess, (LPCVOID)lstEntry.FullDllName.Buffer, &wcsDllName, lstEntry.FullDllName.Length, &dwBytesRead))
					{
						wchar_t* pathEnd = 0;
						pathEnd = wcsrchr(wcsDllName, L'\\');
						if (!pathEnd)
							pathEnd = wcsrchr(wcsDllName, L'/');
						pathEnd++;
						wcstombs(szModule, pathEnd, wcslen(pathEnd) + 1);
					} 
				}
				// module info
				MemMapInfo Mem;
				Mem.Start = (DWORD_PTR)ModuleBase;
				Mem.End = Mem.Start + ModuleSize;
				Mem.Name = szModule;
				MemMapModule.push_back(Mem);

				// module code
				IMAGE_DOS_HEADER DosHdr;
				IMAGE_NT_HEADERS NtHdr;

				ReadProcessMemory(g_hProcess, ModuleBase, &DosHdr, sizeof(IMAGE_DOS_HEADER), NULL);
				ReadProcessMemory(g_hProcess, ModuleBase + DosHdr.e_lfanew, &NtHdr, sizeof(IMAGE_NT_HEADERS), NULL);
				DWORD ssize = (DWORD)NtHdr.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
				PIMAGE_SECTION_HEADER sh = (PIMAGE_SECTION_HEADER)(new char[ssize]);
				ReadProcessMemory(g_hProcess, ModuleBase + DosHdr.e_lfanew + sizeof(IMAGE_NT_HEADERS), sh, ssize, NULL);
				for (int i = 0; i < NtHdr.FileHeader.NumberOfSections; i++)
				{
					CString txt;
					MemMapInfo Mem;
					txt.Format("%.8s", sh[i].Name); txt.MakeLower();
					if (txt == ".text" || txt == "code")
					{
						Mem.Start = (DWORD_PTR)ModuleBase + sh[i].VirtualAddress;
						Mem.End = Mem.Start + sh[i].Misc.VirtualSize;
						Mem.Name = szModule;
						MemMapCode.push_back(Mem);
					}
					if (txt == ".data" || txt == "data" || txt == ".rdata" || txt == ".idata")
					{
						Mem.Start = (DWORD_PTR)ModuleBase + sh[i].VirtualAddress;
						Mem.End = Mem.Start + sh[i].Misc.VirtualSize;
						Mem.Name = szModule;
						MemMapData.push_back(Mem);
					}
				}
				delete sh;
			}

		} while (pLdrListHead != pLdrCurrentNode);
	}
	else
	{
		#ifdef _DEBUG
		printf("Reclass[UpdateExports]: NtQueryInformationProcess failed! Aborting UpdateExports.\n");
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
}

bool UpdateExports()
{
	Exports.clear();
	//if (!gbExports) return;

	static HMODULE hNtDll = (HMODULE)Utils::GetLocalModuleHandle("ntdll.dll");
	static tNtQueryInformationProcess fnNTQIP = (tNtQueryInformationProcess)Utils::GetProcAddress(hNtDll, "NtQueryInformationProcess");

	PPROCESS_BASIC_INFORMATION ProcessInfo = NULL;
	PEB Peb;
	PEB_LDR_DATA LdrData;

	// Try to allocate buffer 
	HANDLE hHeap = GetProcessHeap();
	DWORD dwSize = sizeof(PROCESS_BASIC_INFORMATION);
	ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSize);

	ULONG dwSizeNeeded = 0;
	NTSTATUS status = fnNTQIP(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSize, &dwSizeNeeded);
	if (status >= 0 && dwSize < dwSizeNeeded)
	{
		if (ProcessInfo)
			HeapFree(hHeap, 0, ProcessInfo);

		ProcessInfo = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSizeNeeded);
		if (!ProcessInfo)
		{
			#ifdef _DEBUG
			printf("Couldn't allocate heap buffer!\n");
			#endif
			return 0;
		}

		status = fnNTQIP(g_hProcess, ProcessBasicInformation, ProcessInfo, dwSizeNeeded, &dwSizeNeeded);
	}

	// Did we successfully get basic info on process
	if (NT_SUCCESS(status))
	{	
		// Check for PEB
		if (!ProcessInfo->PebBaseAddress)
		{
			#ifdef _DEBUG
			printf("Reclass[UpdateExports]: PEB is null! Aborting UpdateExports.\n");
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Read Process Environment Block (PEB)
		SIZE_T dwBytesRead = 0;
		if (ReadProcessMemory(g_hProcess, ProcessInfo->PebBaseAddress, &Peb, sizeof(PEB), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			printf("Reclass[UpdateExports]: Failed to read PEB! Aborting UpdateExports.\n");
			#endif
			if (ProcessInfo)
				HeapFree(hHeap, 0, ProcessInfo);
			return 0;
		}

		// Get Ldr
		dwBytesRead = 0;
		if (ReadProcessMemory(g_hProcess, Peb.Ldr, &LdrData, sizeof(LdrData), &dwBytesRead) == 0)
		{
			#ifdef _DEBUG
			printf("Reclass[UpdateExports]: Failed to read PEB Ldr Data! Aborting UpdateExports.\n");
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
			if (!ReadProcessMemory(g_hProcess, (void*)pLdrCurrentNode, &lstEntry, sizeof(LDR_DATA_TABLE_ENTRY), &dwBytesRead))
			{
				#ifdef _DEBUG
				printf("Reclass[UpdateExports]: Could not read list entry from LDR list. Error = %X\n", GetLastError());
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
				char szDllName[MAX_PATH] = { 0 };
				if (lstEntry.BaseDllName.Length > 0)
				{		
					dwBytesRead = 0;
					if (ReadProcessMemory(g_hProcess, (LPCVOID)lstEntry.FullDllName.Buffer, &wcsDllName, lstEntry.FullDllName.Length, &dwBytesRead))
					{
						wchar_t* pathEnd = 0;
						pathEnd = wcsrchr(wcsDllName, L'\\');
						if (!pathEnd)
							pathEnd = wcsrchr(wcsDllName, L'/');
						pathEnd++;
						wcstombs(szDllName, pathEnd, wcslen(pathEnd) + 1);
					}
				}			

				IMAGE_DOS_HEADER DosHdr;
				IMAGE_NT_HEADERS NtHdr;

				ReadProcessMemory(g_hProcess, ModuleHandle, &DosHdr, sizeof(DosHdr), NULL);
				ReadProcessMemory(g_hProcess, ModuleHandle + DosHdr.e_lfanew, &NtHdr, sizeof(IMAGE_NT_HEADERS), NULL);
				if (NtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != 0)
				{
					IMAGE_EXPORT_DIRECTORY ExpDir;
					ReadProcessMemory(g_hProcess, (LPCVOID)(ModuleHandle + NtHdr.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress), &ExpDir, sizeof(ExpDir), NULL);
					PVOID pName		= (void*)(ModuleHandle + ExpDir.AddressOfNames);
					PVOID pOrd		= (void*)(ModuleHandle + ExpDir.AddressOfNameOrdinals);
					PVOID pAddress	= (void*)(ModuleHandle + ExpDir.AddressOfFunctions);

					ULONG aNames[MAX_EXPORTS];
					WORD aOrds[MAX_EXPORTS];
					ULONG aAddresses[MAX_EXPORTS];
					ReadProcessMemory(g_hProcess, (LPCVOID)pName, aNames, ExpDir.NumberOfNames * sizeof(aNames[0]), NULL);
					ReadProcessMemory(g_hProcess, (LPCVOID)pOrd, aOrds, ExpDir.NumberOfNames * sizeof(aOrds[0]), NULL);
					ReadProcessMemory(g_hProcess, (LPCVOID)pAddress, aAddresses, ExpDir.NumberOfFunctions * sizeof(aAddresses[0]), NULL);

					for (DWORD i = 0; i < ExpDir.NumberOfNames; i++)
					{
						char ExportName[256];
						ReadProcessMemory(g_hProcess, (LPCVOID)(ModuleHandle + aNames[i]), ExportName, 256, NULL);
						DWORD_PTR Address = (DWORD_PTR)ModuleHandle + aAddresses[aOrds[i]];

						AddressName Entry;
						Entry.Name.Format("%s.%s", szDllName, ExportName);
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
		printf("Reclass[UpdateExports]: NtQueryInformationProcess failed! Aborting UpdateExports.\n");
		#endif
		if (ProcessInfo)
			HeapFree(hHeap, 0, ProcessInfo);
		return 0;
	}

	if (ProcessInfo)
		HeapFree(hHeap, 0, ProcessInfo);

	return 1;
}

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

	//printf( "ptr %s %d\n", udata, udatalen );

	for (index = 0; index < udatalen; index++)
	{
		//is the number there
		dataVal = _tcschr(digits, data[index]);
		//

		if (dataVal != 0)
		{
			//if it is subtract where to start point
			digitValue = long(dataVal - digits);
			//printf( "->%X", digitValue );
			//increment Retval with digitvalue
			RetVal = RetVal * base + digitValue;

			//printf( " ( %p )", RetVal );
		}
	}

	// printf( "retval %p\n", RetVal );

	//return the result
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

	if (newPos < 0) { return 0; }

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
					s = input.Mid(positions[i - 1] + sizeS2,
						positions[i] - positions[i - 1] - sizeS2);
			}
		}
		if (s.GetLength() > 0)
			results.Add(s);
	}
	return numFound;
}

DWORD_PTR ConvertStrToAddress(CString Address)
{
	CStringArray chuncks;

	if (SplitString(Address, "+", chuncks) == 0)
		chuncks.Add(Address);

	DWORD_PTR Final = 0;

	for (UINT i = 0; i < (UINT)chuncks.GetCount(); i++)
	{
		CString a = chuncks[i];
		a.MakeLower();
		a.Trim();

		bool bPointer = false;
		bool bMod = false;

		if (a[0] == '*')
		{
			bPointer = true;
			a = a.Mid(1);
		}
		if (a[0] == '&')
		{
			bMod = true;
			a = a.Mid(1);
		}

		DWORD_PTR curadd = 0;

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
		}
		else
		{
			curadd = (__int64)StrToNum(a.GetBuffer(), a.GetLength(), 16);
			//printf( "Final [%p] %d\n", curadd, a.GetLength( ) );
		}

		Final += curadd;

		if (bPointer)
		{
			//printf( "here2\n" );
			if (ReadProcessMemory(g_hProcess, (PVOID)Final, &Final, sizeof(Final), NULL) == 0)
			{
				printf("[!] Failed to read memory (stdafx.cpp) GetLastError( ) = %p\n", GetLastError());
			}
		}
	}

	return Final;
}