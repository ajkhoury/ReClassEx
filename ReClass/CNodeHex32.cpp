#include "stdafx.h"
#include "CNodeHex32.h"

CNodeHex32::CNodeHex32( )
{
	m_nodeType = nt_hex32;
}

void CNodeHex32::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
	unsigned char v = (unsigned char)(_tcstoul( Spot.Text.GetString( ), NULL, 16 ) & 0xFF);
	if (Spot.ID >= 0 && Spot.ID < 4)
		ReClassWriteMemory( (LPVOID)(Spot.Address + Spot.ID), &v, 1 );
}

int CNodeHex32::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	unsigned char* pMemory = (unsigned char*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET + 16;
	tx = AddAddressOffset( View, tx, y );

	if (g_bText)
	{
		// TODO these are the dots, do alignment instead of 4
		CStringA str = GetStringFromMemoryA( (char*)pMemory, 4 );
		str += "     ";
		tx = AddText( View, tx, y, g_crChar, HS_NONE, "%s", str );
	}

	tx = AddText( View, tx, y, g_crHex, 0, _T( "%0.2X" ), pMemory[0] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 1, _T( "%0.2X" ), pMemory[1] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 2, _T( "%0.2X" ), pMemory[2] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 3, _T( "%0.2X" ), pMemory[3] ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	return y += g_FontHeight;
}