#include "stdafx.h"
#include "CNodePtr.h"

CNodePtr::CNodePtr( ) 
    : m_pClassNode( NULL )
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
    ULONG_PTR* Data;
    int tx;

    if (m_bHidden)
        return DrawHidden( View, x, y );
    
    Data = (ULONG_PTR*)(View->Data + m_Offset);

    //printf( "read ptr: %p\n", View->Data );
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );

    x = AddOpenClose( View, x, y );
    x = AddIcon( View, x, y, ICON_POINTER, -1, -1 );

    tx = AddAddressOffset( View, x, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Ptr " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( " <%s>" ), m_pClassNode->GetName( ) );
    tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_A );

    tx += g_FontWidth;
    tx = AddComment( View, tx, y );

    y += g_FontHeight;
    DrawSize.x = tx;

    if (m_LevelsOpen[View->Level])
    {
        DWORD NeededSize = m_pClassNode->GetMemorySize( );
        m_Memory.SetSize( NeededSize );

        VIEWINFO NewView;
        memcpy( &NewView, View, sizeof( NewView ) );
        NewView.Data = m_Memory.Data( );
        NewView.Address = *Data;

        ReClassReadMemory( (LPVOID)NewView.Address, (LPVOID)NewView.Data, NeededSize );

        ChildDrawSize = m_pClassNode->Draw( &NewView, x, y );

        y = ChildDrawSize.y;
        if (ChildDrawSize.x > DrawSize.x) 
            DrawSize.x = ChildDrawSize.x;
    }

    DrawSize.y = y;
    return DrawSize;
}