#include "stdafx.h"
#include "CNodeCharPtr.h"

CNodeCharPtr::CNodeCharPtr( )
{
	m_nodeType = nt_pchar;
	m_strName = "PChar";
}

void CNodeCharPtr::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	#ifdef _WIN64
	size_t ptr = _ttoi64( Spot.Text.GetString( ) );
	#else
	size_t ptr = _ttoi( Spot.Text.GetString( ) );
	#endif
	if (Spot.ID == 0)
		ReClassWriteMemory( (LPVOID)Spot.Address, &ptr, sizeof( size_t ) );
}

NodeSize CNodeCharPtr::Draw( const ViewInfo& View, int x, int y )
{
	int tx = 0;
	ULONG_PTR* pMemory = NULL;
	NodeSize drawnSize;

	if (m_bHidden)
		return DrawHidden( View, x, y );

	pMemory = (ULONG_PTR*)&View.pData[m_Offset];

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View, x, y);

	tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "PCHAR " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, m_strName );

	//tx = AddText(View,tx,y,g_crName,HS_NONE," = ");
	//tx = AddText(View,tx,y,g_crValue,0,"%lli",pMemory[0]) + FontWidth;
	//tx = AddComment(View,tx,y);

	/*
	int tx = x + 16;
	tx = AddIcon(View,tx,y,ICON_TEXT,HS_NONE,HS_NONE);
	tx = AddAddressOffset(View,tx,y);
	tx = AddText(View,tx,y,g_crType,HS_NONE,"Text ");
	tx = AddText(View,tx,y,g_crName,69,"%s",m_strName);
	tx = AddText(View,tx,y,g_crIndex,HS_NONE,"[");
	tx = AddText(View,tx,y,g_crIndex,0,"%i",memsize);
	tx = AddText(View,tx,y,g_crIndex,HS_NONE,"]");
	*/

	tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( " = '" ) );
	if (VALID( pMemory ))
	{
		CStringA str( ReadMemoryStringA( (ULONG_PTR)pMemory[0], 128 ) );
		tx = AddText( View, tx, y, g_crChar, 1, "%s", str.GetBuffer( ) );
	}

	tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( "' " ) ) + g_FontWidth;
	tx = AddComment( View, tx, y );

	drawnSize.x = tx;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}
