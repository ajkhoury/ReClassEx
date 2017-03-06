#pragma once

#include "CNodeBase.h"

class CNodeQuat : public CNodeBase
{
public:
	CNodeQuat( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( float ) * 4; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};