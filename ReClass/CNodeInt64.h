#pragma once

#include "CNodeBase.h"

class CNodeInt64 : public CNodeBase
{
public:
	CNodeInt64( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( __int64 ); }

	virtual int Draw( ViewInfo& View, int x, int y );
};