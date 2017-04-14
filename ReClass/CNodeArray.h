#pragma once

class CNodeArray : public CNodeBase
{
public:
	CNodeArray( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( );

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

	void SetTotal( DWORD total ) { m_dwTotal = total; }
	DWORD GetTotal( void ) { return m_dwTotal; }

	void SetClass( CNodeClass* pNode ) { m_pNode = pNode; }
	CNodeClass* GetClass( void ) { return m_pNode; }

protected:
	CNodeClass* m_pNode;
	DWORD m_dwTotal;
	int m_iCurrent;
};
