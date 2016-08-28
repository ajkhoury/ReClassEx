#pragma once

#include "CNodeBase.h"

class CNodeVec2 : public CNodeBase
{
public:
	CNodeVec2();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(float) * 2; }

	virtual int Draw(ViewInfo& View, int x, int y);
};