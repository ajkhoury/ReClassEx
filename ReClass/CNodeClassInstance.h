#pragma once

#include "CNodeBase.h"

class CNodeClassInstance : public CNodeBase
{
public:
	CNodeClassInstance( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( );

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

	void SetClass( CNodeClass* pNode ) { m_pNode = pNode; }
	CNodeClass* GetClass( void ) { return m_pNode; }

private:
	CNodeClass* m_pNode;
};