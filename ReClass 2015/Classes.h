#pragma once

#include "Debug.h"

extern DWORD NodeCreateIndex;

template<class T> int NumDigits(T number)
{
	__int64 digits = 0;
	// remove this if '-' counts as a digit
	if ( number < 0 )
		digits = 1; 
	while (number)
	{
		number /= 0x16;
		digits++;
	}
	return digits;
}

class CMemory
{
public:
	CMemory()
	{
		MemorySize = 0;
		pMemory = NULL;
	}
	~CMemory()
	{
		if (pMemory) 
			delete pMemory;
	}

	DWORD MemorySize;
	BYTE* pMemory;

	void SetSize(DWORD Size)
	{
		if ( (!pMemory) || (Size != MemorySize) )
		{
			if (pMemory) delete pMemory;
			pMemory = new BYTE[Size];
			MemorySize = Size;
		}
	}
};

class CNodeClass;
struct ViewInfo
{
	CDC* dc;
	CRect* client;
	std::vector<HotSpot>* HotSpots;
	std::vector<CNodeClass*>* Classes;
	DWORD_PTR Address;
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

		for(int i = 0; i < 32; i++ )
			bOpen.push_back( false );

		bSelected = false;

		// This is the class name
		Name.Format("N%0.8X", NodeCreateIndex);
		NodeCreateIndex++;
		pParent = NULL;
	}

	~CNodeBase() { }

	virtual int Draw(ViewInfo& View,int x,int y) = 0;
	virtual int GetMemorySize() = 0;
	virtual void Update(HotSpot& Spot) = 0;
	virtual NodeType GetType() = 0; 

	DWORD_PTR offset;
	CString strOffset;

	CString Name;
	CString Comment;
	CNodeBase* pParent;
	std::vector<CNodeBase*> Nodes;

	bool bHidden;
	bool bSelected;
	std::vector<bool> bOpen;

	// Here is incorrect view.address
	void AddHotSpot(ViewInfo& View, CRect& Spot, CString& Text, int ID, int Type)
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

	int AddText( ViewInfo& View, int x, int y, DWORD color, int HitID, const char *fmt, ... )
	{
		va_list va_alist;
		static char logbuf[1024];
		if (fmt==NULL) return x;
		va_start (va_alist, fmt);
		_vsnprintf(logbuf, sizeof(logbuf), fmt, va_alist);
		va_end (va_alist);

		int width = ( int )strlen( logbuf ) * FontWidth;

		if ( ( y >= -FontHeight ) && ( y + FontHeight <= View.client->bottom+FontHeight ) )
		{
			CRect pos;

			if ( HitID != NONE )
			{
				if ( width >= FontWidth * 2 )
					pos.SetRect( x, y, x + width, y + FontHeight );
				else
					pos.SetRect( x, y, x + FontWidth * 2, y + FontHeight );

				AddHotSpot( View, pos, CString(logbuf), HitID, HS_EDIT );
			}

			pos.SetRect(x, y, 0, 0);
			View.dc->SetTextColor( color );
			View.dc->SetBkMode( TRANSPARENT );
			View.dc->DrawText( logbuf, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX );
		}

		return x + width;
	}

	int AddAddressOffset( ViewInfo& View, int x, int y )
	{
		if (gbOffset)
		{
			#ifdef _WIN64
			// goto 722
			// just the left side 0000
			// TODO: fix the ghetto rig FontWidth * x
			// where x = characters over 8

			x += FontWidth; // we need this either way

			unsigned int numdigits = NumDigits( View.Address );
			int pad = 1;

			if ( numdigits < 8 && numdigits > 4 )
				x -= ( ( 8 - numdigits ) * FontWidth );

			if ( numdigits > 8 )
				x += ( ( numdigits - 8 ) * FontWidth );

			x = AddText( View, x - pad, y, crOffset, NONE, "%0.4X", offset ) + FontWidth;
			#else
			x = AddText(View, x, y, crOffset, NONE,	"%0.4X", offset) + FontWidth;
			#endif
		}

		if (gbAddress)
		{
			#ifdef _WIN64
			// full address
			// printf( "%p + %p = %p\n", View.Address, offset, View.Address + offset );
			// %0.9X //"%I64x",

			x = AddText( View, x, y, crAddress, NONE, "%0.9I64X", View.Address + offset ) + FontWidth;
			#else
			x = AddText( View, x, y, crAddress, NONE, "%0.8X"	, View.Address + offset ) + FontWidth;
			#endif
		}

		return x;
	}

	void AddSelection(ViewInfo& View,int x,int y,int Height)
	{
		if ((y > View.client->bottom) || (y + Height < 0))
			return;
		if (bSelected)
			View.dc->FillSolidRect(0, y, View.client->right, Height, crSelect);
		CRect pos;
		pos.SetRect(0, y, 1024, y + Height);
		AddHotSpot(View, pos, CString(), 0, HS_SELECT);
	}

	int AddIcon(ViewInfo& View,int x,int y,int idx,int ID,int Type)
	{
		if ( (y > View.client->bottom) || (y+16 < 0) ) return x + 16;

		DrawIconEx(View.dc->m_hDC,x,y,Icons[idx],16,16,0,NULL,DI_NORMAL);
		if (ID != -1)
		{
			CRect pos;
			pos.SetRect(x,y,x+16,y+16);
			AddHotSpot(View,pos,CString(),ID,Type);
		}
		return x + 16;
	}

	int AddOpenClose(ViewInfo& View,int x,int y)
	{
		if ( (y > View.client->bottom) || (y+16 < 0) ) return x + 16;
		if ( bOpen[View.Level] )
			return AddIcon(View,x,y,ICON_OPEN, 0, HS_OPENCLOSE);
		else
			return AddIcon(View,x,y,ICON_CLOSED, 0, HS_OPENCLOSE);
	}

	void AddDelete(ViewInfo& View,int x,int y)
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

	void AddTypeDrop(ViewInfo& View,int x,int y)
	{
		if (View.bMultiSelected) 
			return;
		if ( (y > View.client->bottom) || (y+16 < 0) ) 
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

	int AddComment(ViewInfo& View,int x,int y)
	{
		x = AddText(View, x, y, crComment, NONE, "//");
		x = AddText(View, x, y, crComment, 70, " %s", Comment);

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
			float f = *((float*) &((BYTE*)View.pData)[offset]);
			// TODO: Change to int64
			int i = *((int*) &((BYTE*)View.pData)[offset]);

			if (gbFloat)
			{
				if ( f > -99999.0 && f < 99999.0 )
					x = AddText( View, x, y, crValue, NONE, "(%0.3f)", f );
				else
					x = AddText( View, x, y, crValue, NONE, "(%0.3f)", 0.0f );
			}

			if (gbInt)
			{
				if ( f > 140000000 && f < 80000000000 )
					x = AddText( View, x, y, crValue, NONE, "(0x%I64X %i)", i, i);
				else
					x = AddText( View, x, y, crValue, NONE, "(%i)", i );
			}

			// *** this is probably broken, let's fix it after
			DWORD_PTR Val = *((DWORD_PTR*) &((BYTE*)View.pData)[offset]);
			CString a(GetAddressName(Val,false));
			if (a.GetLength() > 0)
			{
				if (gbPointers)
				{
					//printf( "<%p> here\n", Val );
					if ( Val > 140000000 && Val < 80000000000 )
						x = AddText(View,x,y,crOffset,NONE,"*->%s ",a);
				}

				if (gbString)
				{
					bool bAddStr = true;
					char txt[64];
					ReadMemory(Val, txt, 64); // TODO: find out why and how, and why it looks wrong

					// also this 4 !! not 8 !!
					for ( int i = 0; i < 8; i++ )
					{
						if (!isprint((BYTE)txt[i]))
							bAddStr = false;
					}

					if (bAddStr)
					{
						txt[63] = '\0';
						x = AddText(View, x, y, crChar, NONE, "'%s'", txt);
					}
				}
			}
		}

		if (GetType() == nt_hex32)
		{
			float f = *((float*) &((BYTE*)View.pData)[offset]);
			int i = *((int*) &((BYTE*)View.pData)[offset]);

			if (gbFloat)
			{
				if ( f > -99999.0 && f < 99999.0 )
					x = AddText( View, x, y, crValue, NONE, "(%0.3f)", f );
				else
					x = AddText( View, x, y, crValue, NONE, "(%0.3f)", 0.0f );
			}

			if (gbInt)
			{
				//if (f > 140000000 && f < 80000000000) // in 64 bit address range
				//	x = AddText(View, x, y, crValue, NONE, "(0x%I64X %i)", i, i);
				//else
					x = AddText(View, x, y, crValue, NONE, "(%i)", i);
			}

			// *** this is probably broken, let's fix it after
			#ifdef _WIN64
			DWORD_PTR Val = *((DWORD_PTR*) &((BYTE*)View.pData)[offset]);
			#else
			DWORD Val = *((DWORD*) &((BYTE*)View.pData)[offset]);
			#endif

			CString a(GetAddressName(Val, false));

			if (a.GetLength() > 0)
			{
				if (gbPointers)
				{
					//printf( "<%p> here\n", Val ); // Shit is for 64 bit nigga we only using __int32
					/*if (Val > 140000000 && Val < 80000000000)
						x = AddText(View,x,y,crOffset,NONE,"*->%s ", a); */
					x = AddText(View, x, y, crOffset, NONE, "*->%s ", a);
				}

				if (gbString)
				{
					bool bAddStr = true;
					char txt[32];
					ReadMemory(Val, txt, 32); // TODO: find out why and how, and why it looks wrong

					for ( int i = 0; i < 4; i++ )
					{
						if (!isprint((BYTE)txt[i]))
							bAddStr = false;
					}

					if (bAddStr)
					{
						txt[31] = '\0'; // null terminte (even though we prolly dont have to)
						x = AddText(View, x, y, crChar, NONE, "'%s'", txt);
					}
				}
			}

		}

		return x;
	}

	void StandardUpdate(HotSpot &Spot)
	{
		if (Spot.ID == 69) Name = Spot.Text;
		if (Spot.ID == 70) Comment = Spot.Text;
	}

	CString GetStringFromMemory( char* pMemory, int Length )
	{
		CString asc;
		for ( int i=0; i < Length; i++ )
		{
			if ( isprint( pMemory[i] & 0xFF ) )
				asc += pMemory[i];
			else
				asc += '.';
		}
		return asc;
	}

	CStringW GetStringFromMemoryW(wchar_t* pMemory, int Length)
	{
		CStringW asc;
		for (int i=0; i < Length;i += sizeof( wchar_t ) )
		{
			asc += iswprint( pMemory[i] ) > 0 ? pMemory[i] : L'.';
		}
		return asc;
	}

	int DrawHidden(ViewInfo& View,int x,int y)
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
	virtual int Draw(ViewInfo& View,int x,int y)
	{
		for (UINT i = 0; i < 21; i++)
			x = AddIcon(View, x, y, i, -1, -1);
		return y += FontHeight;
	}
};

