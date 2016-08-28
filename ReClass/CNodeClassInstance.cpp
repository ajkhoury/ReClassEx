#include "stdafx.h"
#include "CNodeClassInstance.h"

CNodeClassInstance::CNodeClassInstance()
{
	m_nodeType = nt_instance;
}

void CNodeClassInstance::Update(CHotSpot & Spot)
{
	StandardUpdate(Spot);
}

int CNodeClassInstance::GetMemorySize()
{
	return pNode->GetMemorySize();
}

int CNodeClassInstance::Draw(ViewInfo & View, int x, int y)
{
	if (m_bHidden)
		return DrawHidden(View, x, y);

	AddSelection(View, 0, y, g_FontHeight);
	AddDelete(View, x, y);
	AddTypeDrop(View, x, y);

	x = AddOpenClose(View, x, y);
	x = AddIcon(View, x, y, ICON_CLASS, -1, -1);

	int tx = x;
	tx = AddAddressOffset(View, tx, y);

	tx = AddText(View, tx, y, crType, HS_NONE, _T("Instance "));
	tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), m_strName);
	tx = AddText(View, tx, y, crValue, HS_NONE, _T("<%s>"), pNode->GetName());
	tx = AddIcon(View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X);

	tx += g_FontWidth;
	tx = AddComment(View, tx, y);

	y += g_FontHeight;
	if (m_LevelsOpen[View.Level])
	{
		ViewInfo newView;
		newView = View;
		newView.Address = View.Address + m_Offset;
		newView.pData = (unsigned char*)((size_t)newView.pData + m_Offset);
		y = pNode->Draw(newView, x, y);
	}

	return y;
}
