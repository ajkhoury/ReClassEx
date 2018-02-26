#include "stdafx.h"
#include "CNodeWCharPtr.h"

CNodeWCharPtr::CNodeWCharPtr( )
{
    m_nodeType = nt_pwchar;
    m_strName = "PWChar";
}

void CNodeWCharPtr::Update( const PHOTSPOT Spot )
{
    uintptr_t PointerValue;

    StandardUpdate( Spot );

    PointerValue = (uintptr_t)_ttoi64( Spot->Text.GetString( ) );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &PointerValue, sizeof( uintptr_t ) );
}

NODESIZE CNodeWCharPtr::Draw( const PVIEWINFO View, int x, int y )
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
    tx = AddText( View, tx, y, g_clrType, HS_NONE, _T( "PWCHAR " ) );
    tx = AddText( View, tx, y, g_clrName, HS_NAME, m_strName );

    //tx = AddText(View,tx,y,g_clrName,HS_NONE," = ");
    //tx = AddText(View,tx,y,g_clrValue,0,"%lli",pMemory[0]) + FontWidth;
    //tx = AddComment(View,tx,y);

    /*
    tx = x + 16;
    tx = AddIcon(View,tx,y,ICON_TEXT,HS_NONE,HS_NONE);
    tx = AddAddressOffset(View,tx,y);
    tx = AddText(View,tx,y,g_clrType,HS_NONE,"Text ");
    tx = AddText(View,tx,y,g_clrName,69,"%s",m_strName);
    tx = AddText(View,tx,y,g_clrIndex,HS_NONE,"[");
    tx = AddText(View,tx,y,g_clrIndex,0,"%i",memsize);
    tx = AddText(View,tx,y,g_clrIndex,HS_NONE,"]");
    */

    tx = AddText( View, tx, y, g_clrChar, HS_NONE, _T( " = '" ) );
    if (VALID( Data ))
    {
        CStringW MemoryString = ReadMemoryStringW( *Data, 128 );
        tx = AddText( View, tx, y, g_clrChar, 1, "%ls", MemoryString.GetBuffer( ) );
    }

    tx = AddText( View, tx, y, g_clrChar, HS_NONE, _T( "' " ) ) + g_FontWidth;
    tx = AddComment( View, tx, y );
    
    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
