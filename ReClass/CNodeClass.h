#pragma once

#include "CNodeBase.h"

class CNodeClass : public CNodeBase
{
public:
	CNodeClass( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( );

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

	VOID SetChildFrame( class CChildFrame* pChild ) { pChildWindow = pChild; }
	class CChildFrame* GetChildFrame( ) { return pChildWindow; }

public:
	int idx;
	int RequestPosition;
	CString Code;
	class CChildFrame* pChildWindow;
};
