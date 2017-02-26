#pragma once

#include "CNodeBase.h"

class CNodeCustom : public CNodeBase
{
public:
	CNodeCustom( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( );

	virtual NodeSize Draw( ViewInfo& View, int x, int y );

	void SetSize( DWORD size ) { m_dwMemorySize = size; }
	DWORD GetSize( void ) { return m_dwMemorySize; }

private:
	DWORD m_dwMemorySize;
};