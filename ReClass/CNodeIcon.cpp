#include "stdafx.h"
#include "CNodeIcon.h"

NODESIZE CNodeIcon::Draw( const PVIEWINFO View, int x, int y )
{
    NODESIZE DrawSize;

	for (UINT i = 0; i < 21; i++)
		x = AddIcon( View, x, y, i, -1, -1 );

    DrawSize.x = x;
    DrawSize.y = y + g_FontHeight;

	return DrawSize;
}

