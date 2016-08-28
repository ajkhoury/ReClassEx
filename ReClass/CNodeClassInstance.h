#pragma once

#include "CNodeBase.h"

class CNodeClassInstance : public CNodeBase
{
public:
	CNodeClassInstance();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize();

	virtual int Draw(ViewInfo& View, int x, int y);

public:
	CNodeClass* pNode;
};