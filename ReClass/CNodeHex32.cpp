#include "stdafx.h"
#include "CNodeHex32.h"

CNodeHex32::CNodeHex32( )
{
    m_nodeType = nt_hex32;
}

void CNodeHex32::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );
    unsigned char v = (unsigned char)(_tcstoul( Spot->Text.GetString( ), NULL, 16 ) & 0xFF);
    if (Spot->Id >= 0 && Spot->Id < 4)
        ReClassWriteMemory( (LPVOID)(Spot->Address + Spot->Id), &v, 1 );
}

NODESIZE CNodeHex32::Draw( const PVIEWINFO View, int x, int y )
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
        // TODO these are the dots, do alignment instead of 4
        CStringA str = GetStringFromMemoryA( (const char*)Data, 4 );
        str += "     ";
        tx = AddText( View, tx, y, g_crChar, HS_NONE, "%s", str );
    }

    tx = AddText( View, tx, y, g_crHex, 0, _T( "%0.2X" ), Data[0] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 1, _T( "%0.2X" ), Data[1] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 2, _T( "%0.2X" ), Data[2] ) + g_FontWidth;
    tx = AddText( View, tx, y, g_crHex, 3, _T( "%0.2X" ), Data[3] ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}