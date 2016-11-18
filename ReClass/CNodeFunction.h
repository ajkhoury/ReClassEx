#pragma once

#include "CNodeBase.h"

class CNodeFunction : public CNodeBase
{
public:
	CNodeFunction( );

	virtual void Update( HotSpot& Spot );

	virtual int GetMemorySize( ) { return memsize; }

	virtual int Draw( ViewInfo& View, int x, int y );

	void DisassembleBytes( size_t Address );

public:
	DWORD memsize;

	std::vector<CStringA> Assembly;
};