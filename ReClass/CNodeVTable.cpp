#include "stdafx.h"
#include "CNodeVTable.h"

CNodeVTable::CNodeVTable( )
{
	m_nodeType = nt_vtable;
}

void CNodeVTable::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
}

int CNodeVTable::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	ULONG_PTR* pMemory = (ULONG_PTR*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	x = AddOpenClose( View, x, y );
	x = AddIcon( View, x, y, ICON_VTABLE, -1, -1 );

	int tx = x;
	x = AddAddressOffset( View, x, y );
	x = AddText( View, x, y, g_crVTable, HS_NONE, _T( "VTable[%i]" ), m_ChildNodes.size( ) ) + g_FontWidth;

	//if (m_strName.IsEmpty())
	x = AddText( View, x, y, g_crName, HS_NAME, _T( "%s" ), m_strName ) + g_FontWidth;
	//else
	//	x = AddText(View, x, y, g_crName, HS_NONE, _T("%s_vtable"), pParent->m_strName) + FontWidth;

	x = AddComment( View, x, y );

	y += g_FontHeight;
	if (m_LevelsOpen[View.Level])
	{
		ViewInfo NewView;
		DWORD NeededSize = (DWORD)m_ChildNodes.size( ) * sizeof( ULONG_PTR );

		m_Memory.SetSize( NeededSize );
		
		NewView = View;
		NewView.pData = m_Memory.Data( );
		NewView.Address = pMemory[0];

		ReClassReadMemory( (LPVOID)NewView.Address, NewView.pData, NeededSize );

		for (UINT i = 0; i < m_ChildNodes.size( ); i++)
		{
			m_ChildNodes[i]->SetOffset( i * sizeof( ULONG_PTR ) );
			y = m_ChildNodes[i]->Draw( NewView, tx, y );
		}
	}

	return y;
}