#include "stdafx.h"
#include "CNodeText.h"

CNodeText::CNodeText( )
{
    m_nodeType = nt_text;
    m_strName = _T( "Text" );
    m_dwMemorySize = 16;
}

void CNodeText::Update( const PHOTSPOT Spot )
{
    SIZE_T Length;

    StandardUpdate( Spot );

    if (Spot->Id == 0)
    {
        m_dwMemorySize = _ttoi( Spot->Text.GetString( ) );
    }
    else if (Spot->Id == 1)
    {
        Length = Spot->Text.GetLength( ) + 1;
        if (Length > m_dwMemorySize)
            Length = m_dwMemorySize;
        ReClassWriteMemory( (LPVOID)Spot->Address, (LPVOID)Spot->Text.GetString( ), Length );
    }
}

NODESIZE CNodeText::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    char* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (char*)(View->Data + m_Offset);

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_TEXT, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Text " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
    tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), GetMemorySize( ) );
    tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

    if (VALID( Data ))
    {
        CStringA MemoryString( GetStringFromMemoryA( Data, GetMemorySize( ) ) );
        tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( " = '" ) );
        tx = AddText( View, tx, y, g_crChar, 1, "%.150s", MemoryString.GetBuffer( ) );
        tx = AddText( View, tx, y, g_crChar, HS_NONE, _T( "' " ) ) + g_FontWidth;
    }

    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
