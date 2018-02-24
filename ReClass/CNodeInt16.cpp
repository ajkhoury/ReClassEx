#include "stdafx.h"
#include "CNodeInt16.h"

CNodeInt16::CNodeInt16( )
{
    m_nodeType = nt_int16;
}

void CNodeInt16::Update( const PHOTSPOT Spot )
{
    __int16 Int16Value;

    StandardUpdate( Spot );

    Int16Value = _ttoi( Spot->Text.GetString( ) );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &Int16Value, sizeof( __int16 ) );
}

NODESIZE CNodeInt16::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    __int16* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (__int16*)(View->Data + m_Offset);
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Int16 " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
    tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%i" ), *Data ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
