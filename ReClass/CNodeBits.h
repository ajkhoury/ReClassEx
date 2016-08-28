#pragma once

#include "CNodeBase.h"

class CNodeBits : public CNodeBase
{
public:
	CNodeBits();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return 1; }

	virtual int Draw(ViewInfo& View, int x, int y);
};