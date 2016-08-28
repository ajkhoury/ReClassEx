#include "stdafx.h"
#include "CNodeHex8.h"

CNodeHex8::CNodeHex8()
{
	m_nodeType = nt_hex8;
}

void CNodeHex8::Update(CHotSpot & Spot)
{
	StandardUpdate(Spot);
	unsigned char v = (unsigned char)(_tcstoul(Spot.Text.GetString(), NULL, 16) & 0xFF);
	if (Spot.ID == 0)
		ReClassWriteMemory((LPVOID)Spot.Address, &v, 1);
}

int CNodeHex8::Draw(ViewInfo & View, int x, int y)
{
	if (m_bHidden)
		return DrawHidden(View, x, y);

	unsigned char* pMemory = (unsigned char*)&((unsigned char*)View.pData)[m_Offset];
	AddSelection(View, 0, y, g_FontHeight);
	AddDelete(View, x, y);
	AddTypeDrop(View, x, y);
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET + 16;
	tx = AddAddressOffset(View, tx, y);

	if (gbText)
	{
		CStringA str = GetStringFromMemoryA((char*)pMemory, 1);
		str += "        ";
		tx = AddText(View, tx, y, crChar, HS_NONE, "%s", str.GetBuffer());
	}

	tx = AddText(View, tx, y, crHex, 0, _T("%0.2X"), pMemory[0] & 0xFF) + g_FontWidth;
	tx = AddComment(View, tx, y);

	return y += g_FontHeight;
}