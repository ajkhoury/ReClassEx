#pragma once

#include "CNodeBase.h"

class CNodeVec3 : public CNodeBase
{
public:
	CNodeVec3( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( float ) * 3; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};