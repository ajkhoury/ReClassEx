#pragma once

#include "Debug.h"
#include "bits.h"

#define TXOFFSET 16

extern DWORD NodeCreateIndex;

// forward declarations
class CMemory;
class CNodeBase;
class CNodeIcon;
class CNodeClass;
class CNodeHex64;
class CNodeHex32;
class CNodeHex16;
class CNodeHex8;
class CNodeBits;
class CNodeVTable;
class CNodeFunctionPtr;
class CNodePtr;
class CNodeInt64;
class CNodeInt32;
class CNodeInt8;
class CNodeDWORD;
class CNodeWORD;
class CNodeBYTE;
class CNodeText;
class CNodeCharPtr;
class CNodeUnicode;
class CNodeFloat;
class CNodeDouble;
class CNodeVec2;
class CNodeVec3;
class CNodeQuat;
class CNodeMatrix;
class CNodeArray;
class CNodeClassInstance;
class CNodeCustom;

#include "ChildFrm.h"

size_t ConvertStrToAddress(CString Address);

struct ViewInfo
{
	CDC* dc;
	CRect* client;
	std::vector<HotSpot>* HotSpots;
	std::vector<CNodeClass*>* Classes;
	size_t Address;
	void* pData;
	UINT Level;
	bool bMultiSelected;
};

class CNodeBase
{
public:
	CNodeBase()
	{
		offset = 0;
		strOffset = "0";
		bHidden = false;
		bOpen.push_back(true);

		for (int i = 0; i < 32; i++)
			bOpen.push_back(false);

		bSelected = false;

		// This is the class name
		Name.Format(_T("N%0.8X"), NodeCreateIndex);
		NodeCreateIndex++;
		pParent = NULL;
		nodeType = nt_base;
	}

	~CNodeBase() { }

	virtual int Draw(ViewInfo& View, int x, int y) = 0;
	virtual int GetMemorySize() = 0;
	virtual void Update(HotSpot& Spot) = 0;

	NodeType GetType() { return nodeType; }
	size_t GetOffset() { return offset; }

	NodeType nodeType;

	size_t offset;
	CString strOffset;

	CString Name;
	CString Comment;
	CNodeBase* pParent;
	std::vector<CNodeBase*> Nodes;

	bool bHidden;
	bool bSelected;
	std::vector<bool> bOpen;

	// Incorrect view.address
	void AddHotSpot(ViewInfo& View, CRect& Spot, CString Text, int ID, int Type)
	{
		if (Spot.top > View.client->bottom || Spot.bottom < 0) return;

		HotSpot spot;
		spot.Rect = Spot;
		spot.Text = Text;
		spot.Address = View.Address + offset;
		spot.ID = ID;
		spot.Type = Type;
		spot.object = this;
		spot.Level = View.Level;
		View.HotSpots->push_back(spot);
	}

	int AddText(ViewInfo& View, int x, int y, DWORD color, int HitID, const wchar_t *fmt, ...)
	{
		va_list va_alist;
		static wchar_t wcsbuf[1024];
		if (fmt == NULL)
			return x;

		va_start(va_alist, fmt);
		_vsnwprintf(wcsbuf, sizeof(wcsbuf), fmt, va_alist);
		va_end(va_alist);

		int width = static_cast<int>(wcslen(wcsbuf)) * FontWidth;

		if ((y >= -(FontHeight)) && (y + FontHeight <= View.client->bottom + FontHeight))
		{
			CRect pos;

			if (HitID != NONE)
			{
				if (width >= FontWidth * 2)
					pos.SetRect(x, y, x + width, y + FontHeight);
				else
					pos.SetRect(x, y, x + FontWidth * 2, y + FontHeight);

				AddHotSpot(View, pos, wcsbuf, HitID, HS_EDIT);
			}

			pos.SetRect(x, y, 0, 0);
			View.dc->SetTextColor(color);
			View.dc->SetBkMode(TRANSPARENT);
			View.dc->DrawText(wcsbuf, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
		}

		return x + width;
	}

	int AddText(ViewInfo& View, int x, int y, DWORD color, int HitID, const char* fmt, ...)
	{
		va_list va_alist;
		size_t converted;

		static char buffer[1024];
		static TCHAR finalBuffer[1024];

		if (fmt == NULL)
			return x;

		va_start(va_alist, fmt);
		_vsnprintf(buffer, 1024, fmt, va_alist);
		va_end(va_alist);

		#ifdef UNICODE
		mbstowcs_s(&converted, finalBuffer, buffer, 1024);
		#endif

		int width = static_cast<int>(_tcslen(finalBuffer)) * FontWidth;

		if ((y >= -FontHeight) && (y + FontHeight <= View.client->bottom + FontHeight))
		{
			CRect pos;
			if (HitID != NONE)
			{
				if (width >= FontWidth * 2)
					pos.SetRect(x, y, x + width, y + FontHeight);
				else
					pos.SetRect(x, y, x + FontWidth * 2, y + FontHeight);

				AddHotSpot(View, pos, CString(finalBuffer), HitID, HS_EDIT);
			} 

			pos.SetRect(x, y, 0, 0);
			View.dc->SetTextColor(color);
			View.dc->SetBkMode(TRANSPARENT);
			View.dc->DrawText(finalBuffer, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
		}

		return x + width;
	}

	int AddAddressOffset(ViewInfo& View, int x, int y)
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

			x = AddText(View, x, y, crOffset, NONE, _T("%0.4X"), offset) + FontWidth;
			#else
			x = AddText(View, x, y, crOffset, NONE, _T("%0.4X"), offset) + FontWidth;
			#endif
		}

		if (gbAddress)
		{
			#ifdef _WIN64
			x = AddText(View, x, y, crAddress, HS_ADDRESS, _T("%0.9I64X"), View.Address + offset) + FontWidth;
			#else
			x = AddText(View, x, y, crAddress, HS_ADDRESS, _T("%0.8X"), View.Address + offset) + FontWidth;
			#endif
		}

		return x;
	}

	void AddSelection(ViewInfo& View, int x, int y, int Height)
	{
		if ((y > View.client->bottom) || (y + Height < 0))
			return;

		if (bSelected)
			View.dc->FillSolidRect(0, y, View.client->right, Height, crSelect);

		CRect pos(0, y, 1024, y + Height);
		AddHotSpot(View, pos, CString(), 0, HS_SELECT);
	}

