#include "stdafx.h"
#include "CNodeDword.h"

CNodeDword::CNodeDword( )
{
	m_nodeType = nt_uint32;
}

void CNodeDword::Update( const PHOTSPOT Spot )
{
    unsigned __int32 UInt32Value;

	StandardUpdate( Spot );

    UInt32Value = _tcstoul( Spot->Text.GetString( ), NULL, g_bUnsignedHex ? 16 : 10 );
	if (Spot->Id == 0)
		ReClassWriteMemory( (LPVOID)Spot->Address, &UInt32Value, sizeof( unsigned long ) );
}

NODESIZE CNodeDword::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    unsigned __int32* Data;

	if (m_bHidden)
		return DrawHidden( View, x, y );

    
    Data = (unsigned __int32*)(View->Data + m_Offset);
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE );
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "DWORD " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
	tx = AddText( View, tx, y, g_crValue, HS_EDIT, g_bUnsignedHex ? _T( "0x%IX" ) : _T( "%u" ), *Data ) + g_FontWidth;
	tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
	return DrawSize;
}
