#pragma once

#include "CNodeBase.h"

class CNodeInt64 : public CNodeBase
{
public:
	CNodeInt64( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( __int64 ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};