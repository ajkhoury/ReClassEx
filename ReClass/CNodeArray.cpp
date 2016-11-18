#include "stdafx.h"
#include "CNodeArray.h"

CNodeArray::CNodeArray( )
{
	m_nodeType = nt_array;
	Total = 1;
	Current = 0;
}

void CNodeArray::Update( HotSpot& Spot )
{
	StandardUpdate( Spot );
	int v = _ttoi( Spot.Text.GetString( ) );
	if (v < 0)
		return;

	if (Spot.ID == 0)
	{
		if (v == 0)
			return;
		Total = (DWORD)v;
	}
	else if (Spot.ID == 1)
	{
		if (v >= (int)Total)
			return;
		Current = (DWORD)v;
	}
	else if (Spot.ID == 2)
	{
		if (Current > 0)
			Current--;
	}
	else if (Spot.ID == 3)
	{
		if (Current < Total - 1)
			Current++;
	}
}

int CNodeArray::GetMemorySize( )
{
	return pNode->GetMemorySize( ) * Total;
}

int CNodeArray::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );
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
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), Total );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

	tx = AddIcon( View, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "(" ) );
	tx = AddText( View, tx, y, g_crIndex, 1, _T( "%i" ), Current );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( ")" ) );
	tx = AddIcon( View, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK );

	tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( "<%s Size=%i>" ), pNode->GetName( ), GetMemorySize( ) );
	tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X );

	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	y += g_FontHeight;
	if (m_LevelsOpen[View.Level])
	{
		ViewInfo newView;
		newView = View;
		newView.Address = View.Address + m_Offset + pNode->GetMemorySize( ) * Current;
		newView.pData = (unsigned char*)((size_t)View.pData + m_Offset + pNode->GetMemorySize( ) * Current);
		y = pNode->Draw( newView, x, y );
	};
	return y;
}
