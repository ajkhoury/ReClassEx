#pragma once

#include "CNodeBase.h"

class CNodeHex64 : public CNodeBase
{
public:
	CNodeHex64( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return 8; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};
