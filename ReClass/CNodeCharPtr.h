#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeCharPtr : public CNodeBase
{
public:
	CNodeCharPtr( );

	virtual void Update( HotSpot& Spot );

	virtual int GetMemorySize( ) { return sizeof( size_t ); }

	virtual int Draw( ViewInfo& View, int x, int y );

public:
	CNodeBase* pNode;
	CMemory Memory;
};