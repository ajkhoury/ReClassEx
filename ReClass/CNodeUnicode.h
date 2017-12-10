#pragma once

#include "CNodeBase.h"

class CNodeUnicode : public CNodeBase
{
public:
	CNodeUnicode( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( void ) { return m_dwMemorySize; }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

	inline void SetSize( ULONG size ) { m_dwMemorySize = size; }
	inline ULONG GetSize( void ) { return m_dwMemorySize; }

private:
    ULONG m_dwMemorySize;
};