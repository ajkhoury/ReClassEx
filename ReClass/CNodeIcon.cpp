#include "stdafx.h"
#include "CNodeIcon.h"

NodeSize CNodeIcon::Draw( const ViewInfo& View, int x, int y )
{
	NodeSize drawnSize;

	for (UINT i = 0; i < 21; i++)
		x = AddIcon( View, x, y, i, -1, -1 );

	drawnSize.x = x;
	drawnSize.y = y + g_FontHeight;
	return drawnSize;
}

