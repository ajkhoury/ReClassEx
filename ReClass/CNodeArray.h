#pragma once

class CNodeArray : public CNodeBase
{
public:
	CNodeArray( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( );

	virtual int Draw( ViewInfo& View, int x, int y );

	void SetTotal( DWORD total ) { m_dwTotal = total; }
	DWORD GetTotal( void ) { return m_dwTotal; }

	void SetClass( CNodeClass* pNode ) { m_pNode = pNode; }
	CNodeClass* GetClass( void ) { return m_pNode; }

private:
	CNodeClass* m_pNode;
	DWORD m_dwTotal;
	int m_iCurrent;
};