#pragma once

#include "CNodeBase.h"

class CNodeUnicode : public CNodeBase
{
public:
	CNodeUnicode();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize(void);

	virtual int Draw(ViewInfo& View, int x, int y);

public:
	DWORD memsize;
};