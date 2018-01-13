#pragma once

class CNodePtrArray : public CNodeBase
{
public:
	CNodePtrArray( );

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

	virtual ULONG GetMemorySize( );

	virtual void Update( const PHOTSPOT Spot );

	inline size_t Count( void ) { return m_PtrCount; }
    inline void SetCount( size_t Count ) { m_PtrCount = Count; }

	void SetClass( CNodeClass* pNode ) { m_NodePtr->SetClass( pNode ); }
	CNodeClass* GetClass( void ) { return m_NodePtr->GetClass( ); }

protected:
	CNodePtr* m_NodePtr;
	size_t m_PtrCount;
	unsigned m_CurrentIndex = 0;
};

