#include "stdafx.h"
#include "CNodeClass.h"
#include "CClassFrame.h"

CNodeClass::CNodeClass( )
{
    TCHAR szOffset[128];

    m_nodeType = nt_class;
    m_Offset = GetBaseAddress( );

    #if defined(_M_AMD64)
    _ui64tot_s( m_Offset, szOffset, 128, 16 );
    #else
    _ultot_s( m_Offset, szOffset, 128, 16 );
    #endif
    m_strOffset.SetString( szOffset );

    m_RequestPosition = -1;
    m_Idx = 0;
    m_pChildClassFrame = nullptr;
}

void CNodeClass::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );
    if (Spot->Id == 0)
    {
        m_strOffset.SetString( Spot->Text.GetString( ) );
        m_Offset = ConvertStrToAddress( m_strOffset );
    }
    else if (Spot->Id == 1)
    {
        m_RequestPosition = _tcstol( Spot->Text.GetString( ), NULL, 10 ); // m_RequestPosition = ConvertStrToAddress( Spot->Text );
    }
}

ULONG CNodeClass::GetMemorySize( )
{
    int size = 0;
    for (UINT i = 0; i < m_ChildNodes.size( ); i++)
        size += m_ChildNodes[i]->GetMemorySize( );
    return size;
}

NODESIZE CNodeClass::Draw( const PVIEWINFO View, int x, int y )
{
    NODESIZE DrawSize;
    NODESIZE ChildDrawSize;

    DrawSize.x = 0;
    DrawSize.y = 0;

    AddSelection( View, x, y, g_FontHeight );
    x = AddOpenClose( View, x, y );

    // Save tx here
    int tx = x;

    x = AddIcon( View, x, y, ICON_CLASS, -1, -1 );
    x = AddText( View, x, y, g_crOffset, 0, _T( "%s" ), m_strOffset ) + g_FontWidth;

    // x += ( NumDigits( m_Offset ) ) * FontWidth;
    // TODO, figure this out better
    // x += ( ( NumDigits( m_Offset ) - 7 ) * FontWidth ) / 2;
    // printf( "Print %s at %d\n", m_strOffset, x );

    x = AddText( View, x, y, g_crIndex, HS_NONE, _T( "(" ) );
    x = AddText( View, x, y, g_crIndex, HS_OPENCLOSE, _T( "%i" ), m_Idx );
    x = AddText( View, x, y, g_crIndex, HS_NONE, _T( ")" ) );

    x = AddText( View, x, y, g_crType, HS_NONE, _T( "Class " ) );
    x = AddText( View, x, y, g_crName, HS_NAME, m_strName ) + g_FontWidth;
    x = AddText( View, x, y, g_crValue, HS_NONE, _T( "[%i]" ), GetMemorySize( ) ) + g_FontWidth;
    x = AddComment( View, x, y );

    DrawSize.x = x;
    y += g_FontHeight;
    if (m_LevelsOpen[View->Level])
    {
        VIEWINFO ViewInfo;
        memcpy( &ViewInfo, View, sizeof( ViewInfo ) );
        ViewInfo.Level++;

        for (UINT i = 0; i < m_ChildNodes.size( ); i++)
        {
            CNodeBase* pNode = m_ChildNodes[i];
            if (!pNode)
            {
                PrintOut( _T( "Node %d is NULL in class %s!" ), i, m_strName.GetString( ) );
                continue;
            }          
                      
            if (pNode->GetType( ) == nt_vtable)
            {
                CNodeVTable* VTableNode = static_cast<CNodeVTable*>(pNode);
                if (!VTableNode->IsInitialized( ) && m_pChildClassFrame != nullptr)
                    VTableNode->Initialize( static_cast<CWnd*>(m_pChildClassFrame->GetChildView( )) );
            }
            
            if (pNode->GetType( ) == nt_function)
            {
                CNodeFunction* FunctionNode = static_cast<CNodeFunction*>(pNode);
                if (!FunctionNode->IsInitialized( ) && m_pChildClassFrame != nullptr)
                    FunctionNode->Initialize( m_pChildClassFrame->GetChildView( ), m_Offset + FunctionNode->GetOffset( ) );
            }
            
            ChildDrawSize = pNode->Draw( &ViewInfo, tx, y );
            
            y = ChildDrawSize.y;
            if (ChildDrawSize.x > DrawSize.x)
            {
                DrawSize.x = ChildDrawSize.x;
            }
        }
    }

    DrawSize.y = y;
    return DrawSize;
}
