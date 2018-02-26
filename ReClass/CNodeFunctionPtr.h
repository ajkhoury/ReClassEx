#pragma once

#include "CNodeBase.h"
#include "CScintillaEdit.h"

class CNodeFunctionPtr : public CNodeBase {
public:
    CNodeFunctionPtr( );
    CNodeFunctionPtr( CWnd* pParentWindow, ULONG_PTR Address );
    ~CNodeFunctionPtr( );

    virtual void Update( const PHOTSPOT Spot );

    virtual ULONG GetMemorySize( ) { return sizeof( void* ); }

    virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

    void Initialize( CWnd* pParentWindow, ULONG_PTR Address );

    inline bool IsInitialized( ) { return (m_pAssemblyWindow != NULL); }
    
    inline void HideAssemblyWindow( )
    {
        if (!m_bRedrawNeeded)
        {
            if (m_pAssemblyWindow != NULL)
                m_pAssemblyWindow->ShowWindow( SW_HIDE );
            m_bRedrawNeeded = TRUE;
        }

        //if (m_pAssemblyWindow != NULL)
        //{
        //    m_pAssemblyWindow->ShowWindow( SW_HIDE );
        //    m_bRedrawNeeded = TRUE;
        //}
    }

private:
    void DisassembleBytes( ULONG_PTR Address );

public:
    std::vector<CStringA> m_Assembly;

private:
    CScintillaEdit* m_pAssemblyWindow;
    CWnd* m_pParentWindow;

    ULONG m_nLines;
    ULONG m_nLongestLine;

    int m_iWidth;
    int m_iHeight;

    BOOLEAN m_bRedrawNeeded;
};