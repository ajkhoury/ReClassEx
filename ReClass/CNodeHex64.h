#pragma once

#include "CNodeBase.h"

class CNodeHex64 : public CNodeBase {
public:
	CNodeHex64( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( __int64 ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};
