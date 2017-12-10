#include "stdafx.h"
#include "CNodeVec2.h"

CNodeVec2::CNodeVec2( )
{
	m_nodeType = nt_vec2;
	for (size_t i = 0; i < m_LevelsOpen.size( ); i++)
		m_LevelsOpen[i] = true;
}

void CNodeVec2::Update( const PHOTSPOT Spot )
{
    float FloatValue;

	StandardUpdate( Spot );

    FloatValue = (float)_ttof( Spot->Text.GetString( ) );
	if (Spot->Id >= 0 &&Spot->Id < 2)
		ReClassWriteMemory( (LPVOID)(Spot->Address + (Spot->Id * 4)), &FloatValue, sizeof( float ) );
}

NODESIZE CNodeVec2::Draw( const PVIEWINFO View, int x, int y )
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
	tx = AddIcon( View, tx, y, ICON_VECTOR, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Vec2 " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddOpenClose( View, tx, y );
	if (m_LevelsOpen[View->Level])
	{
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "(" ) );
		tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%0.3f" ), Data[0] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, HS_OPENCLOSE, _T( "%0.3f" ), Data[1] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( ")" ) );
	}
	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
	return DrawSize;
}
