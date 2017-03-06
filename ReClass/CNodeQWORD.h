#pragma once

#include "CNodeBase.h"

class CNodeQWORD : public CNodeBase
{
public:
	CNodeQWORD( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( unsigned long long ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};