#pragma once

class CMemory {
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
            free( m_pMemory );
            m_pMemory = NULL;
        }
    }

    inline void SetSize( unsigned long Size )
    {
        if (m_pMemory == NULL || (Size != m_ulSize))
        {
            if (m_pMemory != NULL)
                free( m_pMemory );
            m_pMemory = (unsigned char *)malloc( Size );
            m_ulSize = Size;
        }
    }

    inline unsigned char* Data( ) { return m_pMemory; }
    inline unsigned long DataSize( ) const { return m_ulSize; }

private:
    unsigned char* m_pMemory;
    unsigned long m_ulSize;
};
