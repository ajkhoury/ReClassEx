#include "stdafx.h"
#include "CNodeFloat.h"

CNodeFloat::CNodeFloat( )
{
    m_nodeType = nt_float;
}

void CNodeFloat::Update( const PHOTSPOT Spot )
{
    float FloatValue;

    StandardUpdate( Spot );

    FloatValue = (float)_ttof( Spot->Text.GetString( ) );
    if (Spot->Id == HS_EDIT)
        ReClassWriteMemory( (LPVOID)Spot->Address, &FloatValue, sizeof( float ) );
}

NODESIZE CNodeFloat::Draw( const PVIEWINFO View, int x, int y )
{
    int tx;
    NODESIZE DrawSize;
    float* Data;

    if (m_bHidden)
        return DrawHidden( View, x, y );

    Data = (float*)(View->Data + m_Offset);
    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_FLOAT, HS_NONE, HS_NONE );
    tx = AddAddressOffset( View, tx, y );
    tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "float " ) );
    tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    tx = AddText( View, tx, y, g_crName, HS_NONE, _T( " = " ) );
    //tx = AddText(View,tx,y,g_crValue,0,"%.4f",pMemory[0]) + FontWidth;

    //if ( *pMemory > -99999.0f && *pMemory < 99999.0f )
    //	*pMemory = 0;

    tx = AddText( View, tx, y, g_crValue, HS_EDIT, _T( "%4.3f" ), *Data ) + g_FontWidth;
    tx = AddComment( View, tx, y );

    DrawSize.x = tx;
    DrawSize.y = y + g_FontHeight;
    return DrawSize;
}
