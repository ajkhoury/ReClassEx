#include "stdafx.h"
#include "CNodeByte.h"

CNodeByte::CNodeByte( )
{
	m_nodeType = nt_uint8;
}

void CNodeByte::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	UCHAR v = (UCHAR)_tcstoul( Spot.Text.GetString( ), NULL, g_bUnsignedHex ? 16 : 10 );
	if (Spot.ID == 0)
		ReClassWriteMemory( (LPVOID)Spot.Address, &v, sizeof( unsigned char ) );
}

NodeSize CNodeByte::Draw( const ViewInfo& View, int x, int y )
{
	int tx = 0;
	UCHAR* pMemory = NULL;
	NodeSize drawnSize;

	if (m_bHidden)
		return DrawHidden( View, x, y );

	pMemory = (UCHAR*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "BYTE  " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
	tx = AddText( View, tx, y, g_crValue, HS_EDIT, g_bUnsignedHex ? _T( "0x%02X" ) : _T( "%u" ), pMemory[0] ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	drawnSize.x = tx;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}
