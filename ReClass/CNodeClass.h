#pragma once

#include "CNodeBase.h"

class CNodeClass : public CNodeBase
{
public:
	CNodeClass( );

	virtual void Update( const HotSpot& Spot );

	virtual ULONG GetMemorySize( );

	virtual NodeSize Draw( const ViewInfo& View, int x, int y );

	VOID SetChildFrame( class CClassFrame* pChild ) { pChildWindow = pChild; }
	class CClassFrame* GetChildFrame( ) { return pChildWindow; }

public:
	size_t Idx;
	size_t RequestPosition;
	CString Code;
	class CClassFrame* pChildWindow;
};
