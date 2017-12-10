#pragma once

#include "CNodeBase.h"

class CNodeDword : public CNodeBase {
public:
	CNodeDword( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( unsigned __int32 ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};