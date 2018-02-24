#pragma once

#include "CNodeBase.h"

class CNodeQuat : public CNodeBase {
public:
    CNodeQuat( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( float ) * 4; }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};