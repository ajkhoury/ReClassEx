#include "stdafx.h"
#include "CNodeMatrix.h"

CNodeMatrix::CNodeMatrix( )
{
    m_nodeType = nt_matrix;
}

void CNodeMatrix::Update( const PHOTSPOT Spot )
{
    float FloatValue;

    StandardUpdate( Spot );

    if (Spot->Id < 16)
    {
        FloatValue = (float)_ttof( Spot->Text.GetString( ) );
        ReClassWriteMemory( (LPVOID)(Spot->Address + (Spot->Id * sizeof( float ))), &FloatValue, sizeof( float ) );
    }
}

NODESIZE CNodeMatrix::Draw( const PVIEWINFO View, int x, int y )
{
    int tx, mx;
    NODESIZE DrawSize;
    float* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (float*)(View->Data + m_Offset);
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_MATRIX, HS_NONE, HS_NONE );
    mx = tx;
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Matrix " ) );
    tx = AddText( View, tx, y, g_crName, 69, _T( "%s" ), m_strName );
    tx = AddOpenClose( View, tx, y );
    tx += g_FontWidth;
    tx = AddComment( View, tx, y );

    if (m_LevelsOpen[View->Level])
    {
        y += g_FontHeight;
        tx = mx;
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
        tx = AddText( View, tx, y, g_crValue, 0, _T( "% 14.3f" ), Data[0] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 1, _T( "% 14.3f" ), Data[1] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 2, _T( "% 14.3f" ), Data[2] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 3, _T( "% 14.3f" ), Data[3] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
        y += g_FontHeight;
        tx = mx;
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
        tx = AddText( View, tx, y, g_crValue, 4, _T( "% 14.3f" ), Data[4] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 5, _T( "% 14.3f" ), Data[5] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 6, _T( "% 14.3f" ), Data[6] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 7, _T( "% 14.3f" ), Data[7] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
        y += g_FontHeight;
        tx = mx;
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
        tx = AddText( View, tx, y, g_crValue, 8, _T( "% 14.3f" ), Data[8] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 9, _T( "% 14.3f" ), Data[9] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 10, _T( "% 14.3f" ), Data[10] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 11, _T( "% 14.3f" ), Data[11] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
        y += g_FontHeight;
        tx = mx;
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
        tx = AddText( View, tx, y, g_crValue, 12, _T( "% 14.3f" ), Data[12] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 13, _T( "% 14.3f" ), Data[13] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 14, _T( "% 14.3f" ), Data[14] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "," ) );
        tx = AddText( View, tx, y, g_crValue, 15, _T( "% 14.3f" ), Data[15] );
        tx = AddText( View, tx, y, g_crName, HS_NONE, _T( "|" ) );
    }

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
