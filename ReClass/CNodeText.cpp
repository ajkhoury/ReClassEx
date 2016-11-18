#include "stdafx.h"
#include "CNodeText.h"

CNodeText::CNodeText( )
{
	m_nodeType = nt_text;
	m_strName = _T( "Text" );
	memsize = 16;
}

void CNodeText::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
	if (Spot.ID == 0)
	{
		memsize = _ttoi( Spot.Text.GetBuffer( ) );
	}
	else if (Spot.ID == 1)
	{
		SIZE_T Length = Spot.Text.GetLength( ) + 1;
		if (Length > memsize)
			Length = memsize;
		ReClassWriteMemory( (LPVOID)Spot.Address, (LPVOID)Spot.Text.GetBuffer( ), Length );
	}
}

int CNodeText::GetMemorySize( )
{
	return memsize;
}

int CNodeText::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	char* pMemory = (char*)&View.pData[m_Offset];

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_TEXT, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Text " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), GetMemorySize( ) );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

	if (VALID( pMemory ))
	{
		CStringA str = GetStringFromMemoryA( pMemory, GetMemorySize( ) );
		tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( " = '" ) );
		tx = AddText( View, tx, y, g_crChar, 1, "%.150s", str.GetBuffer( ) );
		tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( "' " ) ) + g_FontWidth;
	}

	tx = AddComment( View, tx, y );
	return y += g_FontHeight;
}
