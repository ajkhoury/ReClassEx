#include "stdafx.h"
#include "CNodeUnicode.h"

CNodeUnicode::CNodeUnicode()
{
	m_nodeType = nt_unicode;
	m_strName = _T("Unicode");
	memsize = 8 * sizeof(wchar_t);
}

void CNodeUnicode::Update(CHotSpot & Spot)
{
	StandardUpdate(Spot);
	if (Spot.ID == 0)
	{
		memsize = _ttoi(Spot.Text.GetBuffer()) * sizeof(wchar_t);
	}
	else if (Spot.ID == 1)
	{
		SIZE_T Length = Spot.Text.GetLength();
		if (Length > (memsize / sizeof(wchar_t)))
			Length = (memsize / sizeof(wchar_t));

		// Has to be done this way in order to make it compatible in mbs and unicode mode (ghetto)
		LPCTSTR pSource = Spot.Text.GetString();
		wchar_t* pwszConverted = new wchar_t[Length + 1];
		for (UINT i = 0; i <= Length; i++)
			pwszConverted[i] = (wchar_t)pSource[i];

		ReClassWriteMemory((LPVOID)Spot.Address, pwszConverted, Length);

		delete[] pwszConverted;
	}
}

int CNodeUnicode::GetMemorySize(void)
{
	return memsize;
}

int CNodeUnicode::Draw(ViewInfo & View, int x, int y)
{
	if (m_bHidden)
		return DrawHidden(View, x, y);

	wchar_t* pMemory = (wchar_t*)&((unsigned char*)View.pData)[m_Offset];
	AddSelection(View, 0, y, g_FontHeight);
	AddDelete(View, x, y);
	AddTypeDrop(View, x, y);
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon(View, tx, y, ICON_TEXT, HS_NONE, HS_NONE);
	tx = AddAddressOffset(View, tx, y);
	tx = AddText(View, tx, y, crType, HS_NONE, _T("Unicode "));
	tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), m_strName);
	tx = AddText(View, tx, y, crIndex, HS_NONE, _T("["));
	tx = AddText(View, tx, y, crIndex, HS_EDIT, _T("%i"), memsize / sizeof(wchar_t));
	tx = AddText(View, tx, y, crIndex, HS_NONE, _T("]"));

	if (VALID(pMemory))
	{
		CStringW str = GetStringFromMemoryW(pMemory, memsize / sizeof(wchar_t));
		tx = AddText(View, tx, y, crChar, HS_NONE, _T(" = '"));
		tx = AddText(View, tx, y, crChar, HS_OPENCLOSE, _T("%.150ls"), str); // ls cause its unicode
		tx = AddText(View, tx, y, crChar, HS_NONE, _T("' ")) + g_FontWidth;
	}

	tx = AddComment(View, tx, y);
	return y += g_FontHeight;
}
