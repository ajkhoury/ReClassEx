#pragma once

#include "CNodeBase.h"

class CNodeInt8 : public CNodeBase
{
public:
	CNodeInt8( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( char ); }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );
};