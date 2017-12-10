#pragma once

#include "CNodeBase.h"

class CNodeQword : public CNodeBase {
public:
    CNodeQword( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( unsigned __int64 ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};