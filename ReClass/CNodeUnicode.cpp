#include "stdafx.h"
#include "CNodeUnicode.h"

CNodeUnicode::CNodeUnicode( )
{
	m_nodeType = nt_unicode;
	m_strName = _T( "Unicode" );
	m_dwMemorySize = 8 * sizeof( wchar_t );
}

void CNodeUnicode::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	if (Spot.ID == 0)
	{
		m_dwMemorySize = _ttoi( Spot.Text.GetString( ) ) * sizeof( wchar_t );
	}
	else if (Spot.ID == 1)
	{
		SIZE_T Length = Spot.Text.GetLength( );
		if (Length > (m_dwMemorySize / sizeof( wchar_t )))
			Length = (m_dwMemorySize / sizeof( wchar_t ));

		// Has to be done this way in order to make it compatible in mbs and unicode mode (ghetto)
		LPCTSTR pSource = Spot.Text.GetString( );
		wchar_t* pwszConverted = new wchar_t[Length + 1];
		for (UINT i = 0; i <= Length; i++)
			pwszConverted[i] = (wchar_t)pSource[i];

		ReClassWriteMemory( (LPVOID)Spot.Address, pwszConverted, Length );

		delete[] pwszConverted;
	}
}

NodeSize CNodeUnicode::Draw( const ViewInfo& View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	NodeSize drawnSize;
	wchar_t* pMemory = (wchar_t*)&((unsigned char*)View.pData)[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_TEXT, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Unicode " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), m_dwMemorySize / sizeof( wchar_t ) );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

	if (VALID( pMemory ))
	{
		CStringW str( GetStringFromMemoryW( pMemory, m_dwMemorySize / sizeof( wchar_t ) ) );
		tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( " = '" ) );
		tx = AddText( View, tx, y, g_crChar, HS_OPENCLOSE, _T( "%.150ws" ), str ); // ws cause its unicode
		tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( "' " ) ) + g_FontWidth;
	}

	tx = AddComment( View, tx, y );

	drawnSize.x = tx;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}
