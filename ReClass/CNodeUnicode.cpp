#include "stdafx.h"
#include "CNodeUnicode.h"

CNodeUnicode::CNodeUnicode( )
{
	m_nodeType = nt_unicode;
	m_strName = _T( "Unicode" );
	m_dwMemorySize = 8 * sizeof( wchar_t );
}

void CNodeUnicode::Update( const PHOTSPOT Spot )
{
    LPCTSTR SourceString;
    wchar_t* ConvertedString;
    SIZE_T Length;

	StandardUpdate( Spot );

	if (Spot->Id == 0)
	{
		m_dwMemorySize = _ttoi( Spot->Text.GetString( ) ) * sizeof( wchar_t );
	}
	else if (Spot->Id == 1)
	{
		Length = Spot->Text.GetLength( );
		if (Length > (m_dwMemorySize / sizeof( wchar_t )))
			Length = (m_dwMemorySize / sizeof( wchar_t ));

		// Has to be done this way in order to make it compatible in mbs and unicode mode (ghetto)
        SourceString = Spot->Text.GetString( );
        ConvertedString = new wchar_t[Length + 1];
		for (size_t i = 0; i <= Length; i++)
            ConvertedString[i] = (wchar_t)SourceString[i];

		ReClassWriteMemory( (LPVOID)Spot->Address, ConvertedString, Length );

		delete[] ConvertedString;
	}
}

NODESIZE CNodeUnicode::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    wchar_t *Data;

	if (m_bHidden)
		return DrawHidden( View, x, y );

    Data = (wchar_t*)(View->Data + m_Offset);

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_TEXT, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Unicode " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), m_dwMemorySize / sizeof( wchar_t ) );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

	if (VALID( Data ))
	{
		CStringW MemoryString( GetStringFromMemoryW( Data, m_dwMemorySize / sizeof( wchar_t ) ) );
		tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( " = '" ) );
		tx = AddText( View, tx, y, g_crChar, HS_OPENCLOSE, _T( "%.150ws" ), MemoryString.GetString( ) ); // ws cause its unicode
		tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( "' " ) ) + g_FontWidth;
	}

	tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
	return DrawSize;
}
