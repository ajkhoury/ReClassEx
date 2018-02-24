#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeWCharPtr : public CNodeBase {
public:
    CNodeWCharPtr( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

public:
    CNodeBase* pNode;
    CMemory Memory;
};