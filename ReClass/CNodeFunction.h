#pragma once

#include "CNodeBase.h"
#include "CScintillaEdit.h"
#include "CChildView.h"

class CNodeFunction : public CNodeBase
{
public:
	CNodeFunction( );
	~CNodeFunction( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return m_dwMemorySize; }

	virtual int Draw( ViewInfo& View, int x, int y );

	void Initialize( CChildView* pChild, ULONG_PTR Address );

private:
	void DisassembleBytes( ULONG_PTR Address );

private:
	DWORD m_dwMemorySize;
	std::vector<CStringA> m_Assembly;

private:
	CScintillaEdit* m_pEdit;

	ULONG m_nLines;
	ULONG m_nLongestLine;

	int m_iWidth;
	int m_iHeight;

	BOOLEAN m_bRedrawNeeded;
};