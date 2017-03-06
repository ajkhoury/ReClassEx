#pragma once

#include "CNodeBase.h"

class CNodeByte : public CNodeBase
{
public:
	CNodeByte( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( unsigned char ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};