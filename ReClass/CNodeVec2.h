#pragma once

#include "CNodeBase.h"

class CNodeVec2 : public CNodeBase {
public:
    CNodeVec2( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( float ) * 2; }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};