#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeCharPtr : public CNodeBase
{
public:
	CNodeCharPtr( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

public:
	CNodeBase* m_pNode;
	CMemory m_Memory;
};