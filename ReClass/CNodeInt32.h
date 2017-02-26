#pragma once

#include "CNodeBase.h"

class CNodeInt32 : public CNodeBase
{
public:
	CNodeInt32( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( long ); }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );
};