#include "stdafx.h"
#include "CNodeInt8.h"

CNodeInt8::CNodeInt8( )
{
	m_nodeType = nt_int8;
}

void CNodeInt8::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
	__int8 v = _ttoi( Spot.Text.GetString( ) );
	if (Spot.ID == 0)
		ReClassWriteMemory( (LPVOID)Spot.Address, &v, sizeof( char ) );
}

int CNodeInt8::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	__int8* pMemory = (__int8*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Int8  " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
	tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%i" ), pMemory[0] ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	return y += g_FontHeight;
}
