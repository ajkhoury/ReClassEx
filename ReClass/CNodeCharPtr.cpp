#include "stdafx.h"
#include "CNodeCharPtr.h"

CNodeCharPtr::CNodeCharPtr()
{
	m_nodeType = nt_pchar;
	m_strName = "PChar";
}

void CNodeCharPtr::Update(CHotSpot & Spot)
{
	StandardUpdate(Spot);
#ifdef _WIN64
	size_t ptr = _ttoi64(Spot.Text.GetString());
#else
	size_t ptr = _ttoi(Spot.Text.GetString());
#endif
	if (Spot.ID == 0)
		ReClassWriteMemory((LPVOID)Spot.Address, &ptr, sizeof(size_t));
}

int CNodeCharPtr::Draw(ViewInfo & View, int x, int y)
{
	if (m_bHidden)
		return DrawHidden(View, x, y);

	size_t* pMemory = (size_t*)&View.pData[m_Offset];

	AddSelection(View, 0, y, g_FontHeight);
	AddDelete(View, x, y);
	AddTypeDrop(View, x, y);
	//AddAdd(View, x, y);

	int tx = x + TXOFFSET;
	tx = AddIcon(View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
	tx = AddAddressOffset(View, tx, y);
	tx = AddText(View, tx, y, crType, HS_NONE, _T("PCHAR "));
	tx = AddText(View, tx, y, crName, HS_NAME, m_strName);

	//tx = AddText(View,tx,y,crName,HS_NONE," = ");
	//tx = AddText(View,tx,y,crValue,0,"%lli",pMemory[0]) + FontWidth;
	//tx = AddComment(View,tx,y);

	/*
	int tx = x + 16;
	tx = AddIcon(View,tx,y,ICON_TEXT,HS_NONE,HS_NONE);
	tx = AddAddressOffset(View,tx,y);
	tx = AddText(View,tx,y,crType,HS_NONE,"Text ");
	tx = AddText(View,tx,y,crName,69,"%s",m_strName);
	tx = AddText(View,tx,y,crIndex,HS_NONE,"[");
	tx = AddText(View,tx,y,crIndex,0,"%i",memsize);
	tx = AddText(View,tx,y,crIndex,HS_NONE,"]");
	*/

	tx = AddText(View, tx, y, crChar, HS_NONE, _T(" = '"));
	if (VALID(pMemory))
	{
		CStringA sc = ReadMemoryStringA((size_t)pMemory[0], 128);
		tx = AddText(View, tx, y, crChar, 1, "%s", sc.GetBuffer());
	}

	tx = AddText(View, tx, y, crChar, HS_NONE, _T("' ")) + g_FontWidth;
	tx = AddComment(View, tx, y);

	return y += g_FontHeight;
}
