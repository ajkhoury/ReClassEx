#pragma once

#include "CNodeBase.h"

class CNodeDouble : public CNodeBase
{
public:
	CNodeDouble();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize(void) { return sizeof(double); }

	virtual int Draw(ViewInfo& View, int x, int y);
};