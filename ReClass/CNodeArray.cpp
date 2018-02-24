#include "stdafx.h"
#include "CNodeArray.h"

CNodeArray::CNodeArray( )
{
	m_nodeType = nt_array;
    m_ulTotal = 1;
	m_iCurrent = 0;
}

void CNodeArray::Update( const PHOTSPOT Spot )
{
    LONG LongValue;
    ULONG UlongValue;

	StandardUpdate( Spot );

    LongValue = _ttol( Spot->Text.GetString( ) );
	if (LongValue < 0)
		return;

    UlongValue = (ULONG)LongValue;

	if (Spot->Id == 0)
	{
		if (UlongValue == 0)
			return;
		m_ulTotal = UlongValue;
	}
	else if (Spot->Id == 1)
	{
		if (UlongValue >= m_ulTotal)
			return;
		m_iCurrent = UlongValue;
	}
	else if (Spot->Id == 2)
	{
		if (m_iCurrent > 0)
			m_iCurrent--;
	}
	else if (Spot->Id == 3)
	{
		if (m_iCurrent < (INT)m_ulTotal - 1)
			m_iCurrent++;
	}
}

ULONG CNodeArray::GetMemorySize( )
{
	return m_pNode->GetMemorySize( ) * m_ulTotal;
}

NODESIZE CNodeArray::Draw( const PVIEWINFO View, int x, int y )
{
	NODESIZE DrawSize;
	NODESIZE ChildDrawSize;

	if (m_bHidden)
		return DrawHidden( View, x, y );

    DrawSize.x = 0;

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	x = AddOpenClose( View, x, y );
	x = AddIcon( View, x, y, ICON_ARRAY, -1, -1 );

	int tx = x;
	tx = AddAddressOffset( View, tx, y );

	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Array " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%u" ), m_ulTotal );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

	tx = AddIcon( View, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "(" ) );
	tx = AddText( View, tx, y, g_crIndex, 1, _T( "%i" ), m_iCurrent );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( ")" ) );
	tx = AddIcon( View, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK );

	tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( "<%s Size=%u>" ), m_pNode->GetName( ), GetMemorySize( ) );
	tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X );

	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	y += g_FontHeight;
	if (m_LevelsOpen[View->Level])
	{
		VIEWINFO NewView;
        memcpy( &NewView, View, sizeof( NewView ) );
		NewView.Address = View->Address + m_Offset + m_pNode->GetMemorySize( ) * m_iCurrent;
		NewView.Data = (UCHAR*)((uintptr_t)View->Data + m_Offset + m_pNode->GetMemorySize( ) * m_iCurrent);

        ChildDrawSize = m_pNode->Draw( &NewView, x, y );

		y = ChildDrawSize.y;
		if (ChildDrawSize.x > DrawSize.x)
        {
            DrawSize.x = ChildDrawSize.x;
		}
	}
	
    DrawSize.y = y;
	return DrawSize;
}
