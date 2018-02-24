#pragma once

#include "CNodeBase.h"

class CNodeInt16 : public CNodeBase {
public:
    CNodeInt16( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( __int16 ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};