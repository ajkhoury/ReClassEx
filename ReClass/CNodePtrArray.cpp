#include "stdafx.h"
#include "CNodePtrArray.h"

CNodePtrArray::CNodePtrArray( )
    : m_ulPtrCount( 1 )
    , m_iCurrentIndex( 0 )
    , m_pNodePtr( new CNodePtr )
{
    m_nodeType = nt_ptrarray;
}

NODESIZE CNodePtrArray::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    NODESIZE ChildDrawSize;
    ULONG ClassSize;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    DrawSize.x = 0;
    DrawSize.y = 0;

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );

    x = AddOpenClose( View, x, y );
    x = AddIcon( View, x, y, ICON_ARRAY, -1, -1 );

    tx = x;
    tx = AddAddressOffset( View, tx, y );

    tx = AddText( View, tx, y, g_clrType, HS_NONE, _T( "ArrayOfPointers " ) );
    tx = AddText( View, tx, y, g_clrName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_clrIndex, HS_NONE, _T( "[" ) );
    tx = AddText( View, tx, y, g_clrIndex, HS_EDIT, _T( "%u" ), m_ulPtrCount );
    tx = AddText( View, tx, y, g_clrIndex, HS_NONE, _T( "]" ) );

    tx = AddIcon( View, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK );
    tx = AddText( View, tx, y, g_clrIndex, HS_NONE, _T( "(" ) );
    tx = AddText( View, tx, y, g_clrIndex, 1, _T( "%i" ), m_iCurrentIndex );
    tx = AddText( View, tx, y, g_clrIndex, HS_NONE, _T( ")" ) );
    tx = AddIcon( View, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK );

    tx = AddText( View, tx, y, g_clrValue, HS_NONE, _T( "<%s* Size=%u>" ), m_pNodePtr->GetClass( )->GetName( ), GetMemorySize( ) );
    tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X );

    tx += g_FontWidth;
    tx = AddComment( View, tx, y );

    y += g_FontHeight;
    if (m_LevelsOpen[View->Level])
    {
        if (IsMemory( View->Address + m_Offset + (sizeof( ULONG_PTR ) * m_iCurrentIndex) ))
        {
            ClassSize = m_pNodePtr->GetClass( )->GetMemorySize( );
            m_pNodePtr->Memory( )->SetSize( ClassSize );

            VIEWINFO NewView;
            memcpy( &NewView, View, sizeof( NewView ) );
            NewView.Data = m_pNodePtr->Memory( )->Data( );
            NewView.Address = *(ULONG_PTR*)(View->Data + m_Offset + (sizeof( ULONG_PTR ) * m_iCurrentIndex));

            ReClassReadMemory( (LPVOID)NewView.Address, (LPVOID)NewView.Data, ClassSize );

            ChildDrawSize = m_pNodePtr->GetClass( )->Draw( &NewView, x, y );

            y = ChildDrawSize.y;
            if (ChildDrawSize.x > DrawSize.x)
                DrawSize.x = ChildDrawSize.x;
        }
    }

    DrawSize.y = y;
    return DrawSize;
}

ULONG CNodePtrArray::GetMemorySize( )
{
    return m_pNodePtr->GetMemorySize( ) * m_ulPtrCount;
}

void CNodePtrArray::Update( const PHOTSPOT Spot )
{
    LONG LongValue;
    ULONG UlongValue;

    StandardUpdate( Spot );

    LongValue = _ttol( Spot->Text.GetString( ) );
    if (LongValue < 0)
        return;

    UlongValue = (ULONG)LongValue;

    if (Spot->Id == 0)
    {
        if (UlongValue == 0)
            return;
        m_ulPtrCount = UlongValue;
    }
    else if (Spot->Id == 1)
    {
        if (UlongValue >= m_ulPtrCount) 
            return;
        m_iCurrentIndex = UlongValue;
    }
    else if (Spot->Id == 2)
    {
        if (m_iCurrentIndex > 0)
            m_iCurrentIndex--;
    }
    else if (Spot->Id == 3)
    {
        if (m_iCurrentIndex < (INT)m_ulPtrCount - 1)
            m_iCurrentIndex++;
    }
}

