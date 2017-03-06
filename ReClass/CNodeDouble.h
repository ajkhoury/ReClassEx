#pragma once

#include "CNodeBase.h"

class CNodeDouble : public CNodeBase
{
public:
	CNodeDouble( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( void ) { return sizeof( double ); }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );
};