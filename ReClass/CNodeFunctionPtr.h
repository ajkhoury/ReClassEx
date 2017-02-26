#pragma once

#include "CNodeBase.h"
#include "CScintillaEdit.h"
#include "CChildView.h"

class CNodeFunctionPtr : public CNodeBase
{
public:
	CNodeFunctionPtr( );
	~CNodeFunctionPtr( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );

	void Initialize( CChildView* pChild, ULONG_PTR Address );

private:
	void DisassembleBytes( ULONG_PTR Address );

public:
	std::vector<CStringA> m_Assembly;

private:
	CScintillaEdit* m_pEdit;

	ULONG m_nLines;
	ULONG m_nLongestLine;

	int m_iWidth;
	int m_iHeight;

	BOOLEAN m_bRedrawNeeded;
};