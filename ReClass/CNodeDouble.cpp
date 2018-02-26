#include "stdafx.h"
#include "CNodeDouble.h"

CNodeDouble::CNodeDouble( )
{
    m_nodeType = nt_double;
}

void CNodeDouble::Update( const PHOTSPOT Spot )
{
    double DoubleValue;

    StandardUpdate( Spot );

    DoubleValue = _tcstod( Spot->Text.GetString( ), NULL );
    if (Spot->Id == 0)
        ReClassWriteMemory( (LPVOID)Spot->Address, &DoubleValue, sizeof( double ) );
}

NODESIZE CNodeDouble::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    double* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (double*)(View->Data + m_Offset);

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_FLOAT, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_clrType, HS_NONE, _T( "double " ) );
    tx = AddText( View, tx, y, g_clrName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_clrName, HS_NONE, _T( " = " ) );
    //tx = AddText(View, tx, y, g_clrValue, 0, "%.4lg", pMemory[0]) + FontWidth;
    tx = AddText( View, tx, y, g_clrValue, HS_EDIT, _T( "%.4g" ), *Data ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
