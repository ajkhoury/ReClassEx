#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodePtr : public CNodeBase {
public:
    CNodePtr( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

    inline void SetClass( CNodeClass* pClassNode ) { m_pClassNode = pClassNode; }
    inline CNodeClass* GetClass( void ) { return m_pClassNode; }

    inline CMemory* Memory( ) { return &m_Memory; }

private:
    CNodeClass* m_pClassNode;
    CMemory m_Memory;
};