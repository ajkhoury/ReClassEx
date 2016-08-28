#pragma once

class CNodeArray : public CNodeBase
{
public:
	CNodeArray();

	virtual void Update(CHotSpot& Spot);

	virtual int GetMemorySize();

	virtual int Draw(ViewInfo& View, int x, int y);

public:
	CNodeBase* pNode;
	DWORD Total;
	DWORD Current;
};