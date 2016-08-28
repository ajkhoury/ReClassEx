#include "stdafx.h"
#include "CNodeHex16.h"

CNodeHex16::CNodeHex16()
{
	m_nodeType = nt_hex16;
}

void CNodeHex16::Update(CHotSpot & Spot)
{
	StandardUpdate(Spot);
	unsigned char v = (unsigned char)(_tcstoul(Spot.Text.GetString(), NULL, 16) & 0xFF);
	if (Spot.ID == 0)
		ReClassWriteMemory((LPVOID)Spot.Address, &v, 1);
	if (Spot.ID == 1)
		ReClassWriteMemory((LPVOID)(Spot.Address + 1), &v, 1);
}

int CNodeHex16::Draw(ViewInfo & View, int x, int y)
{
	if (m_bHidden)
		return DrawHidden(View, x, y);

	unsigned char* pMemory = (unsigned char*)&View.pData[m_Offset];
	AddSelection(View, 0, y, g_FontHeight);
	AddDelete(View, x, y);
	AddTypeDrop(View, x, y);
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET + 16;
	tx = AddAddressOffset(View, tx, y);

	if (gbText)
	{
		CStringA str = GetStringFromMemoryA((char*)pMemory, 2);
		str += "       ";
		tx = AddText(View, tx, y, crChar, HS_NONE, "%s", str.GetBuffer());
	}

	tx = AddText(View, tx, y, crHex, 0, _T("%0.2X"), pMemory[0] & 0xFF) + g_FontWidth;
	tx = AddText(View, tx, y, crHex, 1, _T("%0.2X"), pMemory[1] & 0xFF) + g_FontWidth;
	tx = AddComment(View, tx, y);

	return y += g_FontHeight;
}
