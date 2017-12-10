#pragma once

#include "CNodeBase.h"

class CNodeText : public CNodeBase
{
public:
	CNodeText( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return m_dwMemorySize; }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

    inline void SetSize( DWORD size ) { m_dwMemorySize = size; }
	inline DWORD GetSize( void ) { return m_dwMemorySize; }

private:
	DWORD m_dwMemorySize;
};