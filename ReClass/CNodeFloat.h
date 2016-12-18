#pragma once

#include "CNodeBase.h"

class CNodeFloat : public CNodeBase
{
public:
	CNodeFloat( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( float ); }

	virtual int Draw( ViewInfo& View, int x, int y );
};