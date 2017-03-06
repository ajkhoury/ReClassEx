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

NodeSize CNodeVTable::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	ULONG_PTR* pMemory = (ULONG_PTR*)&View.pData[m_Offset];
	NodeSize drawnSize = { 0 };
	NodeSize childDrawnSize = { 0 };
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
	drawnSize.x = x;
	drawnSize.y = y;

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
			CNodeFunctionPtr* pFunctionPtr = static_cast<CNodeFunctionPtr*>(m_ChildNodes[i]);
			pFunctionPtr->SetOffset( i * sizeof( size_t ) );
			childDrawnSize = pFunctionPtr->Draw( NewView, tx, y );
			drawnSize.y = childDrawnSize.y;
			if (childDrawnSize.x > drawnSize.x) {
				drawnSize.x = childDrawnSize.x;
			}

			y = drawnSize.y;
		}
	}
	else
	{
		for (UINT i = 0; i < m_ChildNodes.size( ); i++)
			static_cast<CNodeFunctionPtr*>(m_ChildNodes[i])->HideAssemblyWindow( );
	}

	return drawnSize;
}