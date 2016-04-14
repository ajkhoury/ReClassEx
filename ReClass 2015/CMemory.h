#pragma once

class CMemory
{
public:
	CMemory()
	{
		MemorySize = 0;
		pMemory = 0;
	}

	~CMemory()
	{
		if (pMemory)
			delete pMemory;
	}

	void SetSize(unsigned long Size)
	{
		if ((!pMemory) || (Size != MemorySize))
		{
			if (pMemory)
				delete pMemory;
			pMemory = new unsigned char[Size];
			MemorySize = Size;
		}
	}

	unsigned long MemorySize;
	unsigned char* pMemory;
};
