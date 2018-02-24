#include "stdafx.h"
#include "CNodeHex64.h"

CNodeHex64::CNodeHex64( )
{
    m_nodeType = nt_hex64;
}

void CNodeHex64::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );
    unsigned char v = (unsigned char)(_tcstoul( Spot->Text.GetString( ), NULL, 16 ) & 0xFF);
    if (Spot->Id >= 0 && Spot->Id < 8)
        ReClassWriteMemory( (LPVOID)(Spot->Address + Spot->Id), &v, 1 );
}

NODESIZE CNodeHex64::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    const UCHAR* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (const UCHAR*)(View->Data + m_Offset);
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET + 16;
    tx = AddAddressOffset( View, tx, y );

    if (g_bText)
    {
        CStringA AsciiMemory = GetStringFromMemoryA( (const char*)Data, 8 ) + " ";
        tx = AddText( View, tx, y, g_crChar, HS_NONE, "%s", AsciiMemory.GetBuffer( ) );
    }

    tx = AddText( View, tx, y, g_crHex, 0, _T( "%0.2X" ), Data[0] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 1, _T( "%0.2X" ), Data[1] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 2, _T( "%0.2X" ), Data[2] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 3, _T( "%0.2X" ), Data[3] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 4, _T( "%0.2X" ), Data[4] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 5, _T( "%0.2X" ), Data[5] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 6, _T( "%0.2X" ), Data[6] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 7, _T( "%0.2X" ), Data[7] ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
