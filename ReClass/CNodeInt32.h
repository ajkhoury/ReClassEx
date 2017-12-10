#pragma once

#include "CNodeBase.h"

class CNodeInt32 : public CNodeBase
{
public:
	CNodeInt32( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( __int32 ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};