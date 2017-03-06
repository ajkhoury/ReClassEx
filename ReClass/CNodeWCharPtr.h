#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeWCharPtr : public CNodeBase
{
public:
	CNodeWCharPtr( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

public:
	CNodeBase* pNode;
	CMemory Memory;
};