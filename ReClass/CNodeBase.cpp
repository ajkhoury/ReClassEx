#include "stdafx.h"

#include "CNodeBase.h"

CNodeBase::CNodeBase() : 
	m_nodeType(nt_base), 
	m_pParentNode(nullptr),
	m_Offset(0),
	m_strOffset(_T("0")),
	m_bHidden(false),
	m_bSelected(false)
{
	// Optimized
	m_LevelsOpen.resize(32, false);
	m_LevelsOpen[0] = true;

	// This is the class name
	m_strName.Format(_T("N%0.8X"), NodeCreateIndex);

	NodeCreateIndex++;
}

// Incorrect view.address
void CNodeBase::AddHotSpot(ViewInfo& View, CRect& Spot, CString Text, int ID, int Type)
{
	if (Spot.top > View.client->bottom || Spot.bottom < 0)
		return;

	CHotSpot spot;
	spot.Rect = Spot;
	spot.Text = Text.GetString();
	spot.Address = View.Address + m_Offset;
	spot.ID = ID;
	spot.Type = Type;
	spot.object = this;
	spot.Level = View.Level;
	View.HotSpots->push_back(spot);
}

int CNodeBase::AddText(ViewInfo& View, int x, int y, DWORD color, int HitID, const wchar_t *fmt, ...)
{
	if (fmt == NULL)
		return x;

	wchar_t wcsbuf[1024];

	va_list va_alist;
	va_start(va_alist, fmt);
	_vsnwprintf(wcsbuf, ARRAYSIZE(wcsbuf), fmt, va_alist);
	va_end(va_alist);

	int width = (int)(wcslen(wcsbuf)) * g_FontWidth;

	if ((y >= -(g_FontHeight)) && (y + g_FontHeight <= View.client->bottom + g_FontHeight))
	{
		CRect pos;
		if (HitID != HS_NONE)
		{
			if (width >= g_FontWidth * 2)
				pos.SetRect(x, y, x + width, y + g_FontHeight);
			else
				pos.SetRect(x, y, x + g_FontWidth * 2, y + g_FontHeight);

			AddHotSpot(View, pos, wcsbuf, HitID, HS_EDIT);
		}
		pos.SetRect(x, y, 0, 0);
		View.dc->SetTextColor(color);
		View.dc->SetBkMode(TRANSPARENT);
		View.dc->DrawText(wcsbuf, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
	}

	return x + width;
}

int CNodeBase::AddText(ViewInfo& View, int x, int y, DWORD color, int HitID, const char* fmt, ...)
{
	char buffer[1024] = { 0 };
	TCHAR finalBuffer[1024] = { 0 };

	va_list va_alist;
	size_t converted;

	if (fmt == NULL)
		return x;

	va_start(va_alist, fmt);
	_vsnprintf_s(buffer, 1024, fmt, va_alist);
	va_end(va_alist);

#ifdef UNICODE
	mbstowcs_s(&converted, finalBuffer, buffer, 1024);
#else
	memcpy(&finalBuffer, buffer, 1024);
#endif

	int width = static_cast<int>(strlen(buffer)) * g_FontWidth;

	if ((y >= -g_FontHeight) && (y + g_FontHeight <= View.client->bottom + g_FontHeight))
	{
		CRect pos;
		if (HitID != HS_NONE)
		{
			if (width >= g_FontWidth * 2)
				pos.SetRect(x, y, x + width, y + g_FontHeight);
			else
				pos.SetRect(x, y, x + g_FontWidth * 2, y + g_FontHeight);

			AddHotSpot(View, pos, finalBuffer, HitID, HS_EDIT);
		}

		pos.SetRect(x, y, 0, 0);
		View.dc->SetTextColor(color);
		View.dc->SetBkMode(TRANSPARENT);
		View.dc->DrawText(finalBuffer, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
	}

	return x + width;
}

int CNodeBase::AddAddressOffset(ViewInfo& View, int x, int y)
{
	if (gbOffset)
	{
#ifdef _WIN64
		// goto 722
		// just the left side 0000
		// TODO: fix the ghetto rig FontWidth * x
		// where x = characters over 8
		//x += FontWidth; // we need this either way
		//int numdigits = Utils::NumDigits(View.Address);
		//if (numdigits < 8 && numdigits > 4)
		//	x -= ((8 - numdigits) * FontWidth);
		//if (numdigits > 8)
		//	x += ((numdigits - 8) * FontWidth);

		x = AddText(View, x, y, crOffset, HS_NONE, _T("%0.4X"), m_Offset) + g_FontWidth;
#else
		x = AddText(View, x, y, crOffset, HS_NONE, _T("%0.4X"), m_Offset) + g_FontWidth;
#endif
	}

	if (gbAddress)
	{
#ifdef _WIN64
		x = AddText(View, x, y, crAddress, HS_ADDRESS, _T("%0.9I64X"), View.Address + m_Offset) + g_FontWidth;
#else
		x = AddText(View, x, y, crAddress, HS_ADDRESS, _T("%0.8X"), View.Address + m_Offset) + g_FontWidth;
#endif
	}

	return x;
}

void CNodeBase::AddSelection(ViewInfo& View, int x, int y, int Height)
{
	if ((y > View.client->bottom) || (y + Height < 0))
		return;

	if (m_bSelected)
		View.dc->FillSolidRect(0, y, View.client->right, Height, crSelect);

	CRect pos(0, y, 1024, y + Height);
	AddHotSpot(View, pos, CString(), 0, HS_SELECT);
}

int CNodeBase::AddIcon(ViewInfo& View, int x, int y, int idx, int ID, int Type)
{
	if ((y > View.client->bottom) || (y + 16 < 0))
		return x + 16;

	DrawIconEx(View.dc->m_hDC, x, y, Icons[idx], 16, 16, 0, NULL, DI_NORMAL);

	if (ID != -1)
	{
		CRect pos(x, y, x + 16, y + 16);
		AddHotSpot(View, pos, CString(), ID, Type);
	}

	return x + 16;
}

int CNodeBase::AddOpenClose(ViewInfo& View, int x, int y)
{
	if ((y > View.client->bottom) || (y + 16 < 0))
		return x + 16;
	return m_LevelsOpen[View.Level] ? AddIcon(View, x, y, ICON_OPEN, 0, HS_OPENCLOSE) : AddIcon(View, x, y, ICON_CLOSED, 0, HS_OPENCLOSE);
}

void CNodeBase::AddDelete(ViewInfo& View, int x, int y)
{
	if ((y > View.client->bottom) || (y + 16 < 0))
		return;

	if (m_bSelected)
		AddIcon(View, View.client->right - 16, y, ICON_DELETE, 0, HS_DELETE);
}

//void CNodeBase::AddAdd(ViewInfo& View,int x,int y)
//{
//	if ( (y > View.client->bottom) || (y+16 < 0) ) return;
//	if (m_bSelected)AddIcon(View,16,y,ICON_ADD,HS_NONE,HS_NONE);
//}

void CNodeBase::AddTypeDrop(ViewInfo& View, int x, int y)
{
	if (View.bMultiSelected || ((y > View.client->bottom) || (y + 16 < 0)))
		return;

	if (m_bSelected)
		AddIcon(View, 0, y, ICON_DROPARROW, 0, HS_DROP);
}

int CNodeBase::ResolveRTTI(size_t Val, int &x, ViewInfo& View, int y)
{
#ifdef _WIN64
	size_t ModuleBase = 0x0;
	//Find module Val is in, then get module base
	for (int i = 0; i < MemMapModule.size(); i++)
	{
		MemMapInfo MemInfo = MemMapModule[i];
		if (Val >= MemInfo.Start && Val <= MemInfo.End)
		{
			ModuleBase = MemInfo.Start;
			break;
		}
	}

	size_t pRTTIObjectLocator = Val - 8; //Val is Ptr to first VFunc, pRTTI is at -0x8
	if (!IsValidPtr(pRTTIObjectLocator))
		return x;

	size_t RTTIObjectLocator;
	ReClassReadMemory((LPVOID)pRTTIObjectLocator, &RTTIObjectLocator, sizeof(DWORD_PTR));

	DWORD dwTypeDescriptorOffset;
	ReClassReadMemory((LPVOID)(RTTIObjectLocator + 0x0C), &dwTypeDescriptorOffset, sizeof(DWORD));
	size_t TypeDescriptor = ModuleBase + dwTypeDescriptorOffset;

	DWORD dwObjectBaseOffset;
	ReClassReadMemory((LPVOID)(RTTIObjectLocator + 0x14), &dwObjectBaseOffset, sizeof(DWORD));
	size_t ObjectBase = ModuleBase + dwObjectBaseOffset;


	DWORD dwClassHierarchyDescriptorOffset;
	ReClassReadMemory((LPVOID)(RTTIObjectLocator + 0x10), &dwClassHierarchyDescriptorOffset, sizeof(DWORD));

	//Offsets are from base
	size_t ClassHierarchyDescriptor = ModuleBase + dwClassHierarchyDescriptorOffset;
	if (!IsValidPtr(ClassHierarchyDescriptor) || !dwClassHierarchyDescriptorOffset)
		return x;

	DWORD NumBaseClasses;
	ReClassReadMemory((LPVOID)(ClassHierarchyDescriptor + 0x8), &NumBaseClasses, sizeof(DWORD));
	if (NumBaseClasses < 0 || NumBaseClasses > 25)
		NumBaseClasses = 0;

	DWORD BaseClassArrayOffset;
	ReClassReadMemory((LPVOID)(ClassHierarchyDescriptor + 0xC), &BaseClassArrayOffset, sizeof(DWORD));

	size_t BaseClassArray = ModuleBase + BaseClassArrayOffset;
	if (!IsValidPtr(BaseClassArray) || !BaseClassArrayOffset)
		return x;

	//x = AddText(View, x, y, crOffset, HS_NONE, " RTTI:");
	CString RTTIString;
	for (unsigned int i = 0; i < NumBaseClasses; i++)
	{
		if (i != 0 && i != NumBaseClasses)
		{
			RTTIString += _T(" : ");
			//x = AddText(View, x, y, crOffset, HS_NONE, " inherits:");
		}

		DWORD BaseClassDescriptorOffset;
		ReClassReadMemory((LPVOID)(BaseClassArray + (0x4 * i)), &BaseClassDescriptorOffset, sizeof(DWORD));

		size_t BaseClassDescriptor = ModuleBase + BaseClassDescriptorOffset;
		if (!IsValidPtr(BaseClassDescriptor) || !BaseClassDescriptorOffset)
			continue;

		DWORD TypeDescriptorOffset;
		ReClassReadMemory((LPVOID)BaseClassDescriptor, &TypeDescriptorOffset, sizeof(DWORD));

		size_t TypeDescriptor = ModuleBase + TypeDescriptorOffset;
		if (!IsValidPtr(TypeDescriptor) || !TypeDescriptorOffset)
			continue;

		CString RTTIName;
		bool FoundEnd = false;
		char LastChar = ' ';
		for (int j = 1; j < 45; j++)
		{
			char RTTINameChar;
			ReClassReadMemory((LPVOID)(TypeDescriptor + 0x10 + j), &RTTINameChar, 1);
			if (RTTINameChar == '@' && LastChar == '@') //Names seem to be ended with @@
			{
				FoundEnd = true;
				RTTIName += RTTINameChar;
				break;
			}
			RTTIName += RTTINameChar;
			LastChar = RTTINameChar;
		}
		//Did we find a valid rtti name or did we just reach end of loop
		if (!FoundEnd)
			continue;

		TCHAR Demangled[MAX_PATH];
		if (_UnDecorateSymbolName(RTTIName, Demangled, MAX_PATH, UNDNAME_NAME_ONLY) == 0)
			RTTIString += RTTIName;
		else
			RTTIString += Demangled;
		//x = AddText(View, x, y, crOffset, HS_RTTI, "%s", RTTIName.c_str());
	}
	x = AddText(View, x, y, crOffset, HS_RTTI, RTTIString);
	return x;
#else	
	size_t pRTTIObjectLocator = Val - 4;
	if (!IsValidPtr(pRTTIObjectLocator))
		return x;

	size_t RTTIObjectLocator;
	ReClassReadMemory((LPVOID)pRTTIObjectLocator, &RTTIObjectLocator, sizeof(size_t));

	size_t pClassHierarchyDescriptor = RTTIObjectLocator + 0x10;
	if (!IsValidPtr(pClassHierarchyDescriptor))
		return x;

	size_t ClassHierarchyDescriptor;
	ReClassReadMemory((LPVOID)pClassHierarchyDescriptor, &ClassHierarchyDescriptor, sizeof(size_t));

	int NumBaseClasses;
	ReClassReadMemory((LPVOID)(ClassHierarchyDescriptor + 0x8), &NumBaseClasses, sizeof(int));
	if (NumBaseClasses < 0 || NumBaseClasses > 25)
		NumBaseClasses = 0;

	size_t pBaseClassArray = ClassHierarchyDescriptor + 0xC;
	if (!IsValidPtr(pBaseClassArray))
		return x;

	size_t BaseClassArray;
	ReClassReadMemory((LPVOID)pBaseClassArray, &BaseClassArray, sizeof(size_t));

	//x = AddText(View, x, y, crOffset, HS_NONE, " RTTI: ");
	CString RTTIString;
	for (int i = 0; i < NumBaseClasses; i++)
	{
		if (i != 0 && i != NumBaseClasses)
		{
			RTTIString += " : ";
			//x = AddText(View, x, y, crOffset, HS_RTTI, " : ");
		}

		size_t pBaseClassDescriptor = BaseClassArray + (4 * i);
		if (!IsValidPtr(pBaseClassDescriptor))
			continue;

		size_t BaseClassDescriptor;
		ReClassReadMemory((LPVOID)pBaseClassDescriptor, &BaseClassDescriptor, sizeof(size_t));

		if (!IsValidPtr(BaseClassDescriptor))
			continue;

		size_t TypeDescriptor; //pointer at 0x00 in BaseClassDescriptor
		ReClassReadMemory((LPVOID)BaseClassDescriptor, &TypeDescriptor, sizeof(size_t));

		CString RTTIName;
		bool FoundEnd = false;
		char LastChar = ' ';
		for (int j = 1; j < 45; j++)
		{
			char RTTINameChar;
			ReClassReadMemory((LPVOID)(TypeDescriptor + 0x08 + j), &RTTINameChar, 1);
			if (RTTINameChar == '@' && LastChar == '@') // Names seem to be ended with @@
			{
				FoundEnd = true;
				RTTIName += RTTINameChar;
				break;
			}

			RTTIName += RTTINameChar;
			LastChar = RTTINameChar;
		}
		//Did we find a valid rtti name or did we just reach end of loop
		if (!FoundEnd)
			continue;

		TCHAR Demangled[MAX_PATH];
		if (_UnDecorateSymbolName(RTTIName, Demangled, MAX_PATH, UNDNAME_NAME_ONLY) == 0)
			RTTIString += RTTIName;
		else
			RTTIString += Demangled;
		//x = AddText(View, x, y, crOffset, HS_RTTI, "%s", RTTIName.c_str());
	}

	x = AddText(View, x, y, crOffset, HS_RTTI, _T("%s"), RTTIString);
	return x;
#endif
}

int CNodeBase::AddComment(ViewInfo& View, int x, int y)
{
	x = AddText(View, x, y, crComment, HS_NONE, _T("//"));
	// Need the extra whitespace in "%s " after the %s to edit.
	x = AddText(View, x, y, crComment, HS_COMMENT, _T("%s "), m_strComment);

	if (m_nodeType == nt_hex64)
	{
		float flVal = *((float*)&View.pData[m_Offset]);
		LONG_PTR intVal = *((LONG_PTR*)&View.pData[m_Offset]);

		if (gbFloat)
		{
			if (flVal > -99999.0 && flVal < 99999.0)
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%0.3f)"), flVal);
			else
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%0.3f)"), 0.0f);
		}

		if (gbInt)
		{
			if (intVal > 0x6FFFFFFF && intVal < 0x7FFFFFFFFFFF)
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%I64d|0x%IX)"), intVal, intVal);
			else if (intVal == 0)
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%I64d)"), intVal);
			else
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%I64d|0x%X)"), intVal, intVal);
		}

		// *** this is probably broken, let's fix it after
		size_t uintVal = (size_t)intVal;
		CString a(GetAddressName(uintVal, false));
		if (a.GetLength() > 0)
		{
			if (gbPointers)
			{
				//printf( "<%p> here\n", Val );
				if (uintVal > 0x6FFFFFFF && uintVal < 0x7FFFFFFFFFFF)
				{
					x = AddText(View, x, y, crOffset, HS_EDIT, _T("*->%s "), a);
					if (gbRTTI)
						x = ResolveRTTI(uintVal, x, View, y);

					// Print out info from PDB at address
					if (gbSymbolResolution)
					{
						CString moduleName = GetModuleName(uintVal);
						if (!moduleName.IsEmpty())
						{
							SymbolReader* symbols = g_SymLoader->GetSymbolsForModule(moduleName);
							if (symbols)
							{
								CString nameOut;
								if (symbols->GetSymbolStringWithVA(uintVal, nameOut))
								{
									x = AddText(View, x, y, crOffset, HS_EDIT, _T("%s "), nameOut);
								}
							}
						}
					}
				}
			}

			if (gbString)
			{
				bool bAddStr = true;
				char txt[64];
				ReClassReadMemory((LPVOID)uintVal, txt, 64);

				for (int i = 0; i < 8; i++)
				{
					if (!isprint((unsigned char)txt[i]))
						bAddStr = false;
				}

				if (bAddStr)
				{
					txt[63] = '\0';
					x = AddText(View, x, y, crChar, HS_NONE, _T("'%hs'"), txt);
				}
			}
		}
	}
	else if (m_nodeType == nt_hex32)
	{
		float flVal = *((float*)&View.pData[m_Offset]);
		int intVal = *((int*)&View.pData[m_Offset]);

		if (gbFloat)
		{
			if (flVal > -99999.0 && flVal < 99999.0)
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%0.3f)"), flVal);
			else
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%0.3f)"), 0.0f);
		}

		if (gbInt)
		{
#ifdef _WIN64
			if (intVal > 0x140000000 && intVal < 0x7FFFFFFFFFFF) // in 64 bit address range
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%i|0x%IX)"), intVal, intVal);
			else if (intVal > 0x400000 && intVal < 0x140000000)
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%i|0x%X)"), intVal, intVal);
			else if (intVal == 0)
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%i)"), intVal);
			else
				x = AddText(View, x, y, crValue, HS_NONE, _T("(%i|0x%X)"), intVal, intVal);
