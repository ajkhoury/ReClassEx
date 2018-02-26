#include "stdafx.h"
#include "CNodeByte.h"

CNodeByte::CNodeByte( )
{
    m_nodeType = nt_uint8;
}

void CNodeByte::Update( const PHOTSPOT Spot )
{
    unsigned __int8 ByteValue;

    StandardUpdate( Spot );

    ByteValue = (unsigned __int8)_tcstoul( Spot->Text.GetString( ), NULL, g_bUnsignedHex ? 16 : 10 );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &ByteValue, sizeof( unsigned __int8 ) );
}

NODESIZE CNodeByte::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    unsigned __int8* Data;
    
    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (unsigned __int8*)(View->Data + m_Offset);

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_clrType, HS_NONE, _T( "BYTE  " ) );
    tx = AddText( View, tx, y, g_clrName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_clrName, HS_NONE, _T( " = " ) );
    tx = AddText( View, tx, y, g_clrValue, HS_EDIT, g_bUnsignedHex ? _T( "0x%02X" ) : _T( "%u" ), Data[0] ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
