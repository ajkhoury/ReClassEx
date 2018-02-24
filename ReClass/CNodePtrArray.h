#pragma once

class CNodePtrArray : public CNodeBase
{
public:
	CNodePtrArray( );

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

	virtual ULONG GetMemorySize( );

	virtual void Update( const PHOTSPOT Spot );

	inline ULONG Count( void ) { return m_ulPtrCount; }
    inline void SetCount( ULONG Count ) { m_ulPtrCount = Count; }

	void SetClass( CNodeClass* pNode ) { m_pNodePtr->SetClass( pNode ); }
	CNodeClass* GetClass( void ) { return m_pNodePtr->GetClass( ); }

protected:
	CNodePtr* m_pNodePtr;
    ULONG m_ulPtrCount;
	INT m_iCurrentIndex;
};

