#pragma once

#include "CNodeBase.h"

class CNodeCustom : public CNodeBase {
public:
    CNodeCustom( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( );

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

    inline void SetSize( ULONG size ) { m_ulMemorySize = size; }
    inline ULONG GetSize( void ) { return m_ulMemorySize; }

private:
    ULONG m_ulMemorySize;
};