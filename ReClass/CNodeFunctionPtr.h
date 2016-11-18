#pragma once

#include "CNodeBase.h"

class CNodeFunctionPtr : public CNodeBase
{
public:
	CNodeFunctionPtr( );

	virtual void Update( HotSpot& Spot );

	virtual int GetMemorySize( ) { return sizeof( size_t ); }

	virtual int Draw( ViewInfo& View, int x, int y );

public:
	std::vector<CStringA> Assembly;
};