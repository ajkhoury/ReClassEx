#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodePtr : public CNodeBase
{
public:
	CNodePtr( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

	void SetClass( CNodeClass* pNode ) { m_pNode = pNode; }
	CNodeClass* GetClass( void ) { return m_pNode; }

private:
	CNodeClass* m_pNode;
	CMemory m_Memory;
};