#pragma once

#include "CNodeBase.h"
#include "CMemory.h"

class CNodePtr : public CNodeBase
{
public:
	CNodePtr();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(size_t); }

	virtual int Draw(ViewInfo& View, int x, int y);

public:
	CNodeBase* pNode;
	CMemory Memory;
};