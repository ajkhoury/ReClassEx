#pragma once

#include "CNodeBase.h"

class CNodeVec3 : public CNodeBase
{
public:
	CNodeVec3();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(float) * 3; }

	virtual int Draw(ViewInfo& View, int x, int y);
};