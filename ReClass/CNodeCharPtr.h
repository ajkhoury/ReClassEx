#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeCharPtr : public CNodeBase
{
public:
	CNodeCharPtr( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );

public:
	CNodeBase* pNode;
	CMemory Memory;
};