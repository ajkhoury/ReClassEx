#pragma once

#include "CNodeBase.h"

class CNodeDouble : public CNodeBase {
public:
    CNodeDouble( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( void ) { return sizeof( double ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};