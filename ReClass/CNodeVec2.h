#pragma once

#include "CNodeBase.h"

class CNodeVec2 : public CNodeBase
{
public:
	CNodeVec2( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( float ) * 2; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};