DWORD_PTR ConvertStrToAddress(CString Address);
class CNodeClass : public CNodeBase
{
public:
	CNodeClass( )
	{
		#ifdef _WIN64
		offset = 0x140000000;
		//printf( "[+] Created offset: %p Characters: %d\n", offset, NumDigits( offset ) );
		strOffset = "140000000";
		RequestPosition = -1;
		#else
		offset = 0x400000;
		strOffset = "400000";
		RequestPosition = -1;
		#endif
	}

	CString Code;
	int RequestPosition;

	virtual NodeType GetType() { return nt_class; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0) 
		{	
			//offset = strtoul(Spot.Text,NULL,16);
			strOffset = Spot.Text;
			offset = ConvertStrToAddress( Spot.Text );
			//printf( "[+] Typed shit: %s -> %p\n", Spot.Text, offset );
		}

		// TODO figure out if I need this
		if (Spot.ID == 1)
			RequestPosition = atoi(Spot.Text); // RequestPosition = ConvertStrToAddress( Spot.Text );
	};

	virtual int GetMemorySize()	
	{
		int size = 0;
		for (UINT i=0; i < Nodes.size();i++)
			size += Nodes[i]->GetMemorySize();
		return size;
	}

	virtual int Draw(ViewInfo& View,int x, int y)
	{
		offset = ConvertStrToAddress(strOffset);

		AddSelection(View, 0, y, FontHeight);
		x = AddOpenClose(View, x, y);
		x = AddIcon(View, x, y, ICON_CLASS, -1, -1);

		UINT idx = 0;
		for (UINT i = 0; i < View.Classes->size(); i++)
		{
			CNodeClass* pClass = View.Classes->at(i);
			if (pClass == this)
				idx = i;
		}

		int tx = x;
		x = AddText( View, x, y, crOffset, 0, "%s", strOffset ) + FontWidth;

		// x += ( NumDigits( offset ) ) * FontWidth;
		// TODO, figure this out better
		// x += ( ( NumDigits( offset ) - 7 ) * FontWidth ) / 2;

		// printf( "Print %s at %d\n", strOffset, x );

		x = AddText( View, x, y, crIndex, NONE, "(" );
		x = AddText( View, x, y, crIndex, 1, "%i", idx );
		x = AddText( View, x, y, crIndex, NONE, ")" );

		x = AddText( View, x, y, crType, NONE, "Class " );
		x = AddText( View, x, y, crName, 69, Name ) + FontWidth;
		x = AddText( View, x, y, crValue, NONE, "[%i]", GetMemorySize( ) ) + FontWidth;
		x = AddComment( View, x, y );

		y += FontHeight;
		if (bOpen[View.Level])
		{
			ViewInfo nv = View; 
			nv.Level++;
			for (UINT i=0; i < Nodes.size();i++)
				y = Nodes[i]->Draw(nv, tx, y);
		}

		return y;
	}
};

