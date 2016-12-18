#pragma once

#include "CNodeBase.h"

class CNodeHex32 : public CNodeBase
{
public:
	CNodeHex32( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return 4; }

	virtual int Draw( ViewInfo& View, int x, int y );
};