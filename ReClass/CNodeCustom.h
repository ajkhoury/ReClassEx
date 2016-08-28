#pragma once

#include "CNodeBase.h"

class CNodeCustom : public CNodeBase
{
public:
	CNodeCustom();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize();

	virtual int Draw(ViewInfo& View, int x, int y);

public:
	DWORD memsize;
};