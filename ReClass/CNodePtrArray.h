#pragma once

class CNodePtrArray : public CNodeBase
{
public:
	CNodePtrArray( );

	virtual NodeSize Draw( const ViewInfo & View, int x, int y );
	virtual ULONG GetMemorySize( );
	virtual void Update( const HotSpot & Spot );

	size_t& Count( ) { return m_PtrCount; }

	void SetClass( CNodeClass* pNode ) { m_NodePtr->SetClass( pNode ); }
	CNodeClass* GetClass( void ) { return m_NodePtr->GetClass( ); }

protected:
	CNodePtr* m_NodePtr;
	size_t m_PtrCount;
	unsigned m_CurrentIndex = 0;
};

