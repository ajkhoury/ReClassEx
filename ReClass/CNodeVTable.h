#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeVTable : public CNodeBase
{
public:
	CNodeVTable( );
	CNodeVTable( CWnd* pParentWindow );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

	inline bool IsInitialized( ) { return (m_pParentWindow != NULL); }

	void Initialize( CWnd* pParentWindow );

private:
	CMemory m_Memory;
	CWnd* m_pParentWindow;
};
