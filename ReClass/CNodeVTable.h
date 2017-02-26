#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeVTable : public CNodeBase
{
public:
	CNodeVTable( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );

private:
	CMemory m_Memory;
};
