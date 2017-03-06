#include "stdafx.h"
#include "CNodeVec2.h"

CNodeVec2::CNodeVec2( )
{
	m_nodeType = nt_vec2;
	for (UINT i = 0; i < m_LevelsOpen.size( ); i++)
		m_LevelsOpen[i] = true;
}

void CNodeVec2::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	float v = (float)_ttof( Spot.Text.GetString( ) );
	if (Spot.ID >= 0 && Spot.ID < 2)
		ReClassWriteMemory( (LPVOID)(Spot.Address + (Spot.ID * 4)), &v, 4 );
}

NodeSize CNodeVec2::Draw( const ViewInfo& View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	NodeSize drawnSize;
	float* pMemory = (float*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_VECTOR, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Vec2 " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddOpenClose( View, tx, y );
	if (m_LevelsOpen[View.Level])
	{
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "(" ) );
		tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%0.3f" ), pMemory[0] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, HS_OPENCLOSE, _T( "%0.3f" ), pMemory[1] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( ")" ) );
	}
	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	drawnSize.x = tx;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}
