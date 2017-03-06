#include "stdafx.h"
#include "CNodeInt64.h"

CNodeInt64::CNodeInt64( )
{
	m_nodeType = nt_int64;
}

void CNodeInt64::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	__int64 v = _ttoi64( Spot.Text.GetString( ) );
	if (Spot.ID == 0)
		ReClassWriteMemory( (LPVOID)Spot.Address, &v, sizeof( __int64 ) );
}

NodeSize CNodeInt64::Draw( const ViewInfo& View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	NodeSize drawnSize;
	__int64 Int64 = *(__int64*)(&View.pData[m_Offset]);

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Int64 " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName.GetString( ) );
	tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
	tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%lli" ), Int64 ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	drawnSize.x = tx;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}
