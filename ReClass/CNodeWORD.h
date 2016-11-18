#pragma once

#include "CNodeBase.h"

class CNodeWORD : public CNodeBase
{
public:
	CNodeWORD( );

	virtual void Update( HotSpot& Spot );

	virtual int GetMemorySize( ) { return sizeof( unsigned short ); }

	virtual int Draw( ViewInfo& View, int x, int y );
};