#pragma once

#include "CNodeBase.h"

class CNodeQuat : public CNodeBase
{
public:
	CNodeQuat( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( float ) * 4; }

	virtual int Draw( ViewInfo& View, int x, int y );
};