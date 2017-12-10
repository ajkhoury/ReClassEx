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
    int IntValue;

	StandardUpdate( Spot );

    IntValue = _ttoi( Spot->Text.GetString( ) );
	if (IntValue < 0)
		return;

	if (Spot->Id == 0)
	{
		if (IntValue == 0)
			return;
		m_ulTotal = (ULONG)IntValue;
	}
	else if (Spot->Id == 1)
	{
		if (IntValue >= m_ulTotal)
			return;
		m_iCurrent = IntValue;
	}
	else if (Spot->Id == 2)
	{
		if (m_iCurrent > 0)
			m_iCurrent--;
	}
	else if (Spot->Id == 3)
	{
		if ((ULONG)m_iCurrent < m_ulTotal - 1)
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
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), m_ulTotal );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

	tx = AddIcon( View, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "(" ) );
	tx = AddText( View, tx, y, g_crIndex, 1, _T( "%i" ), m_iCurrent );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( ")" ) );
	tx = AddIcon( View, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK );

	tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( "<%s Size=%i>" ), m_pNode->GetName( ), GetMemorySize( ) );
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
