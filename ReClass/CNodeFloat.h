#pragma once

#include "CNodeBase.h"

class CNodeFloat : public CNodeBase
{
public:
	CNodeFloat();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(float); }

	virtual int Draw(ViewInfo& View, int x, int y);
};