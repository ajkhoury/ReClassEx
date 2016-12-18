#pragma once

#include "CNodeBase.h"

class CNodeByte : public CNodeBase
{
public:
	CNodeByte( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( unsigned char ); }

	virtual int Draw( ViewInfo& View, int x, int y );
};