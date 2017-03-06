#pragma once

#include "CNodeBase.h"

class CNodeUnicode : public CNodeBase
{
public:
	CNodeUnicode( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( void ) { return m_dwMemorySize; }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

	void SetSize( DWORD size ) { m_dwMemorySize = size; }
	DWORD GetSize( void ) { return m_dwMemorySize; }

private:
	DWORD m_dwMemorySize;
};