#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodeVTable : public CNodeBase
{
public:
	CNodeVTable();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(size_t); }

	virtual int Draw(ViewInfo& View, int x, int y);

public:
	CMemory Memory;
};
