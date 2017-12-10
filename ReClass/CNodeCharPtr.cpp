#include "stdafx.h"
#include "CNodeCharPtr.h"

CNodeCharPtr::CNodeCharPtr( )
{
	m_nodeType = nt_pchar;
	m_strName = "PChar";
}

void CNodeCharPtr::Update( const PHOTSPOT Spot )
{
    uintptr_t PointerValue;

	StandardUpdate( Spot );

	#if defined(_M_AMD64)
    PointerValue = (uintptr_t)_ttoi64( Spot->Text.GetString( ) );
	#else
	PointerValue = (uintptr_t)_ttoi( Spot->Text.GetString( ) );
	#endif
	if (Spot->Id == 0)
		ReClassWriteMemory( (LPVOID)Spot->Address, &PointerValue, sizeof( uintptr_t ) );
}

NODESIZE CNodeCharPtr::Draw( const PVIEWINFO View, int x, int y )
{
	int tx;
    NODESIZE DrawSize;
	uintptr_t* Data;

	if (m_bHidden)
		return DrawHidden( View, x, y );

    Data = (uintptr_t*)(View->Data + m_Offset);

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
	if (VALID( Data ))
	{
		CStringA MemoryString( ReadMemoryStringA( *Data, 128 ) );
		tx = AddText( View, tx, y, g_crChar, 1, "%s", MemoryString.GetBuffer( ) );
	}

	tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( "' " ) ) + g_FontWidth;
	tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
	return DrawSize;
}
