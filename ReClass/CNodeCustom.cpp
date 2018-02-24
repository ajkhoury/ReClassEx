#include "stdafx.h"
#include "CNodeCustom.h"

CNodeCustom::CNodeCustom( )
{
    m_nodeType = nt_custom;
    m_strName = _T( "Custom" );
    m_ulMemorySize = sizeof( void* );
}

void CNodeCustom::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );
    if (Spot->Id == 0)
        m_ulMemorySize = _ttoi( Spot->Text.GetString( ) );
}

ULONG CNodeCustom::GetMemorySize( )
{
    return m_ulMemorySize;
}

NODESIZE CNodeCustom::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;

    if (m_bHidden)
        return DrawHidden( View, x, y );
 
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_CUSTOM, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Custom " ) );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
    tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), m_ulMemorySize );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "] " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName ) + g_FontWidth;
    tx = AddComment( View, tx, y );
    
    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}