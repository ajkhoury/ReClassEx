#include "stdafx.h"
#include "windows.h"
#include "stdio.h"


enum NodeType
{
	nt_base,
	nt_instance,
	nt_struct,
	nt_hidden,
	nt_hex32,
	nt_hex64,
	nt_hex16,
	nt_hex8,
	nt_pointer,
	nt_int64,
	nt_int32,
	nt_int16,
	nt_int8,
	nt_float,
	nt_double,
	nt_uint32,
	nt_uint16,
	nt_uint8,
	nt_text,
	nt_unicode,
	nt_function,
	nt_custom,
	nt_vec2,
	nt_vec3,
	nt_quat,
	nt_matrix,
	nt_vtable,
	nt_array,
	nt_class,
	nt_enum,
	nt_pchar
};

class ReclassSDK
{
public:
	virtual void Test( );
	virtual void GetCurrentFilePath( PCHAR szPath );
	virtual int  GetClassCount( );
	virtual int  GetNodeCount( unsigned int classId );
	virtual int  CreateNewClass( PCHAR szName );
	virtual int  FindClassByName( PCHAR szName );
	virtual bool AddNode( int classId, NodeType type, PCHAR szName );

	__int16 major_version;
	__int16 minor_version;
};

extern "C" __declspec( dllexport ) void Init( ReclassSDK* pSDK )
{
	// Count classes
	unsigned int classes = pSDK->GetClassCount( );
	printf( "Class Count = %d\n", classes );

	// Get file name of .reclass file
	char szPath[256];
	pSDK->GetCurrentFilePath( szPath );
	printf( "Path = %s\n", szPath );

	// Create new file
	int classId = pSDK->CreateNewClass( "Test1234" );

	// Add nodes
	pSDK->AddNode( classId, nt_int64, "m_int64" );
	pSDK->AddNode( classId, nt_uint32, "m_int32" );
	pSDK->AddNode( classId, nt_float, "m_float" );

	// Get total nodes in class
	int nodes = pSDK->GetNodeCount( classId );
	printf( "Nodes in class %d\n", nodes );
};

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch ( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

