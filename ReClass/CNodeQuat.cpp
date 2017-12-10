#include "stdafx.h"
#include "CNodeQuat.h"

CNodeQuat::CNodeQuat( )
{
	m_nodeType = nt_quat;
	for (UINT i = 0; i < m_LevelsOpen.size( ); i++)
		m_LevelsOpen[i] = true;
}

void CNodeQuat::Update( const PHOTSPOT Spot )
{
    float FloatValue;

	StandardUpdate( Spot );

    FloatValue = (float)_ttof( Spot->Text.GetString( ) );
	if (Spot->Id >= 0 && Spot->Id < 4)
		ReClassWriteMemory( (LPVOID)(Spot->Address + (Spot->Id * sizeof( float ))), &FloatValue, sizeof( float ) );
}

NODESIZE CNodeQuat::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    float* Data;

	if (m_bHidden)
		return DrawHidden( View, x, y );
	
    Data = (float*)(View->Data + m_Offset);
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_VECTOR, -1, -1 );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Vec4 " ) );
	tx = AddText( View, tx, y, g_crName, 69, _T( "%s" ), m_strName );
	tx = AddOpenClose( View, tx, y );
	if (m_LevelsOpen[View->Level])
	{
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "(" ) );
		tx = AddText( View, tx, y, g_crValue, 0, _T( "%0.3f" ), Data[0] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 1, _T( "%0.3f" ), Data[1] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 2, _T( "%0.3f" ), Data[2] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 3, _T( "%0.3f" ), Data[3] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( ")" ) );
	}
	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	DrawSize.x = tx;
	DrawSize.y = y + g_FontHeight;
	return DrawSize;
}
