#pragma once

#include "CNodeBase.h"

class CNodeFloat : public CNodeBase
{
public:
	CNodeFloat( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( float ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};