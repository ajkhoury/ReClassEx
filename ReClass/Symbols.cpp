#include "stdafx.h"
#include "Symbols.h"
#include <fstream>

Symbols::Symbols() :
	m_bInitialized(false)
{
	ResolveSearchPath();

	if (!WriteSymSrvDll())
		throw std::exception("WriteSymSrvDll_failed");

	if(!Init()) 
		throw std::exception("init_failed");
}

void Symbols::ResolveSearchPath()
{
	CString searchPath;
	LRESULT lRet;
	HKEY hKey = NULL;

	//C:\Users\User\AppData\Local\Temp\SymbolCache

	for (int i = 14; i >= 8; i--)
	{
		CString regPath = _T("Software\\Microsoft\\VisualStudio\\");
		wchar_t version[4];
		_itow_s(i, version, 10);

#ifdef UNICODE
		regPath.Append(version);
#else
		regPath.Append(CW2A(version));
#endif

		regPath.Append(_T(".0\\Debugger"));

		lRet = RegOpenKeyEx(HKEY_CURRENT_USER, regPath.GetString(), 0, KEY_READ, &hKey);
		if (hKey)
		{
			TCHAR szBuffer[MAX_PATH];
			DWORD dwBufferSize = MAX_PATH;
			lRet = RegQueryValueEx(hKey, _T("SymbolCacheDir"), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
			if (lRet == ERROR_SUCCESS && szBuffer)
			{
				searchPath = szBuffer;
				RegCloseKey(hKey);
				break;
			}
			RegCloseKey(hKey);
		}
	}

	if (!searchPath.IsEmpty())
	{
		m_strSearchPath.Format(_T("srv*%s*http://msdl.microsoft.com/download/symbols"), searchPath.GetString());
		PrintOut(_T("Symbol server path found from Visual Studio config: %s"), searchPath.GetString());
	}
	else
	{
		TCHAR szWindowsDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szWindowsDir);
		m_strSearchPath.Format(_T("srv*%s\\symbols*http://msdl.microsoft.com/download/symbols"), szWindowsDir);
		PrintOut(_T("Symbol server path not found, using windows dir: %s"), szWindowsDir);
	}
}

bool Symbols::WriteSymSrvDll()
{
	HRSRC hSymSrvRes = NULL;
	HGLOBAL hGlobal = NULL;
	LPVOID pSymSrvData = NULL;
	DWORD dwSymSrvDataSize = 0;

#ifdef _WIN64
	hSymSrvRes = FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA_SYMSRV64), RT_RCDATA);
#else
	hSymSrvRes = FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA_SYMSRV32), RT_RCDATA);
#endif
	if (hSymSrvRes != NULL)
	{
		hGlobal = LoadResource(NULL, hSymSrvRes);
		if (hGlobal != NULL)
		{
			pSymSrvData = LockResource(hGlobal);
			dwSymSrvDataSize = SizeofResource(NULL, hSymSrvRes);
			if (pSymSrvData != NULL)
			{
				std::ofstream fSymSrvDll(_T("symsrv.dll"), std::ios::binary);
				if (fSymSrvDll)
				{
					fSymSrvDll.write((const char*)pSymSrvData, dwSymSrvDataSize);
					fSymSrvDll.close();

					UnlockResource(hGlobal);
					FreeResource(hGlobal);

					return true;
				}
			}
		}

		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}

	return false;
}

Symbols::~Symbols()
{
	Cleanup();
	DeleteFile(_T("symsrv.dll"));
}

void Symbols::Cleanup()
{
	if (m_bInitialized)
	{
		for (auto it = symbols.begin(); it != symbols.end(); ++it)
			delete it->second;

		symbols.clear();
		CoUninitialize();
		m_bInitialized = false;
	}
}

bool Symbols::Init()
{
	if (!m_bInitialized)
	{
		HRESULT hr = S_OK;
		hr = CoInitialize(NULL);
		if (FAILED(hr))
		{
			PrintOut(_T("[Symbols::Init] CoInitialize failed - HRESULT = %08X"), hr);
			return false;
		}
		m_bInitialized = true;
	}

	return true;
}

bool Symbols::LoadSymbolsForModule(CString ModulePath, size_t dwBaseAddr, DWORD dwSizeOfImage)
{
	int idx = ModulePath.ReverseFind('/');
	if (idx == -1)
		idx = ModulePath.ReverseFind('\\');
	CString ModuleName = ModulePath.Mid(++idx);

	const TCHAR* szSearchPath = 0;
	if (!m_strSearchPath.IsEmpty())
		szSearchPath = m_strSearchPath.GetString();

	SymbolReader* reader = new SymbolReader();
	if (reader->LoadFile(ModuleName, ModulePath, dwBaseAddr, dwSizeOfImage, szSearchPath))
	{
		PrintOut(_T("[Symbols::LoadSymbolsForModule] Symbols for module %s loaded"), ModuleName.GetString());
		symbols.insert(std::make_pair(ModuleName, reader));
		return true;
	}

	delete reader;

	return false;
}

bool Symbols::LoadSymbolsForPdb(CString PdbPath)
{
	int idx = PdbPath.ReverseFind('/');
	if (idx == -1)
		idx = PdbPath.ReverseFind('\\');
	CString PdbFileName = PdbPath.Mid(++idx);

	const TCHAR* szSearchPath = 0;
	if (!m_strSearchPath.IsEmpty())
		szSearchPath = m_strSearchPath.GetString();

	SymbolReader* reader = new SymbolReader();
	if (reader->LoadFile(PdbFileName, PdbPath, 0, 0, szSearchPath))
	{
		PrintOut(_T("[Symbols::LoadSymbolsForPdb] Symbols for module %s loaded"), PdbFileName.GetString());
		symbols.insert(std::make_pair(PdbFileName, reader));
		return true;
	}

	delete reader;

	return false;
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

SymbolReader* Symbols::GetSymbolsForModule(CString module)
{
	SymbolReader* script = nullptr;
	auto iter = symbols.find(module);
	if (iter != symbols.end())
		script = iter->second;
	return script;
}


