#pragma once

#include "CNodeBase.h"

class CNodeInt64 : public CNodeBase
{
public:
	CNodeInt64( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( __int64 ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};