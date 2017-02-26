#include "stdafx.h"
#include "CNodeFloat.h"

CNodeFloat::CNodeFloat( )
{
	m_nodeType = nt_float;
}

void CNodeFloat::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
	float v = (float)_ttof( Spot.Text.GetString( ) );
	if (Spot.ID == HS_EDIT)
		ReClassWriteMemory( (LPVOID)Spot.Address, &v, 4 );
}

NodeSize CNodeFloat::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	NodeSize drawnSize;
	float* pMemory = (float*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_FLOAT, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "float " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
	//tx = AddText(View,tx,y,g_crValue,0,"%.4f",pMemory[0]) + FontWidth;

	//if ( *pMemory > -99999.0f && *pMemory < 99999.0f )
	//	*pMemory = 0;

	tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%4.3f" ), pMemory[0] ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	drawnSize.x = tx;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}
