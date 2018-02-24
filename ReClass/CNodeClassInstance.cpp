#include "stdafx.h"
#include "CNodeClassInstance.h"

CNodeClassInstance::CNodeClassInstance( ) 
    : m_pNode( NULL )
{
    m_nodeType = nt_instance;
}

void CNodeClassInstance::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );
}

ULONG CNodeClassInstance::GetMemorySize( )
{
    return m_pNode->GetMemorySize( );
}

NODESIZE CNodeClassInstance::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    NODESIZE ChildDrawSize;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    DrawSize.x = 0;
    DrawSize.y = 0;

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );

    x = AddOpenClose( View, x, y );
    x = AddIcon( View, x, y, ICON_CLASS, -1, -1 );

    tx = x;
    tx = AddAddressOffset( View, tx, y );

    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Instance " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( "<%s>" ), m_pNode->GetName( ) );
    tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X );

    tx += g_FontWidth;
    tx = AddComment( View, tx, y );

    y += g_FontHeight;
    if (m_LevelsOpen[View->Level])
    {
        VIEWINFO NewView;
        memcpy( &NewView, View, sizeof( NewView ) );
        NewView.Address = View->Address + m_Offset;
        NewView.Data = (UCHAR*)((uintptr_t)NewView.Data + m_Offset);
        
        ChildDrawSize = m_pNode->Draw( &NewView, x, y );

        y = ChildDrawSize.y;
        if (ChildDrawSize.x > DrawSize.x) 
        {
            DrawSize.x = ChildDrawSize.x;
        }
    }

    DrawSize.x = tx;
    DrawSize.y = y;
    return DrawSize;
}
