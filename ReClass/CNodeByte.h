#pragma once

#include "CNodeBase.h"

class CNodeByte : public CNodeBase {
public:
    CNodeByte( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( unsigned __int8 ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};