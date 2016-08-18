#include "stdafx.h"
#include "Symbols.h"

#include <vector>
#include <fstream>
#include <direct.h>
#define GetCurrentDir getcwd

Symbols sym;

Symbols::Symbols() :
	m_bInitialized(false)
{
}

Symbols::Symbols(HANDLE hProcess)
{
	Init(hProcess);
}

Symbols::~Symbols()
{
	Cleanup();
}

void Symbols::Cleanup()
{
	if (m_bInitialized)
	{
		for (auto it = symbols.begin(); it != symbols.end(); ++it) {
			delete it->second;
		}
		symbols.clear();

		CoUninitialize();

		m_bInitialized = false;
	}
}

bool Symbols::Init(HANDLE hProcess)
{
	HRESULT hr = S_OK;

	Cleanup();

	if (hProcess) 
	{
		hr = CoInitialize(NULL);
		if (FAILED(hr))
		{
			PrintOut(_T("[LoadDataFromPdb] CoInitialize failed - HRESULT = %08X"), hr);
			return false;
		}

		m_hProcess = hProcess;
		m_bInitialized = true;
	}

	return m_bInitialized;
}

bool Symbols::LoadSymbolsForModule(wchar_t* szModulePath, size_t dwBaseAddr, DWORD dwSizeOfImage)
{
#ifdef UNICODE
	CString FilePath(szModulePath);
#else
	CString FilePath(CW2A(szModulePath));
#endif

	int idx = FilePath.ReverseFind('/');
	if (idx == -1)
		idx = FilePath.ReverseFind('\\');
	CString FileName = FilePath.Mid(++idx);

	SymbolReader* reader = new SymbolReader();
	if (reader->LoadFile(FileName, FilePath, dwBaseAddr, dwSizeOfImage))
	{
		symbols.insert(std::make_pair(FileName, reader));
		return true;
	}

	delete reader;

	return false;
}

void Symbols::LoadModuleSymbols()
{
	PPROCESS_BASIC_INFORMATION pbi = NULL;
	PEB peb;
	PEB_LDR_DATA peb_ldr;

	// Try to allocate buffer 
	HANDLE	hHeap = GetProcessHeap();
	DWORD dwSize = sizeof(PROCESS_BASIC_INFORMATION);
	pbi = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSize);

	ULONG dwSizeNeeded = 0;
	NTSTATUS dwStatus = ntdll::NtQueryInformationProcess(m_hProcess, ProcessBasicInformation, pbi, dwSize, &dwSizeNeeded);
	if (NT_SUCCESS(dwStatus) && dwSize < dwSizeNeeded)
	{
		if (pbi)
			HeapFree(hHeap, 0, pbi);

		pbi = (PPROCESS_BASIC_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, dwSizeNeeded);
		if (!pbi) {
			_tprintf(_T("[LoadModuleSymbols] Couldn't allocate heap buffer!\n"));
			return;
		}

		dwStatus = ntdll::NtQueryInformationProcess(m_hProcess, ProcessBasicInformation, pbi, dwSizeNeeded, &dwSizeNeeded);
	}

	// Did we successfully get basic info on process
	if (NT_SUCCESS(dwStatus))
	{
		// Read Process Environment Block (PEB)
		if (pbi->PebBaseAddress)
		{
			SIZE_T dwBytesRead = 0;
			if (ReadProcessMemory(m_hProcess, pbi->PebBaseAddress, &peb, sizeof(peb), &dwBytesRead))
			{
				dwBytesRead = 0;
				if (ReadProcessMemory(m_hProcess, peb.Ldr, &peb_ldr, sizeof(peb_ldr), &dwBytesRead))
				{
					LIST_ENTRY *pLdrListHead = (LIST_ENTRY *)peb_ldr.InLoadOrderModuleList.Flink;
					LIST_ENTRY *pLdrCurrentNode = peb_ldr.InLoadOrderModuleList.Flink;
					do
					{
						LDR_DATA_TABLE_ENTRY lstEntry = { 0 };
						dwBytesRead = 0;
						if (!ReadProcessMemory(m_hProcess, (void*)pLdrCurrentNode, &lstEntry, sizeof(LDR_DATA_TABLE_ENTRY), &dwBytesRead))
						{
							_tprintf(_T("[LoadModuleSymbols] Could not read list entry from LDR list. Error: %s\n"), Utils::GetLastErrorAsString().c_str());
							if (pbi)
								HeapFree(hHeap, 0, pbi);
							return;
						}

						pLdrCurrentNode = lstEntry.InLoadOrderLinks.Flink;

						wchar_t wcsFullDllName[MAX_PATH] = { 0 };
						if (lstEntry.FullDllName.Length > 0)
						{
							dwBytesRead = 0;
							if (!ReadProcessMemory(m_hProcess, (LPCVOID)lstEntry.FullDllName.Buffer, &wcsFullDllName, lstEntry.FullDllName.Length, &dwBytesRead))
							{
								_tprintf(_T("[LoadModuleSymbols] Could not read list entry DLL name. Error: %s\n"), Utils::GetLastErrorAsString().c_str());
								if (pbi)
									HeapFree(hHeap, 0, pbi);
								return;
							}
						}

						if (lstEntry.DllBase != 0 && lstEntry.SizeOfImage != 0)
						{
							if (LoadSymbolsForModule(wcsFullDllName, (size_t)lstEntry.DllBase, lstEntry.SizeOfImage)) {
								PrintOut(_T("Symbol module %ls loaded"), wcsFullDllName);
							}
						}
					} while (pLdrListHead != pLdrCurrentNode);

				} // Get Ldr
			} // Read PEB 
		} // Check for PEB
	}

	if (pbi)
		HeapFree(hHeap, 0, pbi);
}

SymbolReader* Symbols::GetSymbolsForModule(CString module)
{
	SymbolReader* script = nullptr;
	auto iter = symbols.find(module);
	if (iter != symbols.end())
		script = iter->second;
	return script;
}