	int AddIcon(ViewInfo& View, int x, int y, int idx, int ID, int Type)
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

	int AddOpenClose(ViewInfo& View, int x, int y)
	{
		if ((y > View.client->bottom) || (y + 16 < 0))
			return x + 16;
		if (bOpen[View.Level])
			return AddIcon(View, x, y, ICON_OPEN, 0, HS_OPENCLOSE);
		else
			return AddIcon(View, x, y, ICON_CLOSED, 0, HS_OPENCLOSE);
	}

	void AddDelete(ViewInfo& View, int x, int y)
	{
		if ((y > View.client->bottom) || (y + 16 < 0))
			return;
		if (bSelected)
			AddIcon(View, View.client->right - 16, y, ICON_DELETE, 0, HS_DELETE);
	}

	//void AddAdd(ViewInfo& View,int x,int y)
	//{
	//	if ( (y > View.client->bottom) || (y+16 < 0) ) return;
	//	if (bSelected)AddIcon(View,16,y,ICON_ADD,NONE,NONE);
	//}

	void AddTypeDrop(ViewInfo& View, int x, int y)
	{
		if (View.bMultiSelected)
			return;
		if ((y > View.client->bottom) || (y + 16 < 0))
			return;

		if (bSelected)
			AddIcon(View, 0, y, ICON_DROPARROW, 0, HS_DROP);
	}

	//__int64 __resolvefbtype(__int64 lpFunc)
	//{
	//	__int64 vtable;
	//	ReadMemory(lpFunc, &vtable, 8);
	//
	//	if (vtable < 0x10000000)
	//		return 1;
	//
	//	// Get first function
	//	__int64 typeinfoptr;
	//	ReadMemory(vtable, &typeinfoptr, 8);
	//
	//	// Get relative offset to typeinfo string
	//	__int64 typeoffset;
	//	ReadMemory(typeinfoptr + 3, &typeoffset, 4);
	//
	//	typeoffset = typeinfoptr + typeoffset + 7;
	//
	//	__int64 TYPESTRING;
	//	ReadMemory(typeoffset, &TYPESTRING, 8);
	//
	//	printf("%I64X -> %I64X\n", typeoffset, TYPESTRING);
	//
	//	return TYPESTRING;
	//}

	int ResolveRTTI(size_t Val, int &x, ViewInfo& View, int y)
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
		ReadMemory(pRTTIObjectLocator, &RTTIObjectLocator, sizeof(DWORD_PTR));

		DWORD dwTypeDescriptorOffset;
		ReadMemory(RTTIObjectLocator + 0x0C, &dwTypeDescriptorOffset, sizeof(DWORD));
		size_t TypeDescriptor = ModuleBase + dwTypeDescriptorOffset;

		DWORD dwObjectBaseOffset;
		ReadMemory(RTTIObjectLocator + 0x14, &dwObjectBaseOffset, sizeof(DWORD));
		size_t ObjectBase = ModuleBase + dwObjectBaseOffset;


		DWORD dwClassHierarchyDescriptorOffset;
		ReadMemory(RTTIObjectLocator + 0x10, &dwClassHierarchyDescriptorOffset, sizeof(DWORD));

		//Offsets are from base
		size_t ClassHierarchyDescriptor = ModuleBase + dwClassHierarchyDescriptorOffset;
		if (!IsValidPtr(ClassHierarchyDescriptor) || !dwClassHierarchyDescriptorOffset)
			return x;

		DWORD NumBaseClasses;
		ReadMemory(ClassHierarchyDescriptor + 0x8, &NumBaseClasses, sizeof(DWORD));
		if (NumBaseClasses < 0 || NumBaseClasses > 25)
			NumBaseClasses = 0;

		DWORD BaseClassArrayOffset;
		ReadMemory(ClassHierarchyDescriptor + 0xC, &BaseClassArrayOffset, sizeof(DWORD));

		size_t BaseClassArray = ModuleBase + BaseClassArrayOffset;
		if (!IsValidPtr(BaseClassArray) || !BaseClassArrayOffset)
			return x;

