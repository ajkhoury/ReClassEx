#pragma once

#include "CNodeBase.h"

class CNodeInt32 : public CNodeBase
{
public:
	CNodeInt32();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(long); }

	virtual int Draw(ViewInfo& View, int x, int y);
};