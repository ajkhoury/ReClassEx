#pragma once

#include "CNodeBase.h"

class CNodeWord : public CNodeBase {
public:
    CNodeWord( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( WORD ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};