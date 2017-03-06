#pragma once

#include "CNodeBase.h"

class CNodeHex16 : public CNodeBase
{
public:
	CNodeHex16( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return 2; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};