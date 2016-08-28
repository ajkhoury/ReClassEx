#include "stdafx.h"
#include "CNodeIcon.h"

int CNodeIcon::Draw(ViewInfo & View, int x, int y)
{
	for (UINT i = 0; i < 21; i++)
		x = AddIcon(View, x, y, i, -1, -1);
	return y += g_FontHeight;
}

