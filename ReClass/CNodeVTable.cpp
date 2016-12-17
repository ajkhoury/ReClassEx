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

	size_t* pMemory = (size_t*)&View.pData[m_Offset];
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	x = AddOpenClose( View, x, y );
	x = AddIcon( View, x, y, ICON_VTABLE, -1, -1 );

	int tx = x;
	x = AddAddressOffset( View, x, y );
	x = AddText( View, x, y, g_crVTable, HS_NONE, _T( "VTable[%i]" ), Nodes.size( ) ) + g_FontWidth;

	//if (m_strName.IsEmpty())
	x = AddText( View, x, y, g_crName, HS_NAME, _T( "%s" ), m_strName ) + g_FontWidth;
	//else
	//	x = AddText(View, x, y, g_crName, HS_NONE, _T("%s_vtable"), pParent->m_strName) + FontWidth;

	x = AddComment( View, x, y );

	y += g_FontHeight;
	if (m_LevelsOpen[View.Level])
	{
		// vtable stuff
		DWORD NeededSize = (int)Nodes.size( ) * sizeof( size_t );

		Memory.SetSize( NeededSize );
		ViewInfo newView;
		newView = View;
		newView.pData = Memory.Data( );

		newView.Address = pMemory[0];
		ReClassReadMemory( (LPVOID)newView.Address, newView.pData, NeededSize );

		for (UINT i = 0; i < Nodes.size( ); i++)
		{
			Nodes[i]->SetOffset( i * sizeof( size_t ) );
			y = Nodes[i]->Draw( newView, tx, y );
		}
	}

	return y;
}