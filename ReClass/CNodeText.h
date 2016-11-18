#pragma once

#include "CNodeBase.h"

class CNodeText : public CNodeBase
{
public:
	CNodeText( );

	virtual void Update( HotSpot& Spot );

	virtual int GetMemorySize( );

	virtual int Draw( ViewInfo& View, int x, int y );

public:
	DWORD memsize;
};