#include "stdafx.h"
#include "CNodeMatrix.h"

CNodeMatrix::CNodeMatrix( )
{
	m_nodeType = nt_matrix;
}

void CNodeMatrix::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
	if (Spot.ID < 16)
	{
		float v = (float)_ttof( Spot.Text.GetString( ) );
		ReClassWriteMemory( (LPVOID)(Spot.Address + (Spot.ID * sizeof( float ))), &v, sizeof( float ) );
	}
}

int CNodeMatrix::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	float* pMemory = (float*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_MATRIX, HS_NONE, HS_NONE );
	int mx = tx;
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Matrix " ) );
	tx = AddText( View, tx, y, g_crName, 69, _T( "%s" ), m_strName );
	tx = AddOpenClose( View, tx, y );
	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	if (m_LevelsOpen[View.Level])
	{
		y += g_FontHeight;
		tx = mx;
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
		tx = AddText( View, tx, y, g_crValue, 0, _T( "% 14.3f" ), pMemory[0] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 1, _T( "% 14.3f" ), pMemory[1] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 2, _T( "% 14.3f" ), pMemory[2] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 3, _T( "% 14.3f" ), pMemory[3] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
		y += g_FontHeight;
		tx = mx;
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
		tx = AddText( View, tx, y, g_crValue, 4, _T( "% 14.3f" ), pMemory[4] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 5, _T( "% 14.3f" ), pMemory[5] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 6, _T( "% 14.3f" ), pMemory[6] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 7, _T( "% 14.3f" ), pMemory[7] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
		y += g_FontHeight;
		tx = mx;
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
		tx = AddText( View, tx, y, g_crValue, 8, _T( "% 14.3f" ), pMemory[8] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 9, _T( "% 14.3f" ), pMemory[9] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 10, _T( "% 14.3f" ), pMemory[10] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 11, _T( "% 14.3f" ), pMemory[11] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
		y += g_FontHeight;
		tx = mx;
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
		tx = AddText( View, tx, y, g_crValue, 12, _T( "% 14.3f" ), pMemory[12] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 13, _T( "% 14.3f" ), pMemory[13] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 14, _T( "% 14.3f" ), pMemory[14] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 15, _T( "% 14.3f" ), pMemory[15] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
	}

	return y + g_FontHeight;
}
