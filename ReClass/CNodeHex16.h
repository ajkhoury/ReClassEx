#pragma once

#include "CNodeBase.h"

class CNodeHex16 : public CNodeBase
{
public:
	CNodeHex16( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return 2; }

	virtual int Draw( ViewInfo& View, int x, int y );
};