#pragma once

#include "CNodeBase.h"

class CNodeVec2 : public CNodeBase
{
public:
	CNodeVec2( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( float ) * 2; }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );
};