#else
			x = (intVal == 0) ? AddText(View, x, y, crValue, HS_NONE, _T("(%i)"), intVal) : AddText(View, x, y, crValue, HS_NONE, _T("(%i|0x%X)"), intVal, intVal);
#endif
		}

		// *** this is probably broken, let's fix it after
		size_t uintVal = (size_t)intVal;
		CString addressStr(GetAddressName(uintVal, false));
		if (addressStr.GetLength() > 0)
		{
			if (gbPointers)
			{
				//#ifdef _WIN64
				// If set max to 0x140000000 a bunch of invalid pointers come up
				// Set to 0x110000000 instead
				if (uintVal > 0x400000 && uintVal < 0x110000000)
				{
					x = AddText(View, x, y, crOffset, HS_EDIT, _T("*->%s "), addressStr);
					if (gbRTTI)
						x = ResolveRTTI(uintVal, x, View, y);

					// Print out info from PDB at address
					if (gbSymbolResolution)
					{
						CString moduleName = GetModuleName(uintVal);
						if (!moduleName.IsEmpty())
						{
							SymbolReader* symbols = g_SymLoader->GetSymbolsForModule(moduleName);
							if (symbols)
							{
								CString SymbolOut;
								SymbolOut.Preallocate(1024);
								if (symbols->GetSymbolStringWithVA(uintVal, SymbolOut))
								{
									x = AddText(View, x, y, crOffset, HS_EDIT, _T("%s "), SymbolOut.GetString());
								}
							}
						}
					}

				}
			}

			if (gbString)
			{
				bool bAddStr = true;
				char txt[64];
				ReClassReadMemory((LPVOID)uintVal, txt, 64); // TODO: find out why it looks wrong

				for (int i = 0; i < 4; i++)
				{
					if (!isprint((unsigned char)txt[i]))
						bAddStr = false;
				}

				if (bAddStr)
				{
					txt[63] = '\0'; // null terminte (even though we prolly dont have to)
					x = AddText(View, x, y, crChar, HS_NONE, _T("'%hs'"), txt);
				}
			}
		}
	}

	return x;
}

void CNodeBase::StandardUpdate(CHotSpot &Spot)
{
	if (Spot.ID == HS_NAME)
		m_strName = Spot.Text;
	else if (Spot.ID == HS_COMMENT)
		m_strComment = Spot.Text;
}

int CNodeBase::DrawHidden(ViewInfo& View, int x, int y)
{
	if (m_bSelected)
		View.dc->FillSolidRect(0, y, View.client->right, 1, crSelect);
	else
		View.dc->FillSolidRect(0, y, View.client->right, 1, crHidden);
	return y + 1;
}