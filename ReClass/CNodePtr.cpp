#include "stdafx.h"
#include "CNodePtr.h"

CNodePtr::CNodePtr( ) : m_pNode( NULL )
{
    m_nodeType = nt_pointer;
}

void CNodePtr::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );
}

NODESIZE CNodePtr::Draw( const PVIEWINFO View, int x, int y )
{
    NODESIZE DrawSize;
    NODESIZE ChildDrawSize;
    uintptr_t* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );
    
    Data = (uintptr_t*)(View->Data + m_Offset);

    //printf( "read ptr: %p\n", View->Data );
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );

    x = AddOpenClose( View, x, y );
    x = AddIcon( View, x, y, ICON_POINTER, -1, -1 );

    int tx = x;
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Ptr " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( " <%s>" ), m_pNode->GetName( ) );
    tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_A );

    tx += g_FontWidth;
    tx = AddComment( View, tx, y );

    y += g_FontHeight;
    DrawSize.x = tx;
    if (m_LevelsOpen[View->Level])
    {
        DWORD NeededSize = m_pNode->GetMemorySize( );
        m_Memory.SetSize( NeededSize );

        VIEWINFO NewView;
        memcpy( &NewView, View, sizeof( NewView ) );
        NewView.Data = m_Memory.Data( );
        NewView.Address = *Data;

        ReClassReadMemory( (LPVOID)NewView.Address, (LPVOID)NewView.Data, NeededSize );

        ChildDrawSize = m_pNode->Draw( &NewView, x, y );
        y = ChildDrawSize.y;
        if (ChildDrawSize.x > DrawSize.x) 
        {
            DrawSize.x = ChildDrawSize.x;
        }
    }

    DrawSize.y = y;
    return DrawSize;
}