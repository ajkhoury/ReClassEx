#pragma once

#include "CNodeBase.h"

class CNodeDouble : public CNodeBase
{
public:
	CNodeDouble( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( void ) { return sizeof( double ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};