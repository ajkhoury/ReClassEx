#pragma once

class CMemory
{
public:
	CMemory();

	~CMemory();

	void SetSize(unsigned long Size);

public:
	unsigned long MemorySize;
	unsigned char* pMemory;
};