class CNodeHex64 : public CNodeBase
{
public:
	virtual NodeType GetType(){ return nt_hex64; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);

		BYTE v = (BYTE)(strtoul(Spot.Text, NULL, 16) & 0xFF);

		if (Spot.ID == 0) WriteMemory(Spot.Address + 0, &v, 1);
		if (Spot.ID == 1) WriteMemory(Spot.Address + 1, &v, 1);
		if (Spot.ID == 2) WriteMemory(Spot.Address + 2, &v, 1);
		if (Spot.ID == 3) WriteMemory(Spot.Address + 3, &v, 1);
		if (Spot.ID == 4) WriteMemory(Spot.Address + 4, &v, 1);
		if (Spot.ID == 5) WriteMemory(Spot.Address + 5, &v, 1);
		if (Spot.ID == 6) WriteMemory(Spot.Address + 6, &v, 1);
		if (Spot.ID == 7) WriteMemory(Spot.Address + 7, &v, 1);
	}

	virtual int GetMemorySize( )
	{
		return 8;
	}

	virtual int Draw( ViewInfo& View,int x,int y )
	{
		if ( bHidden )
			return DrawHidden( View,x,y );

		BYTE* pMemory = (BYTE*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 32;
		tx = AddAddressOffset(View,tx,y);

		if (gbText)
		{
			// TODO: these are dots ( do according to alignment, not always 8 )
			CString asc = GetStringFromMemory((char*)&pMemory[0], 8) + " ";
			tx = AddText(View, tx, y, crChar, NONE,"%s", asc);
		}

		tx = AddText(View, tx, y, crHex, 0, "%0.2X", pMemory[0]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 1, "%0.2X", pMemory[1]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 2, "%0.2X", pMemory[2]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 3, "%0.2X", pMemory[3]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 4, "%0.2X", pMemory[4]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 5, "%0.2X", pMemory[5]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 6, "%0.2X", pMemory[6]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 7, "%0.2X", pMemory[7]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};


class CNodeHex32 : public CNodeBase
{
public:
	virtual NodeType GetType(){ return nt_hex32; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = (BYTE)(strtoul(Spot.Text,NULL,16) & 0xFF);
		if (Spot.ID == 0) WriteMemory(Spot.Address + 0, &v, 1);
		if (Spot.ID == 1) WriteMemory(Spot.Address + 1, &v, 1);
		if (Spot.ID == 2) WriteMemory(Spot.Address + 2, &v, 1);
		if (Spot.ID == 3) WriteMemory(Spot.Address + 3, &v, 1);
	}

	virtual int GetMemorySize() { return 4; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		BYTE* pMemory = (BYTE*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 32;
		tx = AddAddressOffset(View,tx,y);

		if (gbText)
		{
			// TODO these are the dots, do alignment instead of 4
			CString asc = GetStringFromMemory((PCHAR)&pMemory[0], 4) + "     ";
			tx = AddText(View, tx, y, crChar, NONE,"%s", asc);
		}

		tx = AddText(View, tx, y, crHex, 0, "%0.2X", pMemory[0]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 1, "%0.2X", pMemory[1]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 2, "%0.2X", pMemory[2]) + FontWidth;
		tx = AddText(View, tx, y, crHex, 3, "%0.2X", pMemory[3]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeHex16 : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_hex16; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = ( BYTE )( strtoul(Spot.Text,NULL,16 ) & 0xFF );
		if (Spot.ID == 0) WriteMemory(Spot.Address,&v,1);
		if (Spot.ID == 1) WriteMemory(Spot.Address+1,&v,1);
	}

	virtual int GetMemorySize() { return 2; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		BYTE* pMemory = (BYTE*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 32;
		tx = AddAddressOffset(View,tx,y);

		if (gbText)
		{
			CString asc = GetStringFromMemory((char*)&pMemory[0],2) + "   ";

			// TODO: clean this up
			asc += ' ';
			asc += ' ';
			asc += ' ';
			asc += ' ';

			tx = AddText(View,tx,y,crChar,NONE,"%s",asc);
		}

		tx = AddText(View,tx,y,crHex,0,"%0.2X",pMemory[0]) + FontWidth;
		tx = AddText(View,tx,y,crHex,1,"%0.2X",pMemory[1]) + FontWidth;
		tx = AddComment(View,tx,y);

		return y += FontHeight;
	}
};

class CNodeHex8 : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_hex8; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = ( BYTE )( strtoul(Spot.Text,NULL,16) & 0xFF );
		if (Spot.ID == 0) WriteMemory(Spot.Address,&v,1);
	}

	virtual int GetMemorySize(){ return 1; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		BYTE* pMemory = (BYTE*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 32;
		tx = AddAddressOffset(View,tx,y);

		if (gbText)
		{
			CString asc = GetStringFromMemory((char*)&pMemory[0],1) + "    ";
			asc += ' ';
			asc += ' ';
			asc += ' ';
			asc += ' ';
			tx = AddText(View,tx,y,crChar,NONE,"%s",asc);
		}

		tx = AddText(View, tx, y, crHex, 0, "%0.2X", pMemory[0]) + FontWidth;
		tx = AddComment(View,tx,y);

		return y += FontHeight;
	}
};

class CNodeVTable : public CNodeBase
{
public:
	CMemory Memory;

	virtual NodeType GetType() { return nt_vtable; }
	virtual void Update(HotSpot& Spot) { StandardUpdate(Spot); }
	
	int GetMemorySize()
	{ 
		#ifdef _WIN64
		return 8;
		#else
		return 4;
		#endif
	}

	int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		DWORD_PTR* pMemory = (DWORD_PTR*) &((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		x = AddOpenClose(View, x, y);
		x = AddIcon(View, x, y, ICON_VTABLE, -1, -1);

		int tx = x;
		x = AddAddressOffset(View, x, y);
		x = AddText(View, x, y, crVTable, NONE, "VTable [%i]", Nodes.size()) + FontWidth;
		x = AddComment(View, x, y);

		y += FontHeight;
		if (bOpen[View.Level])
		{
			// vtable stuff
			DWORD NeededSize = Nodes.size()*8;
			Memory.SetSize(NeededSize);
			ViewInfo newView;
			newView = View;
			newView.pData = Memory.pMemory;

			newView.Address = pMemory[0];
			ReadMemory(newView.Address,newView.pData,NeededSize);

			for (UINT i=0; i < Nodes.size();i++)
			{
				Nodes[i]->offset = i * 8;
				y = Nodes[i]->Draw(newView,tx,y); 
			}
		}

		return y;
	}
};

class CNodeFunctionPtr : public CNodeBase
{
public:
	CNodeFunctionPtr() { Name = ""; }

	std::vector<CString> Assembly;

	virtual NodeType GetType() { return nt_function; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0)
		{
			CString d;
			Assembly.clear();
			DWORD_PTR addy = Spot.Address;
			//todo alignment
			//printf( "Reading: %p\n", addy );
			ReadMemory(addy, &addy, sizeof(DWORD_PTR));
			char* code[512];
			ReadMemory(addy, code, 512);

			DISASM MyDisasm;
			memset (&MyDisasm, 0, sizeof(DISASM));
			MyDisasm.EIP = (int) code;
			MyDisasm.VirtualAddr = (long long) addy;

			for (UINT i=0; i < 20; i++)
			{
				MyDisasm.SecurityBlock = (int) (code+512) - MyDisasm.EIP;
				int len = Disasm(&MyDisasm);
				if (len == OUT_OF_BLOCK) break;
				if (len == UNKNOWN_OPCODE) break;

				Assembly.push_back(MyDisasm.CompleteInstr);
				//d.Format("%.8X %s",(int) MyDisasm.VirtualAddr,&MyDisasm.CompleteInstr);

				MyDisasm.EIP = MyDisasm.EIP + len;
				MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + len;
			}
		}
	}
	
	virtual int GetMemorySize() 
	{
		#ifdef _WIN64
		return 8;
		#else
		return 4;
		#endif
	}

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) 
			return DrawHidden(View,x,y);

		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_METHOD, -1, -1);
		int ax = tx;
		tx = AddAddressOffset(View, tx, y);

		if (pParent->GetType() != nt_vtable)
			tx = AddText(View, tx, y, crType, NONE, "FunPtr");
		else
		{
			#ifdef _WIN64
			tx = AddText(View, tx, y, crFunction, NONE, "(%i)", offset / 8);
			#else
			tx = AddText(View, tx, y, crFunction, NONE, "(%i)", offset / 4);
			#endif
		}

		tx = AddIcon(View, tx, y, ICON_CAMERA, 0, HS_CLICK);
		tx += FontWidth;

		tx = AddText(View, tx, y, crFunction, 69, Name);
		tx += FontWidth;

		if (Assembly.size() > 0)
			tx = AddOpenClose(View, tx, y);

		tx += FontWidth;

		tx = AddComment(View,tx,y);

		if (bOpen[View.Level])
		{
			for (UINT i = 0; i < Assembly.size(); i++)
			{
				y += FontHeight;
				AddText(View, ax, y, crHex, NONE, Assembly[i]);
			}
		}

		return y += FontHeight;
	}
};

class CNodePtr : public CNodeBase
{
public:
	CNodeBase* pNode;
	CMemory Memory;

	virtual NodeType GetType()
	{
		return nt_pointer;
	};

	virtual void Update(HotSpot& Spot) { StandardUpdate(Spot); }

	virtual int GetMemorySize()
	{
		#ifdef _WIN64
		return 8;
		#else
		return 4;
		#endif
	}

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View,x,y);

		#ifdef _WIN64
		DWORD_PTR* pMemory = (DWORD_PTR*)&((BYTE*)View.pData)[offset];
		#else
		DWORD* pMemory = (DWORD*)&((BYTE*)View.pData)[offset];
		#endif
		

		//printf( "read ptr: %p\n", View.pData );
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		x = AddOpenClose(View, x, y);
		x = AddIcon(View, x, y, ICON_POINTER, -1, -1);

		int tx = x;
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "Ptr ");
		tx = AddText(View, tx, y, crName, 69, "%s",Name);
		tx = AddText(View, tx, y, crValue, NONE, " <%s>",pNode->Name);
		tx = AddIcon(View, tx, y, ICON_CHANGE, 4, HS_CHANGE_A);

		tx += FontWidth;
		tx = AddComment(View,tx,y);

		y += FontHeight;

		if (bOpen[View.Level])
		{
			DWORD NeededSize = pNode->GetMemorySize( );
			Memory.SetSize(NeededSize);
			ViewInfo newView;
			newView = View;
			newView.pData = Memory.pMemory;
			newView.Address = pMemory[0];

			ReadMemory(newView.Address,newView.pData,NeededSize);

			y = pNode->Draw(newView,x,y);
		}
		return y;
	}
};

