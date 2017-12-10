#include "stdafx.h"
#include "CNodePtrArray.h"

CNodePtrArray::CNodePtrArray( )
    : m_PtrCount( 1 )
    , m_CurrentIndex( 0 )
    , m_NodePtr( new CNodePtr )
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

    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "ArrayOfPointers " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
    tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), m_PtrCount );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

    tx = AddIcon( View, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "(" ) );
    tx = AddText( View, tx, y, g_crIndex, 1, _T( "%i" ), m_CurrentIndex );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( ")" ) );
    tx = AddIcon( View, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK );

    tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( "<%s* Size=%i>" ), m_NodePtr->GetClass( )->GetName( ), GetMemorySize( ) );
    tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X );

    tx += g_FontWidth;
    tx = AddComment( View, tx, y );

    y += g_FontHeight;
    if (m_LevelsOpen[View->Level])
    {
        if (IsMemory( View->Address + m_Offset + (sizeof( uintptr_t ) * m_CurrentIndex) ))
        {
            ClassSize = m_NodePtr->GetClass( )->GetMemorySize( );
            m_NodePtr->Memory( )->SetSize( ClassSize );

            VIEWINFO NewView;
            memcpy( &NewView, View, sizeof( NewView ) );
            NewView.Data = m_NodePtr->Memory( )->Data( );
            NewView.Address = *(uintptr_t*)(View->Data + m_Offset + (sizeof( uintptr_t ) * m_CurrentIndex));

            ReClassReadMemory( (LPVOID)NewView.Address, (LPVOID)NewView.Data, ClassSize );

            ChildDrawSize = m_NodePtr->GetClass( )->Draw( &NewView, x, y );

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
    return m_NodePtr->GetMemorySize( ) * m_PtrCount;
}

void CNodePtrArray::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );

    int v = _ttoi( Spot->Text.GetString( ) );
    if (v < 0) return;

    if (Spot->Id == 0)
    {
        if (v == 0)
            return;
        m_PtrCount = (size_t)v;
    }
    else if (Spot->Id == 1)
    {
        if (v >= (int)m_PtrCount) return;
        m_CurrentIndex = (int)v;
    }
    else if (Spot->Id == 2)
    {
        if (m_CurrentIndex > 0)
            m_CurrentIndex--;
    }
    else if (Spot->Id == 3)
    {
        if ((size_t)m_CurrentIndex < m_PtrCount - 1)
            m_CurrentIndex++;
    }
}

