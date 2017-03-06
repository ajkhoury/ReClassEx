#pragma once

#include "CNodeBase.h"

class CNodeDWORD : public CNodeBase
{
public:
	CNodeDWORD( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( unsigned long ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};