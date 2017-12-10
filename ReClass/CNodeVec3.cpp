#include "stdafx.h"
#include "CNodeVec3.h"

CNodeVec3::CNodeVec3( )
{
	m_nodeType = nt_vec3;
	for (size_t i = 0; i < m_LevelsOpen.size( ); i++)
		m_LevelsOpen[i] = true;
}

void CNodeVec3::Update( const PHOTSPOT Spot )
{
    float FloatValue;

	StandardUpdate( Spot );

    FloatValue = (float)_ttof( Spot->Text.GetString( ) );
	if (Spot->Id >= 0 && Spot->Id < 3)
		ReClassWriteMemory( (LPVOID)(Spot->Address + (Spot->Id * sizeof( float ))), &FloatValue, sizeof( float ) );
}

NODESIZE CNodeVec3::Draw( const PVIEWINFO View, int x, int y )
{
    NODESIZE DrawSize;
    float* Data;

	if (m_bHidden)
		return DrawHidden( View, x, y );

    Data = (float*)(View->Data + m_Offset);

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_VECTOR, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Vec3 " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddOpenClose( View, tx, y );
	if (m_LevelsOpen[View->Level])
	{
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "(" ) );
		tx = AddText( View, tx, y, g_crValue, 0, _T( "%0.3f" ), Data[0] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 1, _T( "%0.3f" ), Data[1] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
		tx = AddText( View, tx, y, g_crValue, 2, _T( "%0.3f" ), Data[2] );
		tx = AddText( View, tx, y, g_crName, HS_NONE, _T( ")" ) );
	}
	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
	return DrawSize;
}
