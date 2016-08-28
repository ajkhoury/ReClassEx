#include "stdafx.h"

CMemory::CMemory() : 
	MemorySize(0),
	pMemory(0)
{
}

CMemory::~CMemory()
{
	if (pMemory) {
		delete[] pMemory;
	}
}

void CMemory::SetSize(unsigned long Size)
{
	if ((!pMemory) || (Size != MemorySize))
	{
		if (pMemory)
			delete[] pMemory;
		pMemory = new unsigned char[Size];
		MemorySize = Size;
	}
}

