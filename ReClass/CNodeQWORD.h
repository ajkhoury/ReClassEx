#pragma once

#include "CNodeBase.h"

class CNodeQWORD : public CNodeBase
{
public:
	CNodeQWORD();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return sizeof(unsigned long long); }

	virtual int Draw(ViewInfo& View, int x, int y);
};