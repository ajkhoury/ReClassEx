#include "stdafx.h"
#include "CNodeQuat.h"

CNodeQuat::CNodeQuat( )
{
	m_nodeType = nt_quat;
	for (UINT i = 0; i < m_LevelsOpen.size( ); i++)
		m_LevelsOpen[i] = true;
}

void CNodeQuat::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
	float v = (float)_ttof( Spot.Text.GetString( ) );
	if (Spot.ID >= 0 && Spot.ID < 4)
		ReClassWriteMemory( (LPVOID)(Spot.Address + (Spot.ID * sizeof( float ))), &v, sizeof( float ) );
}

int CNodeQuat::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	float* pMemory = (float*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_VECTOR, -1, -1 );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Vec4 " ) );
	tx = AddText( View, tx, y, g_crName, 69, _T( "%s" ), m_strName );
	tx = AddOpenClose( View, tx, y );
	if (m_LevelsOpen[View.Level])
	{
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "(" ) );
		tx = AddText( View, tx, y, g_crValue, 0, _T( "%0.3f" ), pMemory[0] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 1, _T( "%0.3f" ), pMemory[1] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 2, _T( "%0.3f" ), pMemory[2] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 3, _T( "%0.3f" ), pMemory[3] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( ")" ) );
	}
	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	return y + g_FontHeight;
}
