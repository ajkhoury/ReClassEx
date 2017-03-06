#include "stdafx.h"
#include "CNodePtr.h"

CNodePtr::CNodePtr( ) : m_pNode( NULL )
{
	m_nodeType = nt_pointer;
}

void CNodePtr::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
}

NodeSize CNodePtr::Draw( const ViewInfo& View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	NodeSize drawnSize = { 0 };
	NodeSize childDrawnSize;
	ULONG_PTR* pMemory = (ULONG_PTR*)&View.pData[m_Offset];

	//printf( "read ptr: %p\n", View.pData );
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	x = AddOpenClose( View, x, y );
	x = AddIcon( View, x, y, ICON_POINTER, -1, -1 );

	int tx = x;
	tx = AddAddressOffset( View, tx, y );
	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Ptr " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( " <%s>" ), m_pNode->GetName( ) );
	tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_A );

	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	y += g_FontHeight;
	drawnSize.x = tx;
	if (m_LevelsOpen[View.Level])
	{
		DWORD NeededSize = m_pNode->GetMemorySize( );
		m_Memory.SetSize( NeededSize );

		ViewInfo newView;
		newView = View;
		newView.pData = m_Memory.Data( );
		newView.Address = pMemory[0];

		ReClassReadMemory( (LPVOID)newView.Address, (LPVOID)newView.pData, NeededSize );

		childDrawnSize = m_pNode->Draw( newView, x, y );
		y = childDrawnSize.y;
		if (childDrawnSize.x > drawnSize.x) {
			drawnSize.x = childDrawnSize.x;
		}

	}

	drawnSize.y = y;
	return drawnSize;
}