		//x = AddText(View, x, y, crOffset, NONE, " RTTI:");
		CString RTTIString;
		for (unsigned int i = 0; i < NumBaseClasses; i++)
		{
			if (i != 0 && i != NumBaseClasses)
			{
				RTTIString += _T(" : ");
				//x = AddText(View, x, y, crOffset, NONE, " inherits:");
			}

			DWORD BaseClassDescriptorOffset;
			ReadMemory(BaseClassArray + (0x4 * i), &BaseClassDescriptorOffset, sizeof(DWORD));

			size_t BaseClassDescriptor = ModuleBase + BaseClassDescriptorOffset;
			if (!IsValidPtr(BaseClassDescriptor) || !BaseClassDescriptorOffset)
				continue;

			DWORD TypeDescriptorOffset;
			ReadMemory(BaseClassDescriptor, &TypeDescriptorOffset, sizeof(DWORD));

			size_t TypeDescriptor = ModuleBase + TypeDescriptorOffset;
			if (!IsValidPtr(TypeDescriptor) || !TypeDescriptorOffset)
				continue;

			CString RTTIName;
			bool FoundEnd = false;
			char LastChar = ' ';
			for (int j = 1; j < 45; j++)
			{
				char RTTINameChar;
				ReadMemory(TypeDescriptor + 0x10 + j, &RTTINameChar, 1);
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
		ReadMemory(pRTTIObjectLocator, &RTTIObjectLocator, sizeof(size_t));

		size_t pClassHierarchyDescriptor = RTTIObjectLocator + 0x10;
		if (!IsValidPtr(pClassHierarchyDescriptor))
			return x;

		size_t ClassHierarchyDescriptor;
		ReadMemory(pClassHierarchyDescriptor, &ClassHierarchyDescriptor, sizeof(size_t));

		int NumBaseClasses;
		ReadMemory(ClassHierarchyDescriptor + 0x8, &NumBaseClasses, sizeof(int));
		if (NumBaseClasses < 0 || NumBaseClasses > 25)
			NumBaseClasses = 0;

		size_t pBaseClassArray = ClassHierarchyDescriptor + 0xC;
		if (!IsValidPtr(pBaseClassArray))
			return x;

		size_t BaseClassArray;
		ReadMemory(pBaseClassArray, &BaseClassArray, sizeof(size_t));

		//x = AddText(View, x, y, crOffset, NONE, " RTTI: ");
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
			ReadMemory(pBaseClassDescriptor, &BaseClassDescriptor, sizeof(size_t));

			if (!IsValidPtr(BaseClassDescriptor))
				continue;

			size_t TypeDescriptor; //pointer at 0x00 in BaseClassDescriptor
			ReadMemory(BaseClassDescriptor, &TypeDescriptor, sizeof(size_t));

			CString RTTIName;
			bool FoundEnd = false;
			char LastChar = ' ';
			for (int j = 1; j < 45; j++)
			{
				char RTTINameChar;
				ReadMemory(TypeDescriptor + 0x08 + j, &RTTINameChar, 1);
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

	int AddComment(ViewInfo& View, int x, int y)
	{
		x = AddText(View, x, y, crComment, NONE, _T("//"));
		// Need the extra whitespace in "%s " anfter the %s to edit.
		x = AddText(View, x, y, crComment, HS_COMMENT, _T("%s "), Comment);

		// Added
		//if (GetType() == nt_int64)
		//{
			//DWORD_PTR Val = *((DWORD_PTR*)&((BYTE*)View.pData)[offset]);
			//x = AddText(View, x, y, crValue, NONE, "(0x%I64X)", Val);

			// Frostbite typeinfo shit
			//if (gbInt && Val > 10000000 && Val < 80000000000)
			//{
			//	__int64 strptr = __resolvefbtype(Val);
			//
			//	__int64 str;
			//	ReadMemory(strptr, &str, 8);
			//
			//	char szBuffer[256] = { 0 };
			//	ReadMemory(str, &szBuffer, 255);
			//
			//	x = AddText(View, x, y, crValue, NONE, "(0x%I64X) {%I64X->%I64X=fb::%s}", Val, strptr, str, GetStringFromMemory(szBuffer, 40).GetString());
			//}
		//}

		if (GetType() == nt_hex64)
		{
			float flVal = *((float*)&((BYTE*)View.pData)[offset]);
			// TODO: Change to int64
			__int64 intVal = *((__int64*)&((BYTE*)View.pData)[offset]);

			if (gbFloat)
			{
				if (flVal > -99999.0 && flVal < 99999.0)
					x = AddText(View, x, y, crValue, NONE, _T("(%0.3f)"), flVal);
				else
					x = AddText(View, x, y, crValue, NONE, _T("(%0.3f)"), 0.0f);
			}

			if (gbInt)
			{
				if (intVal > 0x6FFFFFFF && intVal < 0x7FFFFFFFFFFF)
					x = AddText(View, x, y, crValue, NONE, _T("(0x%I64X %I64d)"), intVal, intVal);
				else
					x = AddText(View, x, y, crValue, NONE, _T("(%I64d)"), intVal);
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
						x = AddText(View, x, y, crOffset, NONE, _T("*->%s "), a);
						if (gbRTTI)
							x = ResolveRTTI(uintVal, x, View, y);
					}
				}

				if (gbString)
				{
					bool bAddStr = true;
					char txt[64];
					ReadMemory(uintVal, txt, 64);

					for (int i = 0; i < 8; i++)
					{
						if (!isprint((BYTE)txt[i]))
							bAddStr = false;
					}

					if (bAddStr)
					{
						txt[63] = '\0';
						x = AddText(View, x, y, crChar, NONE, _T("'%hs'"), txt);
					}
				}
			}
		}
		else if (GetType() == nt_hex32)
		{
			float flVal = *((float*)&((BYTE*)View.pData)[offset]);
			int intVal = *((int*)&((BYTE*)View.pData)[offset]);

			if (gbFloat)
			{
				if (flVal > -99999.0 && flVal < 99999.0)
					x = AddText(View, x, y, crValue, NONE, _T("(%0.3f)"), flVal);
				else
					x = AddText(View, x, y, crValue, NONE, _T("(%0.3f)"), 0.0f);
			}

			if (gbInt)
			{
				#ifdef _WIN64
				if (intVal > 0x140000000 && intVal < 0x7FFFFFFFFFFF) // in 64 bit address range
					x = AddText(View, x, y, crValue, NONE, _T("(%i|0x%I64X)"), intVal, intVal);
				else if (intVal > 0x400000 && intVal < 0x140000000)
					x = AddText(View, x, y, crValue, NONE, _T("(%i|0x%IX)"), intVal);
				#else
				x = AddText(View, x, y, crValue, NONE, _T("(%i|0x%IX)"), intVal, intVal);
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
					if (uintVal > 0x400000 && uintVal < 0xFFFFFFFF)
					{
						x = AddText(View, x, y, crOffset, NONE, _T("*->%s "), addressStr);
						if (gbRTTI)
							x = ResolveRTTI(uintVal, x, View, y);
					}
				}

				if (gbString)
				{
					bool bAddStr = true;
					char txt[64];
					ReadMemory(uintVal, txt, 64); // TODO: find out why it looks wrong

					for (int i = 0; i < 4; i++)
					{
						if (!isprint((unsigned char)txt[i]))
							bAddStr = false;
					}

					if (bAddStr)
					{
						txt[63] = '\0'; // null terminte (even though we prolly dont have to)
						x = AddText(View, x, y, crChar, NONE, _T("'%hs'"), txt);
					}
				}
			}

		}

		return x;
	}

	void StandardUpdate(HotSpot &Spot)
	{
		if (Spot.ID == HS_NAME)
			Name = Spot.Text;
		else if (Spot.ID == HS_COMMENT)
			Comment = Spot.Text;
	}

	CStringA GetStringFromMemoryA(unsigned char* pMemory, int Length)
	{
		CStringA ascii;
		for (int i = 0; i < Length; i++)
		{
			if (isprint(pMemory[i] & 0xFF))
				ascii += pMemory[i];
			else
				ascii += '.';
		}
		return ascii;
	}

	CStringW GetStringFromMemoryW(unsigned char* pMemory, int Length)
	{
		CStringW widechar;
		for (int i = 0; i < Length; i += sizeof(wchar_t)) {
			widechar += iswprint(pMemory[i]) > 0 ? (wchar_t)pMemory[i] : (wchar_t)(L'.');
		}
		return widechar;
	}

	int DrawHidden(ViewInfo& View, int x, int y)
	{
		if (bSelected)
			View.dc->FillSolidRect(0, y, View.client->right, 1, crSelect);
		else
			View.dc->FillSolidRect(0, y, View.client->right, 1, crHidden);
		return y + 1;
	}
};

