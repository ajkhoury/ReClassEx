#pragma once

#include "CNodeBase.h"

class CNodeHex32 : public CNodeBase
{
public:
	CNodeHex32( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return 4; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};