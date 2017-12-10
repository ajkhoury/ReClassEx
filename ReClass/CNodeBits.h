#pragma once

#include "CNodeBase.h"

class CNodeBits : public CNodeBase
{
public:
	CNodeBits( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return 1; }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};