#include "stdafx.h"
#include "CNodeHex64.h"

CNodeHex64::CNodeHex64( )
{
	m_nodeType = nt_hex64;
}

void CNodeHex64::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	unsigned char v = (unsigned char)(_tcstoul( Spot.Text.GetString( ), NULL, 16 ) & 0xFF);
	if (Spot.ID >= 0 && Spot.ID < 8)
		ReClassWriteMemory( (LPVOID)(Spot.Address + Spot.ID), &v, 1 );
}

NodeSize CNodeHex64::Draw( const ViewInfo& View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	NodeSize drawnSize;
	unsigned char* pMemory = (unsigned char*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET + 16;
	tx = AddAddressOffset( View, tx, y );

	if (g_bText)
	{
		CStringA str = GetStringFromMemoryA( (char*)pMemory, 8 ) + " ";
		tx = AddText( View, tx, y, g_crChar, HS_NONE, "%s", str.GetBuffer( ) );
	}

	tx = AddText( View, tx, y, g_crHex, 0, _T( "%0.2X" ), pMemory[0] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 1, _T( "%0.2X" ), pMemory[1] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 2, _T( "%0.2X" ), pMemory[2] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 3, _T( "%0.2X" ), pMemory[3] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 4, _T( "%0.2X" ), pMemory[4] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 5, _T( "%0.2X" ), pMemory[5] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 6, _T( "%0.2X" ), pMemory[6] ) + g_FontWidth;
	tx = AddText( View, tx, y, g_crHex, 7, _T( "%0.2X" ), pMemory[7] ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	drawnSize.x = tx;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}
