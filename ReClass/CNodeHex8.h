#pragma once

#include "CNodeBase.h"

class CNodeHex8 : public CNodeBase {
public:
    CNodeHex8( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( __int8 ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};