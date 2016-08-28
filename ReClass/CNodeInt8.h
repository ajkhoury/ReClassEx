#pragma once

#include "CNodeBase.h"

class CNodeInt8 : public CNodeBase
{
public:
	CNodeInt8();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(char); }

	virtual int Draw(ViewInfo& View, int x, int y);
};