#include "stdafx.h"

#include "CNodeWord.h"

CNodeWord::CNodeWord( )
{
    m_nodeType = nt_uint16;
}

void CNodeWord::Update( const PHOTSPOT Spot )
{
    WORD Uint16Value;

    StandardUpdate( Spot );

    Uint16Value = (WORD)_tcstoul( Spot->Text.GetString( ), NULL, g_bUnsignedHex ? 16 : 10 );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &Uint16Value, sizeof( WORD ) );
}

NODESIZE CNodeWord::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    WORD *Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (WORD*)(View->Data + m_Offset);

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_clrType, HS_NONE, _T( "WORD  " ) );
    tx = AddText( View, tx, y, g_clrName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_clrName, HS_NONE, _T( " = " ) );
    tx = AddText( View, tx, y, g_clrValue, HS_EDIT, g_bUnsignedHex ? _T( "0x%X" ) : _T( "%u" ), *Data ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
