#pragma once

#include "CNodeBase.h"

class CNodeHex32 : public CNodeBase {
public:
	CNodeHex32( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( __int32 ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};