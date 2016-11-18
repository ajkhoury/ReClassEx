#pragma once

#include "CNodeBase.h"

class CNodeClass : public CNodeBase
{
public:
	CNodeClass( );

	virtual void Update( HotSpot& Spot );

	virtual int GetMemorySize( );

	virtual int Draw( ViewInfo& View, int x, int y );

public:
	int idx;
	int RequestPosition;
	CString Code;
	class CChildFrame* pChildWindow;
};
