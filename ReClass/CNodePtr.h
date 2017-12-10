#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodePtr : public CNodeBase
{
public:
	CNodePtr( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

	inline void SetClass( CNodeClass* pNode ) { m_pNode = pNode; }
	inline CNodeClass* GetClass( void ) { return m_pNode; }

	inline CMemory* Memory( ) { return &m_Memory; }

private:
	CNodeClass* m_pNode;
	CMemory m_Memory;
};