#pragma once

#include "CNodeBase.h"

class CNodeVec3 : public CNodeBase {
public:
    CNodeVec3( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( float ) * 3; }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};