class CNodeIcon : public CNodeBase
{
public:
	virtual int Draw(ViewInfo& View, int x, int y)
	{
		for (UINT i = 0; i < 21; i++)
			x = AddIcon(View, x, y, i, -1, -1);
		return y += FontHeight;
	}
};

class CNodeClass : public CNodeBase
{
public:
	CNodeClass()
	{
		nodeType = nt_class;
		offset = GetBase();
		TCHAR szOffset[128];
#ifdef _WIN64
		_ui64tot_s(offset, szOffset, 128, 16);
#else
		_ultot_s(offset, szOffset, 128, 16);
#endif
		strOffset = szOffset;
		RequestPosition = -1;
		idx = 0;
	}

	int idx;
	CString Code;
	int RequestPosition;
	CChildFrame* pChildWindow;

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0)
		{
			strOffset = Spot.Text;
			offset = ConvertStrToAddress(Spot.Text);
		}

		if (Spot.ID == 1)
		{
			RequestPosition = _tcstol(Spot.Text, NULL, 10); // RequestPosition = ConvertStrToAddress( Spot.Text );
		}
	}

	virtual int GetMemorySize()
	{
		int size = 0;
		for (UINT i = 0; i < Nodes.size(); i++)
			size += Nodes[i]->GetMemorySize();
		return size;
	}

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		offset = ConvertStrToAddress(strOffset);

		AddSelection(View, 0, y, FontHeight);
		x = AddOpenClose(View, x, y);

		// Save tx here
		int tx = x;

		x = AddIcon(View, x, y, ICON_CLASS, -1, -1);
		x = AddText(View, x, y, crOffset, 0, _T("%s"), strOffset) + FontWidth;

		// x += ( NumDigits( offset ) ) * FontWidth;
		// TODO, figure this out better
		// x += ( ( NumDigits( offset ) - 7 ) * FontWidth ) / 2;

		// printf( "Print %s at %d\n", strOffset, x );

		x = AddText(View, x, y, crIndex, HS_NONE, _T("("));
		x = AddText(View, x, y, crIndex, HS_OPENCLOSE, _T("%i"), idx);
		x = AddText(View, x, y, crIndex, HS_NONE, _T(")"));

		x = AddText(View, x, y, crType, HS_NONE, _T("Class "));
		x = AddText(View, x, y, crName, HS_NAME, Name) + FontWidth;
		x = AddText(View, x, y, crValue, HS_NONE, _T("[%i]"), GetMemorySize()) + FontWidth;
		x = AddComment(View, x, y);

		y += FontHeight;
		if (bOpen[View.Level])
		{
			ViewInfo nv;
			nv = View;
			nv.Level++;
			for (UINT i = 0; i < Nodes.size(); i++)
				y = Nodes[i]->Draw(nv, tx, y);
		}

		return y;
	}
};

class CNodeHex64 : public CNodeBase
{
public:
	CNodeHex64() { nodeType = nt_hex64; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = (BYTE)(_tcstoul(Spot.Text, NULL, 16) & 0xFF);
		WriteMemory(Spot.Address + Spot.ID, &v, 1);
	}

