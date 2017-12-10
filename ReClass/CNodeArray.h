#pragma once

class CNodeArray : public CNodeBase {
public:
	CNodeArray( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( );

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

	inline void SetTotal( ULONG total ) { m_ulTotal = total; }
	inline ULONG GetTotal( void ) { return m_ulTotal; }

	inline void SetClass( CNodeClass* pNode ) { m_pNode = pNode; }
	inline CNodeClass* GetClass( void ) { return m_pNode; }

protected:
	CNodeClass* m_pNode;
	ULONG m_ulTotal;
	int m_iCurrent;
};
