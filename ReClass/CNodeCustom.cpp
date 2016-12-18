#include "stdafx.h"
#include "CNodeCustom.h"

CNodeCustom::CNodeCustom( )
{
	m_nodeType = nt_custom;
	m_strName = _T( "Custom" );
	m_dwMemorySize = sizeof( void* );
}

void CNodeCustom::Update( HotSpot& Spot )
{
	StandardUpdate( Spot );
	if (Spot.ID == 0)
		m_dwMemorySize = _ttoi( Spot.Text.GetString( ) );
}

ULONG CNodeCustom::GetMemorySize( )
{
	return m_dwMemorySize;
}

int CNodeCustom::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_CUSTOM, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Custom " ) );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), m_dwMemorySize );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "] " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName ) + g_FontWidth;
	tx = AddComment( View, tx, y );
	return y += g_FontHeight;
}