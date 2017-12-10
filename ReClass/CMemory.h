#pragma once

class CMemory
{
public:
	CMemory( ) 
        : m_ulSize( 0 )
        , m_pMemory( NULL )
	{
	}

	~CMemory( )
	{
		if (m_pMemory)
		{
			delete[] m_pMemory;
		}
	}

	void SetSize( unsigned long Size )
	{
		if (m_pMemory == NULL || (Size != m_ulSize))
		{
			if (m_pMemory)
            {
                delete[] m_pMemory;
            }
				
			m_pMemory = new unsigned char[Size];
			m_ulSize = Size;
		}
	}

	unsigned char* Data( )
	{
		return m_pMemory;
	}

	unsigned long DataSize( )
	{
		return m_ulSize;
	}

private:
	unsigned char* m_pMemory;
	unsigned long m_ulSize;
};
