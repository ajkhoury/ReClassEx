#pragma once

#include "CNodeBase.h"

class CNodeFloat : public CNodeBase {
public:
    CNodeFloat( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( float ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};