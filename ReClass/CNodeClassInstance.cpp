#include "stdafx.h"
#include "CNodeClassInstance.h"

CNodeClassInstance::CNodeClassInstance( ) : 
	m_pNode( NULL )
{
	m_nodeType = nt_instance;
}

void CNodeClassInstance::Update( HotSpot & Spot )
{
	StandardUpdate( Spot );
}

ULONG CNodeClassInstance::GetMemorySize( )
{
	return m_pNode->GetMemorySize( );
}

NodeSize CNodeClassInstance::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	NodeSize drawnSize;
	NodeSize childDrawnSize;
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	x = AddOpenClose( View, x, y );
	x = AddIcon( View, x, y, ICON_CLASS, -1, -1 );

	int tx = x;
	tx = AddAddressOffset( View, tx, y );

	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Instance " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( "<%s>" ), m_pNode->GetName( ) );
	tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X );

	tx += g_FontWidth;
	tx = AddComment( View, tx, y );

	y += g_FontHeight;
	if (m_LevelsOpen[View.Level])
	{
		ViewInfo NewView;
		NewView = View;
		NewView.Address = View.Address + m_Offset;
		NewView.pData = (UCHAR*)((ULONG_PTR)NewView.pData + m_Offset);
		childDrawnSize = m_pNode->Draw( NewView, x, y );
		y = childDrawnSize.y;
		if (childDrawnSize.x > drawnSize.x) {
			drawnSize.x = childDrawnSize.x;
		}
	}

	drawnSize.x = tx;
	drawnSize.y = y;
	return drawnSize;
}
