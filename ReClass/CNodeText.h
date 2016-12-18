#pragma once

#include "CNodeBase.h"

class CNodeText : public CNodeBase
{
public:
	CNodeText( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return m_dwMemorySize; }

	virtual int Draw( ViewInfo& View, int x, int y );

	void SetSize( DWORD size ) { m_dwMemorySize = size; }
	DWORD GetSize( void ) { return m_dwMemorySize; }

private:
	DWORD m_dwMemorySize;
};