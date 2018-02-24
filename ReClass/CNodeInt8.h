#pragma once

#include "CNodeBase.h"

class CNodeInt8 : public CNodeBase {
public:
    CNodeInt8( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( __int8 ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};