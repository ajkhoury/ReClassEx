#pragma once

class CNodeMatrix : public CNodeBase
{
public:
	CNodeMatrix();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize() { return 4 * 4 * sizeof(float); }

	virtual int Draw(ViewInfo& View, int x, int y);
};