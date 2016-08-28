#include "stdafx.h"
#include "CNodeDouble.h"

CNodeDouble::CNodeDouble()
{
	m_nodeType = nt_double;
}

void CNodeDouble::Update(CHotSpot & Spot)
{
	StandardUpdate(Spot);
	double v = _ttof(Spot.Text.GetString());
	if (Spot.ID == 0)
		ReClassWriteMemory((LPVOID)Spot.Address, &v, 8);
}

int CNodeDouble::Draw(ViewInfo & View, int x, int y)
{
	if (m_bHidden)
		return DrawHidden(View, x, y);

	double* pMemory = (double*)&View.pData[m_Offset];

	AddSelection(View, 0, y, g_FontHeight);
	AddDelete(View, x, y);
	AddTypeDrop(View, x, y);
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon(View, tx, y, ICON_FLOAT, HS_NONE, HS_NONE);
	tx = AddAddressOffset(View, tx, y);
	tx = AddText(View, tx, y, crType, HS_NONE, _T("double "));
	tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), m_strName);
	tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
	//tx = AddText(View, tx, y, crValue, 0, "%.4lg", pMemory[0]) + FontWidth;
	tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%.4g"), pMemory[0]) + g_FontWidth;
	tx = AddComment(View, tx, y);

	return y += g_FontHeight;
}
