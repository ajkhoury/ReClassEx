#include "stdafx.h"
#include "CNodeQword.h"

CNodeQword::CNodeQword( )
{
    m_nodeType = nt_uint64;
}

void CNodeQword::Update( const PHOTSPOT Spot )
{
    UINT64 Uint64Value;

    StandardUpdate( Spot );

    Uint64Value = _tcstoull( Spot->Text.GetString( ), NULL, g_bUnsignedHex ? 16 : 10 );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &Uint64Value, sizeof( UINT64 ) );
}

NODESIZE CNodeQword::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    UINT64* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );
   
    Data = (UINT64*)(View->Data + m_Offset);
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "QWORD " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
    tx = AddText( View, tx, y, g_crValue, HS_EDIT, g_bUnsignedHex ? _T( "0x%I64X" ) : _T( "%llu" ), *Data ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
