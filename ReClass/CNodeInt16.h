#pragma once

#include "CNodeBase.h"

class CNodeInt16 : public CNodeBase
{
public:
	CNodeInt16( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( short ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};