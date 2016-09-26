#pragma once

#include "CNodeBase.h"

class CNodeFunction : public CNodeBase
{
public:
	CNodeFunction( );

	virtual void Update( CHotSpot& Spot );

	virtual int GetMemorySize( ) { return memsize; }

	virtual int Draw( ViewInfo& View, int x, int y );

public:
	DWORD memsize;

	std::vector<CStringA> Assembly;
};