#include "stdafx.h"
#include "CNodeBits.h"

#include "BitArray.h"

CNodeBits::CNodeBits( )
{
    m_nodeType = nt_bits;
}

void CNodeBits::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );
    unsigned char v = (unsigned char)(_tcstoul( Spot->Text.GetString( ), NULL, 16 ) & 0xFF);
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &v, 1 );
}

NODESIZE CNodeBits::Draw( const PVIEWINFO View, int x, int y )
{
    UCHAR* Data;
    NODESIZE DrawSize;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (UCHAR*)&View->Data[m_Offset];
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    int tx = x + TXOFFSET + 16;
    tx = AddAddressOffset( View, tx, y );

    if (g_bText)
    {
        BitArray<UCHAR> bits;
        bits.SetValue( Data[0] );

        tx = AddText( View, tx, y, g_crChar, HS_EDIT, "%s ", bits.GetBitsReverseString( ) );
    }

    tx = AddText( View, tx, y, g_crHex, 0, _T( "%0.2X" ), Data[0] ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;

    return DrawSize;
}
