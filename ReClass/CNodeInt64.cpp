#include "stdafx.h"
#include "CNodeInt64.h"

CNodeInt64::CNodeInt64()
{
	m_nodeType = nt_int64;
}

void CNodeInt64::Update(CHotSpot & Spot)
{
	StandardUpdate(Spot);
	__int64 v = _ttoi64(Spot.Text.GetString());
	if (Spot.ID == 0)
		ReClassWriteMemory((LPVOID)Spot.Address, &v, sizeof(__int64));
}

int CNodeInt64::Draw(ViewInfo & View, int x, int y)
{
	if (m_bHidden) 
		return DrawHidden(View, x, y);

	__int64 Int64 = *(__int64*)(&View.pData[m_Offset]);

	AddSelection(View, 0, y, g_FontHeight);
	AddDelete(View, x, y);
	AddTypeDrop(View, x, y);
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon(View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
	tx = AddAddressOffset(View, tx, y);
	tx = AddText(View, tx, y, crType, HS_NONE, _T("Int64 "));
	tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), m_strName.GetString());
	tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
	tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%lli"), Int64) + g_FontWidth;
	tx = AddComment(View, tx, y);

	return y += g_FontHeight;
}
