#pragma once

#include "CNodeBase.h"

class CNodeHex16 : public CNodeBase {
public:
	CNodeHex16( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( __int16 ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};