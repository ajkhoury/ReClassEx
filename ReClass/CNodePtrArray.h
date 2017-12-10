#pragma once

class CNodePtrArray : public CNodeBase
{
public:
	CNodePtrArray( );

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

	virtual ULONG GetMemorySize( );

	virtual void Update( const PHOTSPOT Spot );

	size_t& Count( ) { return m_PtrCount; }

	void SetClass( CNodeClass* pNode ) { m_NodePtr->SetClass( pNode ); }
	CNodeClass* GetClass( void ) { return m_NodePtr->GetClass( ); }

protected:
	CNodePtr* m_NodePtr;
	size_t m_PtrCount;
	unsigned m_CurrentIndex = 0;
};

