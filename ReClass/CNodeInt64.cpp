#include "stdafx.h"
#include "CNodeInt64.h"

CNodeInt64::CNodeInt64( )
{
    m_nodeType = nt_int64;
}

void CNodeInt64::Update( const PHOTSPOT Spot )
{
    __int64 Int64Value;

    StandardUpdate( Spot );

    Int64Value = _ttoi64( Spot->Text.GetString( ) );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &Int64Value, sizeof( __int64 ) );
}

NODESIZE CNodeInt64::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    __int64 Int64;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Int64 = *(__int64*)(View->Data + m_Offset);

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_clrType, HS_NONE, _T( "Int64 " ) );
    tx = AddText( View, tx, y, g_clrName, HS_NAME, _T( "%s" ), m_strName.GetString( ) );
    tx = AddText( View, tx, y, g_clrName, HS_NONE, _T( " = " ) );
    tx = AddText( View, tx, y, g_clrValue, HS_EDIT, _T( "%lli" ), Int64 ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
