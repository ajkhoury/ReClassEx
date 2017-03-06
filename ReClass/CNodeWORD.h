#pragma once

#include "CNodeBase.h"

class CNodeWORD : public CNodeBase
{
public:
	CNodeWORD( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( unsigned short ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};