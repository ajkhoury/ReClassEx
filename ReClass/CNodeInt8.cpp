#include "stdafx.h"
#include "CNodeInt8.h"

CNodeInt8::CNodeInt8( )
{
	m_nodeType = nt_int8;
}

void CNodeInt8::Update( const PHOTSPOT Spot )
{
    __int8 Int8Value;

	StandardUpdate( Spot );

    Int8Value = _ttoi( Spot->Text.GetString( ) );
	if (Spot->Id == 0)
		ReClassWriteMemory( (LPVOID)Spot->Address, &Int8Value, sizeof( __int8 ) );
}

NODESIZE CNodeInt8::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    __int8* Data;

	if (m_bHidden)
		return DrawHidden( View, x, y );

    Data = (__int8*)(View->Data + m_Offset);
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Int8  " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
	tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%i" ), *Data ) + g_FontWidth;
	tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
	return DrawSize;
}
