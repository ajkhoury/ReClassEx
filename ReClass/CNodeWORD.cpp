#include "stdafx.h"
#include "CNodeWORD.h"

CNodeWORD::CNodeWORD( )
{
	m_nodeType = nt_uint16;
}

void CNodeWORD::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
	WORD v = _ttoi( Spot.Text.GetString( ) );
	if (Spot.ID == 0)
		ReClassWriteMemory( (LPVOID)Spot.Address, &v, sizeof( unsigned short ) );
}

int CNodeWORD::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	WORD* pMemory = (WORD*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "WORD  " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
	tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%u" ), pMemory[0] ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	return y += g_FontHeight;
}
