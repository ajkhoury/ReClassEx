#include "stdafx.h"
#include "CNodeInt32.h"

CNodeInt32::CNodeInt32( )
{
    m_nodeType = nt_int32;
}

void CNodeInt32::Update( const PHOTSPOT Spot )
{
    __int32 Int32Value;

    StandardUpdate( Spot );

    Int32Value = _ttoi( Spot->Text.GetString( ) );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &Int32Value, sizeof( long ) );
}

NODESIZE CNodeInt32::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    __int32* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );
 
    Data = (__int32*)(View->Data + m_Offset);
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_clrType, HS_NONE, _T( "Int32 " ) );
    tx = AddText( View, tx, y, g_clrName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_clrName, HS_NONE, _T( " = " ) );
    tx = AddText( View, tx, y, g_clrValue, HS_EDIT, _T( "%i" ), *Data ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