	virtual int GetMemorySize() { return 8; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET + 16;
		tx = AddAddressOffset(View, tx, y);

		if (gbText)
		{
			CStringA str = GetStringFromMemoryA(pMemory, 8) + " ";
			tx = AddText(View, tx, y, crChar, HS_NONE, "%s", str.GetBuffer());
		}

		tx = AddText(View, tx, y, crHex, 0, _T("%0.2X"), pMemory[0]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 1, _T("%0.2X"), pMemory[1]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 2, _T("%0.2X"), pMemory[2]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 3, _T("%0.2X"), pMemory[3]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 4, _T("%0.2X"), pMemory[4]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 5, _T("%0.2X"), pMemory[5]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 6, _T("%0.2X"), pMemory[6]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 7, _T("%0.2X"), pMemory[7]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeHex32 : public CNodeBase
{
public:
	CNodeHex32() { nodeType = nt_hex32; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = (BYTE)(_tcstoul(Spot.Text, NULL, 16) & 0xFF);
		if (Spot.ID == 0) 
			WriteMemory(Spot.Address + 0, &v, 1);
		if (Spot.ID == 1) 
			WriteMemory(Spot.Address + 1, &v, 1);
		if (Spot.ID == 2) 
			WriteMemory(Spot.Address + 2, &v, 1);
		if (Spot.ID == 3) 
			WriteMemory(Spot.Address + 3, &v, 1);
	}

	virtual int GetMemorySize() { return 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET + 16;
		tx = AddAddressOffset(View, tx, y);

		if (gbText)
		{
			// TODO these are the dots, do alignment instead of 4
			CStringA str = GetStringFromMemoryA(pMemory, 4);
			str += " ";
			tx = AddText(View, tx, y, crChar, HS_NONE, "%s", str);
		}

		tx = AddText(View, tx, y, crHex, 0, _T("%0.2X"), pMemory[0]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 1, _T("%0.2X"), pMemory[1]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 2, _T("%0.2X"), pMemory[2]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 3, _T("%0.2X"), pMemory[3]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeHex16 : public CNodeBase
{
public:
	CNodeHex16() { nodeType = nt_hex16; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = (BYTE)(_tcstoul(Spot.Text, NULL, 16) & 0xFF);
		if (Spot.ID == 0) 
			WriteMemory(Spot.Address, &v, 1);
		if (Spot.ID == 1)
			WriteMemory(Spot.Address + 1, &v, 1);
	}

	virtual int GetMemorySize() { return 2; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) 
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET + 16;
		tx = AddAddressOffset(View, tx, y);

		if (gbText)
		{
			CStringA str = GetStringFromMemoryA(pMemory, 2);
			str += "  ";
			tx = AddText(View, tx, y, crChar, NONE, "%s", str.GetBuffer());
		}

		tx = AddText(View, tx, y, crHex, 0, _T("%0.2X"), pMemory[0] & 0xFF) + FontWidth;
		tx = AddText(View, tx, y, crHex, 1, _T("%0.2X"), pMemory[1] & 0xFF) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeHex8 : public CNodeBase
{
public:
	CNodeHex8() { nodeType = nt_hex8; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = (BYTE)(_tcstoul(Spot.Text, NULL, 16) & 0xFF);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 1);
	}

	virtual int GetMemorySize() { return 1; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) 
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET + 16;
		tx = AddAddressOffset(View, tx, y);

		if (gbText)
		{
			CStringA str = GetStringFromMemoryA(pMemory, 1);
			str += " ";
			tx = AddText(View, tx, y, crChar, HS_NONE, "%s", str.GetBuffer());
		}

		tx = AddText(View, tx, y, crHex, 0, _T("%0.2X"), pMemory[0] & 0xFF) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeBits : public CNodeBase
{
public:
	CNodeBits() { nodeType = nt_bits; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = (BYTE)(_tcstoul(Spot.Text, NULL, 16) & 0xFF);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 1);
	}

	virtual int GetMemorySize() { return 1; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET + 16;
		tx = AddAddressOffset(View, tx, y);

		if (gbText)
		{
			BitArray<unsigned char> bits;
			bits.SetValue(pMemory[0]);

			CStringA str = bits.GetBitsReverseString();
			str += ' ';
			tx = AddText(View, tx, y, crChar, HS_NONE, "%s", str.GetBuffer());
		}

		tx = AddText(View, tx, y, crHex, 0, _T("%0.2X"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeVTable : public CNodeBase
{
public:
	CNodeVTable() { nodeType = nt_vtable; }

	CMemory Memory;

	virtual void Update(HotSpot& Spot) { StandardUpdate(Spot); }

	int GetMemorySize()
	{
		return sizeof(size_t);
	}

	int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		DWORD_PTR* pMemory = (DWORD_PTR*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		x = AddOpenClose(View, x, y);
		x = AddIcon(View, x, y, ICON_VTABLE, -1, -1);

		int tx = x;
		x = AddAddressOffset(View, x, y);
		x = AddText(View, x, y, crVTable, HS_NONE, _T("VTable[%i]"), Nodes.size()) + FontWidth;

		if (Name.IsEmpty())
			x = AddText(View, x, y, crName, HS_NONE, _T("%s"), Name) + FontWidth;
		else
			x = AddText(View, x, y, crName, HS_NONE, _T("%s_vtable"), pParent->Name) + FontWidth;

		x = AddComment(View, x, y);

		y += FontHeight;
		if (bOpen[View.Level])
		{
			// vtable stuff
			DWORD NeededSize = (int)Nodes.size() * sizeof(size_t);

			Memory.SetSize(NeededSize);
			ViewInfo newView;
			newView = View;
			newView.pData = Memory.pMemory;

			newView.Address = pMemory[0];
			ReadMemory(newView.Address, newView.pData, NeededSize);

			for (UINT i = 0; i < Nodes.size(); i++)
			{
				Nodes[i]->offset = i * sizeof(size_t);
				y = Nodes[i]->Draw(newView, tx, y);
			}
		}

		return y;
	}
};

class CNodeFunctionPtr : public CNodeBase
{
public:
	CNodeFunctionPtr()
	{
		nodeType = nt_function;
		Name = _T("");
	}

	std::vector<CStringA> Assembly;

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0)
		{
			Assembly.clear();

			size_t addy = Spot.Address;
			ReadMemory(addy, &addy, sizeof(size_t));
			char* code[1536]; // max 1536 lines
			ReadMemory(addy, code, 1536);
			char** EndCodeSection = (code + 1536);

			DISASM MyDisasm;
			memset(&MyDisasm, 0, sizeof(DISASM));

			MyDisasm.EIP = (size_t)code;

			MyDisasm.VirtualAddr = (unsigned __int64)addy;
			#ifdef _WIN64
			MyDisasm.Archi = 64;
			#else
			MyDisasm.Archi = 0;
			#endif
			MyDisasm.Options = PrefixedNumeral;

			bool Error = 0;
			while (!Error)
			{
				MyDisasm.SecurityBlock = (unsigned __int32)((size_t)EndCodeSection - (size_t)MyDisasm.EIP);

				int len = Disasm(&MyDisasm);
				if (len == OUT_OF_BLOCK)
					Error = 1;
				else if (len == UNKNOWN_OPCODE)
					Error = 1;
				else
				{
					char szInstruction[96];
					sprintf_s(szInstruction, "%p  %s", (void*)MyDisasm.VirtualAddr, MyDisasm.CompleteInstr);
					Assembly.push_back(szInstruction);

					MyDisasm.EIP = MyDisasm.EIP + len;
					MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + len;
					if (MyDisasm.EIP >= (UIntPtr)EndCodeSection)
						break;

					unsigned char opcode;
					ReadMemory(MyDisasm.VirtualAddr, &opcode, sizeof(unsigned char));
					if (opcode == 0xCC) // INT3 instruction
						break;
				}
			}
		}
	}

	virtual int GetMemorySize()
	{
		return sizeof(size_t);
	}

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_METHOD, -1, -1);
		int ax = tx;
		tx = AddAddressOffset(View, tx, y);

		if (pParent->GetType() != nt_vtable)
			tx = AddText(View, tx, y, crType, HS_NONE, _T("Function"));
		else
			tx = AddText(View, tx, y, crFunction, HS_NONE, _T("(%i)"), offset / sizeof(size_t));

		tx = AddIcon(View, tx, y, ICON_CAMERA, HS_EDIT, HS_CLICK);
		tx += FontWidth;

		if (Name.IsEmpty())
			tx = AddText(View, tx, y, crName, HS_NAME, _T("Function_%i"), offset / sizeof(size_t));
		else
			tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);

		tx += FontWidth;

		if (Assembly.size() > 0)
			tx = AddOpenClose(View, tx, y);

		tx += FontWidth;

		tx = AddComment(View, tx, y);

		if (bOpen[View.Level])
		{
			for (UINT i = 0; i < Assembly.size(); i++)
			{
				y += FontHeight;
				AddText(View, ax, y, crHex, HS_NONE, "%s", Assembly[i].GetBuffer());
			}
		}

		return y += FontHeight;
	}
};

class CNodePtr : public CNodeBase
{
public:
	CNodePtr() { nodeType = nt_pointer; }

	CNodeBase* pNode;
	CMemory Memory;

	virtual void Update(HotSpot& Spot) { StandardUpdate(Spot); }

	virtual int GetMemorySize()
	{
		return sizeof(size_t);
	}

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		size_t* pMemory = (size_t*)&((BYTE*)View.pData)[offset];

		//printf( "read ptr: %p\n", View.pData );
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		x = AddOpenClose(View, x, y);
		x = AddIcon(View, x, y, ICON_POINTER, -1, -1);

		int tx = x;
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Ptr "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crValue, HS_NONE, _T(" <%s>"), pNode->Name);
		tx = AddIcon(View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_A);

		tx += FontWidth;
		tx = AddComment(View, tx, y);

		y += FontHeight;

		if (bOpen[View.Level])
		{
			DWORD NeededSize = pNode->GetMemorySize();
			Memory.SetSize(NeededSize);

			ViewInfo newView;
			newView = View;
			newView.pData = Memory.pMemory;
			newView.Address = pMemory[0];

			ReadMemory(newView.Address, newView.pData, NeededSize);

			y = pNode->Draw(newView, x, y);
		}
		return y;
	}
};

class CNodeInt64 : public CNodeBase
{
public:
	CNodeInt64() { nodeType = nt_int64; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int64 v = _ttoi64(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 8);
	}

	virtual int GetMemorySize() { return 8; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View, x, y);
		__int64* pMemory = (__int64*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Int64 "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%lli"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeInt32 : public CNodeBase
{
public:
	CNodeInt32() { nodeType = nt_int32; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int32 v = _ttoi(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 4);
	}

	virtual int GetMemorySize() { return 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View, x, y);
		__int32* pMemory = (__int32*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Int32 "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%i"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};
class CNodeInt16 : public CNodeBase
{
public:
	CNodeInt16() { nodeType = nt_int16; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int16 v = _ttoi(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 2);
	}

	virtual int GetMemorySize() { return 2; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View, x, y);
		__int16* pMemory = (__int16*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Int16 "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%i"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeInt8 : public CNodeBase
{
public:
	CNodeInt8() { nodeType = nt_int8; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int8 v = _ttoi(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 1);
	}

	virtual int GetMemorySize() { return 1; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View, x, y);
		__int8* pMemory = (__int8*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Int8  "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%i"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeDWORD : public CNodeBase
{
public:
	CNodeDWORD() { nodeType = nt_uint32; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		DWORD v = _ttoi(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 4);
	}

	virtual int GetMemorySize() { return 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View, x, y);
		DWORD* pMemory = (DWORD*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("DWORD "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%u"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeWORD : public CNodeBase
{
public:
	CNodeWORD() { nodeType = nt_uint16; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		WORD v = _ttoi(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 2);

	}

	virtual int GetMemorySize() { return 2; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		WORD* pMemory = (WORD*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_UNSIGNED, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, _T("WORD  "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, NONE, _T(" = "));
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%u"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeBYTE : public CNodeBase
{
public:
	CNodeBYTE() { nodeType = nt_uint8; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = _ttoi(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 1);
	}

	virtual int GetMemorySize() { return 1; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_UNSIGNED, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("BYTE  "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%u"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeText : public CNodeBase
{
public:
	CNodeText()
	{
		nodeType = nt_text;
		Name = _T("Text");
		memsize = 4;
	}

	DWORD memsize;

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0)
			memsize = _ttoi(Spot.Text);

		if (Spot.ID == 1)
		{
			DWORD Length = Spot.Text.GetLength() + 1;
			if (Length > memsize)
				Length = memsize;
			WriteMemory(Spot.Address, Spot.Text.GetBuffer(), Length);
		}
	}

	virtual int GetMemorySize() { return memsize; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_TEXT, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Text "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crIndex, HS_NONE, _T("["));
		tx = AddText(View, tx, y, crIndex, HS_EDIT, _T("%i"), memsize);
		tx = AddText(View, tx, y, crIndex, HS_NONE, _T("]"));

		CStringA str = GetStringFromMemoryA(pMemory, memsize);
		tx = AddText(View, tx, y, crChar, HS_NONE, _T(" = '"));
		tx = AddText(View, tx, y, crChar, 1, "%s", str.GetBuffer());
		tx = AddText(View, tx, y, crChar, HS_NONE, _T("' ")) + FontWidth;

		tx = AddComment(View, tx, y);
		return y += FontHeight;
	}
};

class CNodeCharPtr : public CNodeBase
{
public:
	CNodeCharPtr() {
		nodeType = nt_pchar;
		Name = "PChar";
	}

	CNodeBase* pNode;
	CMemory Memory;

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int64 v = _ttoi64(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 8);
	}

	virtual int GetMemorySize()
	{
		return sizeof(size_t);
	}

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		size_t* pMemory = (size_t*)&((BYTE*)View.pData)[offset];

		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View, x, y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_INTEGER, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("PCHAR "));
		tx = AddText(View, tx, y, crName, HS_NAME, Name);

		//tx = AddText(View,tx,y,crName,NONE," = ");
		//tx = AddText(View,tx,y,crValue,0,"%lli",pMemory[0]) + FontWidth;
		//tx = AddComment(View,tx,y);

		/*
		int tx = x + 16;
		tx = AddIcon(View,tx,y,ICON_TEXT,NONE,NONE);
		tx = AddAddressOffset(View,tx,y);
		tx = AddText(View,tx,y,crType,NONE,"Text ");
		tx = AddText(View,tx,y,crName,69,"%s",Name);
		tx = AddText(View,tx,y,crIndex,NONE,"[");
		tx = AddText(View,tx,y,crIndex,0,"%i",memsize);
		tx = AddText(View,tx,y,crIndex,NONE,"]");
		*/

		tx = AddText(View, tx, y, crChar, HS_NONE, _T(" = '"));
		if (VALID(pMemory))
		{
			size_t dw = pMemory[0];
			CStringA sc = ReadMemoryString(dw, 64);
			tx = AddText(View, tx, y, crChar, 1, "%s", sc.GetBuffer());
		}

		tx = AddText(View, tx, y, crChar, HS_NONE, _T("' ")) + FontWidth;

		tx = AddComment(View, tx, y);
		return y += FontHeight;
	}
};

class CNodeUnicode : public CNodeBase
{
public:
	CNodeUnicode()
	{
		nodeType = nt_unicode;
		Name = "Unicode";
		memsize = sizeof(size_t);
	}

	DWORD memsize;

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0)
			memsize = _ttoi(Spot.Text);

		if (Spot.ID == 1)
		{
			DWORD Length = Spot.Text.GetLength() + 1;
			if (Length > memsize)
				Length = memsize;

			TCHAR* pSource = Spot.Text.GetBuffer();

			wchar_t* pwszConverted = new wchar_t[Length + 1];
			for (UINT i = 0; i <= Length; i++)
				pwszConverted[i] = pSource[i];

			WriteMemory(Spot.Address, pwszConverted, Length * sizeof(wchar_t));

			delete pwszConverted;
		}
	};

	virtual int GetMemorySize(void) { return memsize; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		BYTE* pMemory = (BYTE*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_TEXT, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Unicode "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crIndex, HS_NONE, _T("["));
		tx = AddText(View, tx, y, crIndex, HS_EDIT, _T("%i"), memsize / sizeof(wchar_t));
		tx = AddText(View, tx, y, crIndex, HS_NONE, _T("]"));

		CStringW str = GetStringFromMemoryW(pMemory, memsize);
		tx = AddText(View, tx, y, crChar, HS_NONE, _T(" = '"));
		tx = AddText(View, tx, y, crChar, HS_OPENCLOSE, _T("%ls"), str);
		tx = AddText(View, tx, y, crChar, HS_NONE, _T("' ")) + FontWidth;

		tx = AddComment(View, tx, y);
		return y += FontHeight;
	}
};

class CNodeFloat : public CNodeBase
{
public:
	CNodeFloat() { nodeType = nt_float; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = (float)_ttof(Spot.Text);
		if (Spot.ID == HS_EDIT)
			WriteMemory(Spot.Address, &v, 4);
	}

	virtual int GetMemorySize() { return 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		float* pMemory = (float*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_FLOAT, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("float "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		//tx = AddText(View,tx,y,crValue,0,"%.4f",pMemory[0]) + FontWidth;

		//if ( *pMemory > -99999.0f && *pMemory < 99999.0f )
		//	*pMemory = 0;

		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%4.3f"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeDouble : public CNodeBase
{
public:
	CNodeDouble() { nodeType = nt_double; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		double v = _ttof(Spot.Text);
		if (Spot.ID == 0)
			WriteMemory(Spot.Address, &v, 8);
	}

	virtual int GetMemorySize(void)
	{
		// doubles are always 64 bits ffs
		return 8;
	}

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);
		double* pMemory = (double*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_FLOAT, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("double "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crName, HS_NONE, _T(" = "));
		//tx = AddText(View, tx, y, crValue, 0, "%.4lg", pMemory[0]) + FontWidth;
		tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%.4g"), pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeVec2 : public CNodeBase
{
public:
	CNodeVec2()
	{
		nodeType = nt_vec2;
		for (UINT i = 0; i < bOpen.size(); i++)
			bOpen[i] = true;
	}

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = (float)_ttof(Spot.Text);
		if (Spot.ID >= 0 && Spot.ID < 2)
			WriteMemory(Spot.Address + (Spot.ID * 4), &v, 4);
	}

	virtual int GetMemorySize() { return 4 + 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		float* pMemory = (float*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_VECTOR, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Vec2 "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddOpenClose(View, tx, y);
		if (bOpen[View.Level])
		{
			tx = AddText(View, tx, y, crName, HS_NONE, _T("("));
			tx = AddText(View, tx, y, crValue, HS_EDIT, _T("%0.3f"), pMemory[0]);
			tx = AddText(View, tx, y, crName, HS_NONE, _T(","));
			tx = AddText(View, tx, y, crValue, HS_OPENCLOSE, _T("%0.3f"), pMemory[1]);
			tx = AddText(View, tx, y, crName, HS_NONE, _T(")"));
		}
		tx += FontWidth;
		tx = AddComment(View, tx, y);
		return (y + FontHeight);
	}
};

class CNodeVec3 : public CNodeBase
{
public:
	CNodeVec3()
	{
		nodeType = nt_vec3;
		for (UINT i = 0; i < bOpen.size(); i++)
			bOpen[i] = true;
	}

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = (float)_ttof(Spot.Text);
		if (Spot.ID >= 0 && Spot.ID < 3)
			WriteMemory(Spot.Address + (Spot.ID * 4), &v, 4);
	}

	virtual int GetMemorySize() { return 12; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View, x, y);
		float* pMemory = (float*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_VECTOR, HS_NONE, HS_NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, HS_NONE, _T("Vec3 "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddOpenClose(View, tx, y);
		if (bOpen[View.Level])
		{
			tx = AddText(View, tx, y, crName, HS_NONE, _T("("));
			tx = AddText(View, tx, y, crValue, 0, _T("%0.3f"), pMemory[0]);
			tx = AddText(View, tx, y, crName, HS_NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 1, _T("%0.3f"), pMemory[1]);
			tx = AddText(View, tx, y, crName, HS_NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 2, _T("%0.3f"), pMemory[2]);
			tx = AddText(View, tx, y, crName, HS_NONE, _T(")"));
		}
		tx += FontWidth;
		tx = AddComment(View, tx, y);

		return y + FontHeight;
	}
};

class CNodeQuat : public CNodeBase
{
public:
	CNodeQuat()
	{
		nodeType = nt_quat;
		for (UINT i = 0; i < bOpen.size(); i++)
			bOpen[i] = true;
	}

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = (float)_ttof(Spot.Text);
		if (Spot.ID >= 0 && Spot.ID < 4)
			WriteMemory(Spot.Address + (Spot.ID * 4), &v, 4);
	}

	virtual int GetMemorySize() { return 16; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		float* pMemory = (float*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_VECTOR, -1, -1);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, _T("Vec4 "));
		tx = AddText(View, tx, y, crName, 69, _T("%s"), Name);
		tx = AddOpenClose(View, tx, y);
		if (bOpen[View.Level])
		{
			tx = AddText(View, tx, y, crName, NONE, _T("("));
			tx = AddText(View, tx, y, crValue, 0, _T("%0.3f"), pMemory[0]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 1, _T("%0.3f"), pMemory[1]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 2, _T("%0.3f"), pMemory[2]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 3, _T("%0.3f"), pMemory[3]);
			tx = AddText(View, tx, y, crName, NONE, _T(")"));
		}
		tx += FontWidth;
		tx = AddComment(View, tx, y);

		return y + FontHeight;
	}
};

class CNodeMatrix : public CNodeBase
{
public:
	CNodeMatrix() { nodeType = nt_matrix; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID < 16)
		{
			float v = (float)_ttof(Spot.Text);
			WriteMemory(Spot.Address + (Spot.ID * 4), &v, 4);
		}
	}

	virtual int GetMemorySize() { return 4 * 4 * 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View, x, y);
		float* pMemory = (float*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_MATRIX, NONE, NONE);
		int mx = tx;
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, _T("Matrix "));
		tx = AddText(View, tx, y, crName, 69, _T("%s"), Name);
		tx = AddOpenClose(View, tx, y);
		tx += FontWidth;
		tx = AddComment(View, tx, y);

		if (bOpen[View.Level])
		{
			y += FontHeight;
			tx = mx;
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
			tx = AddText(View, tx, y, crValue, 0, _T("% 14.3f"), pMemory[0]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 1, _T("% 14.3f"), pMemory[1]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 2, _T("% 14.3f"), pMemory[2]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 3, _T("% 14.3f"), pMemory[3]);
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
			y += FontHeight;
			tx = mx;
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
			tx = AddText(View, tx, y, crValue, 4, _T("% 14.3f"), pMemory[4]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 5, _T("% 14.3f"), pMemory[5]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 6, _T("% 14.3f"), pMemory[6]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 7, _T("% 14.3f"), pMemory[7]);
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
			y += FontHeight;
			tx = mx;
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
			tx = AddText(View, tx, y, crValue, 8, _T("% 14.3f"), pMemory[8]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 9, _T("% 14.3f"), pMemory[9]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 10, _T("% 14.3f"), pMemory[10]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 11, _T("% 14.3f"), pMemory[11]);
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
			y += FontHeight;
			tx = mx;
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
			tx = AddText(View, tx, y, crValue, 12, _T("% 14.3f"), pMemory[12]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 13, _T("% 14.3f"), pMemory[13]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 14, _T("% 14.3f"), pMemory[14]);
			tx = AddText(View, tx, y, crName, NONE, _T(","));
			tx = AddText(View, tx, y, crValue, 15, _T("% 14.3f"), pMemory[15]);
			tx = AddText(View, tx, y, crName, NONE, _T("|"));
		}

		return y + FontHeight;
	}
};

class CNodeArray : public CNodeBase
{
public:
	CNodeArray()
	{
		nodeType = nt_array;
		Total = 1;
		Current = 0;
	}

	CNodeBase* pNode;
	DWORD Total;
	DWORD Current;

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		int v = _ttoi(Spot.Text);
		if (v < 0)
			return;

		if (Spot.ID == 0)
		{
			if (v == 0)
				return;
			Total = (DWORD)v;
		}
		else if (Spot.ID == 1)
		{
			if (v >= (int)Total)
				return;
			Current = (DWORD)v;
		}
		else if (Spot.ID == 2)
		{
			if (Current > 0)
				Current--;
		}
		else if (Spot.ID == 3)
		{
			if (Current < Total - 1)
				Current++;
		}
	};

	virtual int GetMemorySize() { return pNode->GetMemorySize() * Total; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		x = AddOpenClose(View, x, y);
		x = AddIcon(View, x, y, ICON_ARRAY, -1, -1);

		int tx = x;
		tx = AddAddressOffset(View, tx, y);

		tx = AddText(View, tx, y, crType, NONE, _T("Array "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crIndex, NONE, _T("["));
		tx = AddText(View, tx, y, crIndex, HS_EDIT, _T("%i"), Total);
		tx = AddText(View, tx, y, crIndex, NONE, _T("]"));

		tx = AddIcon(View, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK);
		tx = AddText(View, tx, y, crIndex, NONE, _T("("));
		tx = AddText(View, tx, y, crIndex, 1, _T("%i"), Current);
		tx = AddText(View, tx, y, crIndex, NONE, _T(")"));
		tx = AddIcon(View, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK);

		tx = AddText(View, tx, y, crValue, NONE, _T("<%s Size=%i>"), pNode->Name, GetMemorySize());
		tx = AddIcon(View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X);

		tx += FontWidth;
		tx = AddComment(View, tx, y);

		y += FontHeight;
		if (bOpen[View.Level])
		{
			ViewInfo newView;
			newView = View;
			newView.Address = View.Address + offset + pNode->GetMemorySize() * Current;
			newView.pData = (void*)((size_t)View.pData + offset + pNode->GetMemorySize() * Current);
			y = pNode->Draw(newView, x, y);
		};
		return y;
	}
};

class CNodeClassInstance : public CNodeBase
{
public:
	CNodeClassInstance() { nodeType = nt_instance; }

	CNodeClass* pNode;

	virtual void Update(HotSpot& Spot) { StandardUpdate(Spot); }

	virtual int GetMemorySize() { return pNode->GetMemorySize(); }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		x = AddOpenClose(View, x, y);
		x = AddIcon(View, x, y, ICON_CLASS, -1, -1);

		int tx = x;
		tx = AddAddressOffset(View, tx, y);

		tx = AddText(View, tx, y, crType, NONE, _T("Instance "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name);
		tx = AddText(View, tx, y, crValue, NONE, _T("<%s>"), pNode->Name);
		tx = AddIcon(View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X);

		tx += FontWidth;
		tx = AddComment(View, tx, y);

		y += FontHeight;
		if (bOpen[View.Level])
		{
			ViewInfo newView;
			newView = View;
			newView.Address = View.Address + offset;
			newView.pData = (void*)((size_t)newView.pData + offset);

			y = pNode->Draw(newView, x, y);
		}

		return y;
	}
};

class CNodeCustom : public CNodeBase
{
public:
	CNodeCustom()
	{
		nodeType = nt_custom;
		Name = _T("Custom");
		memsize = sizeof(size_t);
	}

	int memsize;

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0)
			memsize = _ttoi(Spot.Text);
	}

	virtual int GetMemorySize() { return memsize; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden)
			return DrawHidden(View, x, y);

		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + TXOFFSET;
		tx = AddIcon(View, tx, y, ICON_CUSTOM, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, _T("Custom "));
		tx = AddText(View, tx, y, crIndex, NONE, _T("["));
		tx = AddText(View, tx, y, crIndex, HS_EDIT, _T("%i"), memsize);
		tx = AddText(View, tx, y, crIndex, NONE, _T("] "));
		tx = AddText(View, tx, y, crName, HS_NAME, _T("%s"), Name) + FontWidth;
		tx = AddComment(View, tx, y);
		return y += FontHeight;
	}
};

