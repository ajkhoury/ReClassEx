#pragma once

#include "CNodeBase.h"

class CNodeBits : public CNodeBase
{
public:
	CNodeBits( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return 1; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};