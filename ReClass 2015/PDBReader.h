#pragma once

#include <DbgHelp.h>

class PDBReader
{
public:
	PDBReader();

	bool Init();
	bool LoadFile(PCSTR FilePath);
	bool GetSymbolNameFromAddress(size_t Address, CString& NameOut);

private:
	bool m_bInitialized;
};
