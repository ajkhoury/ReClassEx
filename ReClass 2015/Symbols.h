#pragma once

#include "SymbolReader.h"
#include <map>

class Symbols
{
public:
	Symbols();
	Symbols(HANDLE hProcess);
	~Symbols();

	void Cleanup();
	bool Init(HANDLE hProcess);
	bool IsInitialized() { return m_bInitialized; }

	void LoadModuleSymbols();

	SymbolReader* GetSymbolsForModule(CString module);

private:
	bool LoadSymbolsForModule(wchar_t* szModulePath, size_t dwBaseAddr, DWORD dwSizeOfImage);

	//typedef std::pair<CString, size_t> key_type;
	std::map<CString, SymbolReader*> symbols;

	bool m_bInitialized;
	HANDLE m_hProcess;
};

extern Symbols sym;