class CNodeInt64 : public CNodeBase
{
public:
	virtual NodeType GetType(void) { return nt_int64; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int64 v = _atoi64(Spot.Text);
		if (Spot.ID == 0) WriteMemory( Spot.Address, &v, 8 );
	}

	virtual int GetMemorySize() { return 8; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		__int64* pMemory = (__int64*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_INTEGER, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "Int64 ");
		tx = AddText(View, tx, y, crName, 69, "%s",Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		tx = AddText(View, tx, y, crValue, 0, "%lli", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeInt32 : public CNodeBase
{
public:
	virtual NodeType GetType() {return nt_int32; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int32 v = atoi(Spot.Text);
		if (Spot.ID == 0) WriteMemory(Spot.Address,&v,4);
	}

	virtual int GetMemorySize(){ return 4; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		__int32* pMemory = (__int32*) &((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_INTEGER,NONE,NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "Int32 ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		tx = AddText(View, tx, y, crValue, 0, "%i", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};
class CNodeInt16 : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_int16; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int16 v = atoi(Spot.Text);
		if (Spot.ID == 0) WriteMemory(Spot.Address,&v,2);
	}

	virtual int GetMemorySize(){ return 2; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		__int16* pMemory = (__int16*) &((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_INTEGER, NONE, NONE);
		tx = AddAddressOffset(View,tx,y);
		tx = AddText(View, tx, y, crType, NONE, "Int16 ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		tx = AddText(View, tx, y, crValue, 0, "%i", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};
class CNodeInt8 : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_int8; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		__int8 v = atoi(Spot.Text);
		if (Spot.ID == 0) WriteMemory(Spot.Address,&v,1);
	}

	virtual int GetMemorySize() { return 1; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		__int8* pMemory = (__int8*) &((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_INTEGER, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "Int8  ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		tx = AddText(View, tx, y, crValue, 0, "%i", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeCustom : public CNodeBase
{
public:
	CNodeCustom()
	{
		Name = "Custom";
		memsize = 4;
	}

	DWORD memsize;

	virtual NodeType GetType() { return nt_custom; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0)
			memsize = atoi(Spot.Text);
	};

	virtual int GetMemorySize() { return memsize; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_CUSTOM, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "Custom ");
		tx = AddText(View, tx, y, crIndex, NONE, "[");
		tx = AddText(View, tx, y, crIndex, 0, "%i", memsize);
		tx = AddText(View, tx, y, crIndex, NONE, "] ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name) + FontWidth;
		tx = AddComment(View, tx, y);
		return y += FontHeight;
	}
};

class CNodeDWORD : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_uint32; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		DWORD v = atoi(Spot.Text);
		if (Spot.ID == 0) 
			WriteMemory(Spot.Address,&v,4);
	}

	virtual int GetMemorySize() { return 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		DWORD* pMemory = (DWORD*)&((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_UNSIGNED, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "DWORD ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		tx = AddText(View, tx, y, crValue, 0, "%u", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};
class CNodeWORD : public CNodeBase
{
public:
	virtual NodeType GetType( ) { return nt_uint16; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate( Spot);
		WORD v = atoi( Spot.Text );
		if (Spot.ID == 0)
			WriteMemory( Spot.Address, &v, 2 );

	}

	virtual int GetMemorySize( ) { return 2; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		WORD* pMemory = (WORD*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_UNSIGNED, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "WORD  ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		tx = AddText(View, tx, y, crValue, 0, "%u", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};
class CNodeBYTE : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_uint8; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		BYTE v = atoi(Spot.Text);
		if (Spot.ID == 0) WriteMemory(Spot.Address,&v,1);
	}

	virtual int GetMemorySize(){ return 1; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) 
			return DrawHidden(View,x,y);

		BYTE* pMemory = (BYTE*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_UNSIGNED, NONE, NONE );
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "BYTE  ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		tx = AddText(View, tx, y, crValue, 0, "%u", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeText : public CNodeBase
{
public:
	CNodeText()
	{
		Name = "Text";
		memsize = 4;
	}

	DWORD memsize;

	virtual NodeType GetType() { return nt_text; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0) 
			memsize = atoi(Spot.Text);

		if (Spot.ID == 1)
		{
			DWORD Length = Spot.Text.GetLength()+1;
			if (Length > memsize) Length = memsize;
			WriteMemory(Spot.Address,Spot.Text.GetBuffer(),Length);
		}
	}

	virtual int GetMemorySize() { return memsize; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		char* pMemory = (char*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View,tx,y,ICON_TEXT,NONE,NONE);
		tx = AddAddressOffset(View,tx,y);
		tx = AddText(View,tx,y,crType,NONE,"Text ");
		tx = AddText(View,tx,y,crName,69,"%s",Name);
		tx = AddText(View,tx,y,crIndex,NONE,"[");
		tx = AddText(View,tx,y,crIndex,0,"%i",memsize);
		tx = AddText(View,tx,y,crIndex,NONE,"]");

		CString asc = GetStringFromMemory(&pMemory[0],memsize);
		tx = AddText(View,tx,y,crChar,NONE," = '");
		tx = AddText(View,tx,y,crChar,1,"%s",asc);
		tx = AddText(View,tx,y,crChar,NONE,"' ") + FontWidth;

		tx = AddComment(View,tx,y);
		return y += FontHeight;
	}
};

class CNodePChar : public CNodeBase
{
public:
	CNodeBase* pNode;
	CMemory Memory;

	virtual NodeType GetType( ) { return nt_pchar; }

	virtual void Update( HotSpot& Spot )
	{
		StandardUpdate(Spot);
		__int64 v = _atoi64(Spot.Text);
		if (Spot.ID == 0) 
			WriteMemory( Spot.Address, &v, 8 );
	}

	virtual int GetMemorySize()
	{
		#ifdef _WIN64
		return 8;
		#else
		return 4;
		#endif
	}

	virtual int Draw( ViewInfo& View, int x, int y )
	{
		if (bHidden) 
			return DrawHidden(View,x,y);

		__int64* pMemory = (__int64*) &((BYTE*)View.pData)[offset];

		AddSelection(View, 0, y, FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);
		//AddAdd(View, x, y);

		int tx = x + 16;
		tx = AddIcon(View, tx,y, ICON_INTEGER, NONE, NONE);
		tx = AddAddressOffset( View, tx, y );
		tx = AddText(View, tx, y, crType, NONE, "PCHAR ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);

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

		CString asc;
		tx = AddText(View, tx, y, crChar, NONE, " = '");

		if (VALID(pMemory))
		{
			DWORD_PTR dw = pMemory[0];
			CString sc = ReadMemoryString(dw, 20);
			tx = AddText( View, tx, y, crChar, 1, sc.GetBuffer( ));
		} 
		else 
			asc = "";

		tx = AddText( View, tx, y, crChar, NONE, "' " ) + FontWidth;
		return y += FontHeight;
	}
};

class CNodeUnicode : public CNodeBase
{
public:
	CNodeUnicode( void )
	{
		Name = "Unicode";
		memsize = 4;
	}

	DWORD memsize;

	virtual NodeType GetType() { return nt_unicode; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID == 0) 
			memsize = atoi(Spot.Text);

		if (Spot.ID == 1)
		{
			DWORD Length = Spot.Text.GetLength() + 1;

			if (Length > memsize)
				Length = memsize;

			wchar_t* pwszConverted = new wchar_t[ Length + 1 ];

			const char* pszSource = Spot.Text.GetBuffer();

			for ( DWORD i = 0; i <= Length; i++ )
				pwszConverted[i] = pszSource[i];

			WriteMemory(Spot.Address, pwszConverted, Length * sizeof( wchar_t ));

			delete pwszConverted;
		}
	};

	virtual int GetMemorySize( void ) { return memsize; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) 
			return DrawHidden(View,x,y);

		wchar_t* pMemory = (wchar_t*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View,tx,y,ICON_TEXT,NONE,NONE);
		tx = AddAddressOffset(View,tx,y);
		tx = AddText(View, tx, y,crType,NONE,"Unicode ");
		tx = AddText(View, tx, y,crName,69,"%s",Name);
		tx = AddText(View, tx, y,crIndex,NONE,"[");
		tx = AddText(View, tx, y,crIndex,0,"%i",( memsize / sizeof( wchar_t ) ) );
		tx = AddText(View, tx, y,crIndex,NONE,"]");

		CStringW asc = GetStringFromMemoryW(&pMemory[0],memsize);
		tx = AddText(View, tx, y,crChar,NONE," = '");
		tx = AddText(View, tx, y,crChar,1,"%S",asc);
		tx = AddText(View, tx, y,crChar,NONE,"' ") + FontWidth;

		tx = AddComment(View, tx, y);
		return y += FontHeight;
	}
};

class CNodeFloat : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_float; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = atof(Spot.Text);
		if (Spot.ID == 0) 
			WriteMemory(Spot.Address  ,&v,4);
	}

	virtual int GetMemorySize() { return 4; }

	virtual int Draw(ViewInfo& View, int x, int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		float* pMemory = (float*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_FLOAT, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "float ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		//tx = AddText(View,tx,y,crValue,0,"%.4f",pMemory[0]) + FontWidth;

		//if ( *pMemory > -99999.0f && *pMemory < 99999.0f )
		//	*pMemory = 0;

		tx = AddText(View, tx, y, crValue, 0, "%4.3f", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeDouble : public CNodeBase
{
public:
	virtual NodeType GetType(void) { return nt_double; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate( Spot );
		double v = atof( Spot.Text );
		if (Spot.ID == 0) 
			WriteMemory( Spot.Address, &v, 8 );
	}

	virtual int GetMemorySize( void )
	{
		#ifdef _WIN64
		return 8;
		#else
		return 4;
		#endif
	}

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) 
			return DrawHidden(View,x,y);
		double* pMemory = (double*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);
		//AddAdd(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_FLOAT, NONE, NONE);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "double ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddText(View, tx, y, crName, NONE, " = ");
		//tx = AddText(View, tx, y, crValue, 0, "%.4lg", pMemory[0]) + FontWidth;
		tx = AddText(View, tx, y, crValue, 0, "%3.4lg", pMemory[0]) + FontWidth;
		tx = AddComment(View, tx, y);

		return y += FontHeight;
	}
};

class CNodeVec2 : public CNodeBase
{
public:
	CNodeVec2::CNodeVec2()
	{
		for (UINT i = 0; i < bOpen.size(); i++) 
			bOpen[i] = true;
	}

	virtual NodeType GetType() { return nt_vec2; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = atof(Spot.Text);
		if (Spot.ID == 0) 
			WriteMemory(Spot.Address + 0, &v, 4);
		if (Spot.ID == 1) 
			WriteMemory(Spot.Address + 4, &v, 4);
	};

	virtual int GetMemorySize() { return 4 + 4; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) 
			return DrawHidden(View,x,y);

		float* pMemory = (float*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View, x, y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_VECTOR,-1,-1);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "Vec2 ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddOpenClose(View,tx,y);
		if (bOpen[View.Level])
		{
			tx = AddText(View, tx, y, crName, NONE, "(");
			tx = AddText(View, tx, y, crValue, 0, "%0.3f",pMemory[0]);
			tx = AddText(View, tx, y, crName, NONE, ",");
			tx = AddText(View, tx, y, crValue, 1, "%0.3f",pMemory[1]);
			tx = AddText(View, tx, y, crName, NONE, ")");
		}
		tx += FontWidth;
		tx = AddComment(View, tx, y);
		return (y + FontHeight);
	}
};
class CNodeVec3 : public CNodeBase
{
public:
	CNodeVec3::CNodeVec3()
	{
		for (UINT i = 0; i < bOpen.size(); i++)
			bOpen[i] = true;
	}

	virtual NodeType GetType() { return nt_vec3; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = (float)atof(Spot.Text);
		if (Spot.ID == 0) WriteMemory(Spot.Address + 0, &v, 4);
		if (Spot.ID == 1) WriteMemory(Spot.Address + 4, &v, 4);
		if (Spot.ID == 2) WriteMemory(Spot.Address + 8, &v, 4);
	};

	virtual int GetMemorySize() { return 12; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		float* pMemory = (float*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View,tx,y,ICON_VECTOR,-1,-1);
		tx = AddAddressOffset(View,tx,y);
		tx = AddText(View,tx,y,crType,NONE,"Vec3 ");
		tx = AddText(View,tx,y,crName,69,"%s",Name);
		tx = AddOpenClose(View,tx,y);
		if (bOpen[View.Level])
		{
			tx = AddText(View,tx,y,crName,NONE,"(");
			tx = AddText(View,tx,y,crValue,0,"%0.3f",pMemory[0]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,1,"%0.3f",pMemory[1]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,2,"%0.3f",pMemory[2]);
			tx = AddText(View,tx,y,crName,NONE,")");
		}
		tx += FontWidth;
		tx = AddComment(View,tx,y);

		return y + FontHeight;
	}
};
class CNodeQuat : public CNodeBase
{
public:
	virtual NodeType GetType()
	{
		return nt_quat;
	};

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		float v = atof(Spot.Text);
		if (Spot.ID == 0) 
			WriteMemory(Spot.Address + 0, &v, 4);
		if (Spot.ID == 1) 				  	  
			WriteMemory(Spot.Address + 4, &v, 4);
		if (Spot.ID == 2) 				  	  
			WriteMemory(Spot.Address + 8, &v, 4);
		if (Spot.ID == 3) 					  
			WriteMemory(Spot.Address + 12,&v, 4);
	};

	virtual int GetMemorySize() { return 16; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) 
			return DrawHidden(View,x,y);

		float* pMemory = (float*)&((BYTE*)View.pData)[offset];
		AddSelection(View, 0, y, FontHeight);
		AddDelete(View,x,y);
		AddTypeDrop(View,x,y);

		int tx = x + 16;
		tx = AddIcon(View, tx, y, ICON_VECTOR, -1, -1);
		tx = AddAddressOffset(View, tx, y);
		tx = AddText(View, tx, y, crType, NONE, "Vec4 ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddOpenClose(View, tx, y);
		if (bOpen[View.Level])
		{
			tx = AddText(View, tx, y, crName, NONE, "(");
			tx = AddText(View, tx, y, crValue, 0, "%0.3f",pMemory[0]);
			tx = AddText(View, tx, y, crName, NONE, ",");
			tx = AddText(View, tx, y, crValue, 1, "%0.3f",pMemory[1]);
			tx = AddText(View, tx, y, crName, NONE, ",");
			tx = AddText(View, tx, y, crValue, 2, "%0.3f",pMemory[2]);
			tx = AddText(View, tx, y, crName, NONE, ",");
			tx = AddText(View, tx, y, crValue, 3, "%0.3f",pMemory[3]);
			tx = AddText(View, tx, y, crName, NONE, ")");
		}
		tx += FontWidth;
		tx = AddComment(View, tx, y);

		return y + FontHeight;
	}
};
class CNodeMatrix : public CNodeBase
{
public:
	virtual NodeType GetType() { return nt_matrix; };
	
	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		if (Spot.ID < 16)
		{
			float v = (float)atof(Spot.Text);
			DWORD o = Spot.ID * 4;
			WriteMemory(Spot.Address + o, &v, 4);
		}
	};
	
	virtual int GetMemorySize() { return 4*4*4; }

	virtual int Draw(ViewInfo& View,int x,int y)
	{
		if (bHidden) return DrawHidden(View,x,y);
		float* pMemory = (float*) &((BYTE*)View.pData)[offset];
		AddSelection(View,0,y,FontHeight);
		AddDelete(View, x, y);
		AddTypeDrop(View, x, y);

		int tx = x + 16;
		tx = AddIcon(View,tx,y,ICON_MATRIX,-1,-1);
		int mx = tx;
		tx = AddAddressOffset(View,tx,y);
		tx = AddText(View, tx, y, crType, NONE, "Matrix ");
		tx = AddText(View, tx, y, crName, 69, "%s", Name);
		tx = AddOpenClose(View,tx,y);
		tx += FontWidth;
		tx = AddComment(View,tx,y);

		if (bOpen[View.Level])
		{
			y += FontHeight;
			tx = mx;
			tx = AddText(View, tx, y, crName,NONE,"|");
			tx = AddText(View, tx, y, crValue,0,"% 14.3f",pMemory[0]);
			tx = AddText(View, tx, y, crName,NONE,",");
			tx = AddText(View, tx, y, crValue,1,"% 14.3f",pMemory[1]);
			tx = AddText(View, tx, y, crName,NONE,",");
			tx = AddText(View, tx, y, crValue,2,"% 14.3f",pMemory[2]);
			tx = AddText(View, tx, y, crName,NONE,",");
			tx = AddText(View, tx, y, crValue,3,"% 14.3f",pMemory[3]);
			tx = AddText(View, tx, y, crName,NONE,"|");
			y += FontHeight;
			tx = mx;
			tx = AddText(View,tx,y,crName,NONE,"|");
			tx = AddText(View,tx,y,crValue,4,"% 14.3f",pMemory[4]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,5,"% 14.3f",pMemory[5]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,6,"% 14.3f",pMemory[6]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,7,"% 14.3f",pMemory[7]);
			tx = AddText(View,tx,y,crName,NONE,"|");
			y += FontHeight;
			tx = mx;
			tx = AddText(View,tx,y,crName,NONE,"|");
			tx = AddText(View,tx,y,crValue,8,"% 14.3f",pMemory[8]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,9,"% 14.3f",pMemory[9]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,10,"% 14.3f",pMemory[10]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,11,"% 14.3f",pMemory[11]);
			tx = AddText(View,tx,y,crName,NONE,"|");
			y += FontHeight;
			tx = mx;
			tx = AddText(View,tx,y,crName,NONE,"|");
			tx = AddText(View,tx,y,crValue,12,"% 14.3f",pMemory[12]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,13,"% 14.3f",pMemory[13]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,14,"% 14.3f",pMemory[14]);
			tx = AddText(View,tx,y,crName,NONE,",");
			tx = AddText(View,tx,y,crValue,15,"% 14.3f",pMemory[15]);
			tx = AddText(View,tx,y,crName,NONE,"|");
		}

		return y + FontHeight;
	}
};

class CNodeArray : public CNodeBase
{
public:
	CNodeArray()
	{
		Total = 1;
		Current = 0;
	}

	CNodeBase* pNode;
	DWORD Total;
	DWORD Current;

	virtual NodeType GetType() { return nt_array; };

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
		int v = atoi(Spot.Text);
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

		tx = AddText(View, tx, y, crType,NONE,"Array ");
		tx = AddText(View, tx, y, crName,69,"%s",Name);
		tx = AddText(View, tx, y, crIndex,NONE,"[");
		tx = AddText(View, tx, y, crIndex,0,"%i",Total);
		tx = AddText(View, tx, y, crIndex,NONE,"]");
								  
		tx = AddIcon(View, tx, y, ICON_LEFT,2,HS_CLICK);
		tx = AddText(View, tx, y, crIndex,NONE,"(");
		tx = AddText(View, tx, y, crIndex,1,"%i",Current);
		tx = AddText(View, tx, y, crIndex,NONE,")");
		tx = AddIcon(View, tx, y, ICON_RIGHT,3,HS_CLICK);
								  
		tx = AddText(View, tx, y, crValue,NONE,"<%s Size=%i>",pNode->Name,GetMemorySize());
		tx = AddIcon(View, tx, y, ICON_CHANGE,4,HS_CHANGE_X);

		tx += FontWidth;
		tx = AddComment(View, tx, y);

		y += FontHeight;
		if (bOpen[View.Level])
		{
			ViewInfo newView;
			newView = View;
			//printf( "33333 set\n" );
			newView.Address = View.Address + offset + pNode->GetMemorySize() * Current;
			newView.pData	= (void*)((DWORD_PTR)View.pData + offset + pNode->GetMemorySize( ) * Current) ;
			y = pNode->Draw(newView,x,y);
		};
		return y;
	}
};

class CNodeClassInstance : public CNodeBase
{
public:
	CNodeClass* pNode;

	virtual NodeType GetType() { return nt_instance; }

	virtual void Update(HotSpot& Spot)
	{
		StandardUpdate(Spot);
	}

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
		tx = AddAddressOffset(View,tx,y);

		tx = AddText(View, tx, y, crType, NONE, "Instance ");

		tx = AddText(View, tx, y, crName, 69, "%s", Name);

		tx = AddText(View, tx, y, crValue, NONE, "<%s>", pNode->Name);
		tx = AddIcon(View, tx, y, ICON_CHANGE, 4, HS_CHANGE_X);

		tx += FontWidth;
		tx = AddComment(View, tx, y);

		y += FontHeight;
		if (bOpen[View.Level])
		{
			ViewInfo newView;

			newView = View;
			newView.Address = View.Address + offset;
			newView.pData =  (void*)((DWORD_PTR)newView.pData + offset);

			y = pNode->Draw(newView, x, y);
		}

		return y;
	}
};
