#pragma once

#include "CNodeBase.h"

class CNodeHex8 : public CNodeBase
{
public:
	CNodeHex8();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return 1; }

	virtual int Draw(ViewInfo& View, int x, int y);
};