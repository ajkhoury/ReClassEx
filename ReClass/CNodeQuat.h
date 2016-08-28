#pragma once

#include "CNodeBase.h"

class CNodeQuat : public CNodeBase
{
public:
	CNodeQuat();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(float) * 4; }

	virtual int Draw(ViewInfo& View, int x, int y);
};