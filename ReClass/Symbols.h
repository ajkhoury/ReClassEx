#pragma once

#include "SymbolReader.h"
#include <map>

class Symbols
{
public:
	Symbols( );

	void ResolveSearchPath( );
	BOOLEAN WriteSymSrvDll( );

	~Symbols( );

	void Cleanup( );
	BOOLEAN Init( );
	BOOLEAN IsInitialized( ) { return m_bInitialized; }

	BOOLEAN LoadSymbolsForModule( CString szModulePath, ULONG_PTR ModuleBaseAddress, ULONG SizeOfModule );
	BOOLEAN LoadSymbolsForPdb( CString szPdbPath );

	SymbolReader* GetSymbolsForModuleAddress( ULONG_PTR ModuleAddress );
	SymbolReader* GetSymbolsForModuleName( CString ModuleName );

private:

	std::map<CString, SymbolReader*> m_SymbolNames;
	std::map<ULONG_PTR, SymbolReader*> m_SymbolAddresses;

	BOOLEAN m_bInitialized;

	CString m_strSearchPath;
};
