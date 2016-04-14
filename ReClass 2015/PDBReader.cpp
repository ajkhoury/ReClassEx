#include "stdafx.h"

#include "PDBReader.h"

PDBReader::PDBReader()
{
	Init();
}

bool PDBReader::Init()
{
	m_bInitialized = SymInitialize(g_hProcess, NULL, TRUE) ? true : false;
	if (!m_bInitialized)
	{
		PrintOut(_T("[PDBReader::Init] Failed to initialize!"));
	}
}

bool PDBReader::LoadFile(PCSTR FilePath)
{
	if (!m_bInitialized)
		return false;
	return SymSetSearchPath(g_hProcess, FilePath) ? true : false;
}

bool PDBReader::GetSymbolNameFromAddress(size_t Address, CString& NameOut)
{
	if (!m_bInitialized)
		return false;

	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;

	if (SymFromAddr(g_hProcess, Address, 0, pSymbol))
	{
		NameOut = CString(pSymbol->Name);
		return true;
	}
	else
	{
		//NameOut = CString("");
		return false;
	}
}