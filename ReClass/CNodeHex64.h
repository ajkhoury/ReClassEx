#pragma once

#include "CNodeBase.h"

class CNodeHex64 : public CNodeBase
{
public:
	CNodeHex64();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return 8; }

	virtual int Draw(ViewInfo& View, int x, int y);
};
