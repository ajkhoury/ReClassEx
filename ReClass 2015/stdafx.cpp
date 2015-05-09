#include "stdafx.h"

//Globals
HANDLE hProcess		= NULL;
DWORD ProcessID		= NULL;

DWORD NodeCreateIndex = 0;

COLORREF crBackground	= RGB(255,255,255);
COLORREF crSelect		= RGB(240,240,240);
COLORREF crHidden		= RGB(240,240,240);

COLORREF crOffset		= RGB(255,0,0);
COLORREF crAddress		= RGB(0,200,0);
COLORREF crType			= RGB(0,0,255);
COLORREF crName			= RGB(32,32,128);
COLORREF crIndex		= RGB(32,200,200);
COLORREF crValue		= RGB(255,128,0);
COLORREF crComment		= RGB(0,200,0);

COLORREF crVTable		= RGB(0,255,0);
COLORREF crFunction		= RGB(255,0,255);
COLORREF crChar			= RGB(0,0,255);
COLORREF crCustom		= RGB(64,128,64);
COLORREF crHex			= RGB(0,0,0);

CFont Font;
//CFont SmallFont;
int FontWidth;
int FontHeight;

bool gbAddress = true;
bool gbOffset = true;
bool gbText = true;
bool gbFloat = true;
bool gbInt = true;
bool gbString = true;
bool gbPointers = true;

bool gbTop = true;
bool gbClassBrowser = true;
bool gbFilterProcesses = false;

CString tdHex("char");
CString tdInt64("__int64");
CString tdInt32("__int32");
CString tdInt16("__int16");
CString tdInt8("__int8");
CString tdDWORD("DWORD");
CString tdWORD("WORD");
CString tdBYTE("unsigned char");
CString tdVec2("Vector2");
CString tdVec3("Vector3");
CString tdQuat("Vector4");
CString tdMatrix("matrix3x4_t");
CString tdPChar("char*");

std::vector<HICON> Icons;

void ReadMemory( DWORD_PTR Address, void* Buffer, DWORD Size )
{
	if ( ReadProcessMemory( hProcess, ( void* )Address, Buffer, Size, NULL ) == 0)
	{
		// printf( "[!] Failed to read memory @ %I64x GetLastError( ) = %p\n", Address, GetLastError( ) );
		ZeroMemory( Buffer, Size );
	}
}

void WriteMemory(DWORD_PTR Address,void* Buffer,DWORD Size)
{
	DWORD OldProtect;
//	VirtualProtectEx  (hProcess,(void*)Address,Size,PAGE_READWRITE,&OldProtect); <- srsly PAGE_READWRITE? O_o
	VirtualProtectEx  (hProcess,(void*)Address,Size,PAGE_EXECUTE_READWRITE,&OldProtect);
	WriteProcessMemory(hProcess,(void*)Address,Buffer,Size,NULL);
	VirtualProtectEx  (hProcess,(void*)Address,Size,OldProtect,NULL);
	//{
	//	CString e;
	//	e.Format("Error: %0.8X (%i) %0.8X<-%i",GetLastError(),GetLastError(),Address,((BYTE*)Buffer)[0]);
	//	MessageBox(NULL,e,"Info",MB_OK);
	//}
}

CString ReadMemoryString( DWORD_PTR address, SIZE_T max )
{
	char	buffer[1024];
	SIZE_T	bytesRead;

	if ( ReadProcessMemory( hProcess, ( PVOID )address, buffer, max, &bytesRead ) != 0 )
	{
		//printf( "Read: %p\n", address );
		//printf( "Bytes Read: %d\n", bytesRead );
		//printf( "String %s\n", buffer );
		
		for ( int i = 0; i < bytesRead; i++ )
		{
			// If not a printable character and is not null terminator replace with '.'
			if ( !( isprint( buffer[i] & 0xFF ) ) && buffer[i] != '\0' )
			{
				buffer[i] = '.';
			}
		}

		// Terminate at max
		buffer[bytesRead] = '\0';

		return buffer;
	} else {
		//printf( "Failed to read memory, GetLastError( )=%p\n", GetLastError( ) );
		return "..";
	}
}

void PauseResumeThreadList( bool bResumeThread ) 
{ 
	if (hProcess == NULL) return;
	DWORD dwOwnerPID = GetProcessId(hProcess);

	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
	if (hThreadSnap == INVALID_HANDLE_VALUE) return; 

	THREADENTRY32 te32;
	ZeroMemory(&te32,sizeof(te32));
	te32.dwSize = sizeof(THREADENTRY32); 

	BOOL MoreThreads = Thread32First(hThreadSnap, &te32);
	while (MoreThreads)
	{
		if (te32.th32OwnerProcessID == dwOwnerPID) 
		{
			HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
			if (bResumeThread)
				ResumeThread(hThread);
			else
				SuspendThread(hThread);
			CloseHandle(hThread);
		} 
		MoreThreads = Thread32Next(hThreadSnap, &te32);
	}
	CloseHandle (hThreadSnap); 
}

std::vector<MemMapInfo> MemMap;
std::vector<MemMapInfo> MemMapCode;
std::vector<MemMapInfo> MemMapData;
std::vector<MemMapInfo> MemMapModule;
std::vector<AddressName> Exports;
std::vector<AddressName> CustomNames;

bool IsCode(DWORD_PTR Address)
{
	for (UINT i=0; i < MemMapCode.size();i++)
	{
		if (Address >= MemMapCode[i].Start && Address <= MemMapCode[i].End)
			return true;
	}
	return false;
}

bool IsData(DWORD_PTR Address)
{
	for (UINT i=0; i < MemMapData.size();i++)
	{
		if (Address >= MemMapData[i].Start && Address <= MemMapData[i].End)
			return true;
	}
	return false;
}

DWORD_PTR GetBase( )
{
	if ( MemMap.size( ) > 1 )
		MemMap.at( 0 ).Start;

	//for (UINT i=0; i < MemMap.size();i++)
	//{
	//	if ( Address >= MemMap[i].Start && Address <= MemMap[i].End )
	//		return true;
	//}
#ifdef _WIN64
	return 0x140000000;
#else
	return 0x400000;
#endif
}

bool IsMemory(DWORD_PTR Address)
{
	for (UINT i=0; i < MemMap.size();i++)
	{
		if (Address >= MemMap[i].Start && Address <= MemMap[i].End)
			return true;
	}
	return false;
}

bool IsModule(DWORD_PTR Address)
{
	for (UINT i=0; i < MemMapModule.size();i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
			return true;
	}
	return false;
}

CString GetAddressName(DWORD_PTR Address, bool bHEX)
{
	CString txt;
	for (UINT i = 0; i < CustomNames.size(); i++)
	{
		if (Address == CustomNames[i].Address)
		{
			#ifdef _WIN64
			txt.Format("%s.%I64X", CustomNames[i].Name, Address);
			#else
			txt.Format("%s.%X", CustomNames[i].Name, Address);
			#endif

			return txt;
		}
	}
	for (UINT i=0; i < Exports.size();i++)
	{
		if (Address == Exports[i].Address)
		{
			#ifdef _WIN64
			txt.Format("%s.%I64X", Exports[i].Name, Address);
			#else
			txt.Format("%s.%X", Exports[i].Name, Address);
			#endif

			return txt;
		}
	}
	for (UINT i=0; i < MemMapCode.size(); i++)
	{
		if (Address >= MemMapCode[i].Start && Address <= MemMapCode[i].End)
		{
			#ifdef _WIN64
			txt.Format("<CODE>%s.%I64X", MemMapCode[i].Name, Address);
			#else
			txt.Format("<CODE>%s.%X", MemMapCode[i].Name, Address);
			#endif

			return txt;
		}
	}
	for (UINT i=0; i < MemMapData.size(); i++)
	{
		if (Address >= MemMapData[i].Start && Address <= MemMapData[i].End)
		{
			#ifdef _WIN64
			txt.Format("<DATA>%s.%I64X", MemMapData[i].Name, Address);
			#else
			txt.Format("<DATA>%s.%X", MemMapData[i].Name, Address);
			#endif

			return txt;
		}
	}
	for (UINT i=0; i < MemMapModule.size();i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
		{
			#ifdef _WIN64
			txt.Format("%s.%I64X", MemMapModule[i].Name, Address);
			#else
			txt.Format("%s.%X", MemMapModule[i].Name, Address);
			#endif

			return txt;
		}
	}
	for (UINT i=0; i < MemMap.size();i++)
	{
		if (Address >= MemMap[i].Start && Address <= MemMap[i].End)
		{
			#ifdef _WIN64
			txt.Format("%I64X", Address);
			#else
			txt.Format("%X", Address);
			#endif

			return txt;
		}
	}

	if (bHEX)
	{
		#ifdef _WIN64
		txt.Format("%I64X", Address);
		#else
		txt.Format("%X", Address);
		#endif
	}


	return txt;
}

CString GetModuleName(DWORD_PTR Address)
{
	for ( unsigned int i=0; i < MemMapModule.size();i++)
	{
		if (Address >= MemMapModule[i].Start && Address <= MemMapModule[i].End)
			return MemMapModule[i].Name;
	}
	return "<unknown>";
}

bool IsProcHandleValid( HANDLE hProc )
{
	if( !hProc )
		return false;
	const DWORD RetVal = WaitForSingleObject( hProc, 0 );
	if( RetVal == WAIT_FAILED ) 
		return false;
	return ( RetVal == WAIT_TIMEOUT );
}

#define MAX_EXPORTS 16384
void UpdateMemoryMap(void)
{
	MemMap.clear();
	MemMapCode.clear();
	MemMapData.clear();
	MemMapModule.clear();
	Exports.clear();
	CustomNames.clear();

	if (hProcess == NULL)
		return;
	if (!IsProcHandleValid(hProcess))
	{
		hProcess = NULL;
		return;
	}

	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);

	MEMORY_BASIC_INFORMATION MBI;
	ULONG_PTR pMemory = (ULONG_PTR)SysInfo.lpMinimumApplicationAddress;
	while(pMemory < (ULONG_PTR)SysInfo.lpMaximumApplicationAddress)
	{
		if ( VirtualQueryEx(hProcess,(LPCVOID)pMemory,&MBI,sizeof(MBI)) != 0)
		{
			if (MBI.State == MEM_COMMIT /*&& MBI.Type == MEM_PRIVATE*/)
			{
				MemMapInfo Mem;
				Mem.Start = (DWORD_PTR)pMemory;
				Mem.End = (DWORD_PTR)pMemory + MBI.RegionSize - 1;
				MemMap.push_back(Mem);
			}
			pMemory = (ULONG_PTR)MBI.BaseAddress + MBI.RegionSize;
		}
		else
		{
			pMemory += 1024;
		}
	}

	MODULEENTRY32	ModInfo;
	ModInfo.dwSize		= sizeof(MODULEENTRY32);

	HANDLE ModuleList = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetProcessId(hProcess));
	if ((int)ModuleList != -1)
	{
		BOOL rm = Module32First(ModuleList,&ModInfo);
		while(rm)
		{
			MemMapInfo Mem;
			Mem.Start = (DWORD_PTR)ModInfo.modBaseAddr;
			Mem.End = Mem.Start + ModInfo.modBaseSize;
			Mem.Name = ModInfo.szModule;
			MemMapModule.push_back(Mem);

			rm = Module32Next(ModuleList,&ModInfo);
		}
		CloseHandle(ModuleList);
	}

	AddressName Entry;
	ModuleList = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetProcessId(hProcess));
	if ((int)ModuleList != -1)
	{
		BOOL rm = Module32First(ModuleList,&ModInfo);
		while(rm)
		{
			IMAGE_DOS_HEADER DH;
			IMAGE_NT_HEADERS pe;

			ReadProcessMemory(hProcess,ModInfo.modBaseAddr,&DH,sizeof(DH),NULL);
			ReadProcessMemory(hProcess,ModInfo.modBaseAddr + DH.e_lfanew,&pe,sizeof(pe),NULL);
			DWORD ssize = (DWORD)pe.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
			PIMAGE_SECTION_HEADER sh = (PIMAGE_SECTION_HEADER)(new char[ssize]);
			ReadProcessMemory(hProcess,ModInfo.modBaseAddr + DH.e_lfanew + sizeof(IMAGE_NT_HEADERS),sh,ssize,NULL);
			for (int i=0; i < pe.FileHeader.NumberOfSections; i++)
			{
				CString txt;
				MemMapInfo Mem;
				txt.Format("%.8s",sh[i].Name); txt.MakeLower();
				if (txt == ".text" || txt == "code")
				{
					Mem.Start	= (DWORD_PTR)ModInfo.modBaseAddr + sh[i].VirtualAddress;
					Mem.End		= Mem.Start + sh[i].Misc.VirtualSize;
					Mem.Name	= ModInfo.szModule;
					MemMapCode.push_back(Mem);
				}
				if (txt == ".data" || txt == "data" || txt == ".rdata" || txt == ".idata")
				{
					Mem.Start	= (DWORD_PTR)ModInfo.modBaseAddr + sh[i].VirtualAddress;
					Mem.End		= Mem.Start + sh[i].Misc.VirtualSize;
					Mem.Name	= ModInfo.szModule;
					MemMapData.push_back(Mem);
				}
			}
			delete sh;

			rm = Module32Next(ModuleList,&ModInfo);
		}
		CloseHandle(ModuleList);
	}

	for (UINT i=0; i<MemMap.size();i++)
	{
		if (IsModule(MemMap[i].Start)) MemMap[i].Name = GetModuleName(MemMap[i].Start);
	}
}

void UpdateExports()
{
	Exports.clear();
	//if (!gbExports) return;

	MODULEENTRY32	ModInfo;
	ModInfo.dwSize		= sizeof(MODULEENTRY32);
	AddressName Entry;

	HANDLE ModuleList = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetProcessId(hProcess));
	if ((int)ModuleList != -1)
	{
		BOOL rm = Module32First(ModuleList,&ModInfo);
		while(rm)
		{
			IMAGE_DOS_HEADER DH;
			IMAGE_NT_HEADERS pe;

			ReadProcessMemory(hProcess,ModInfo.modBaseAddr,&DH,sizeof(DH),NULL);
			ReadProcessMemory(hProcess,ModInfo.modBaseAddr + DH.e_lfanew,&pe,sizeof(pe),NULL);

			if (pe.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != 0)
			{
				IMAGE_EXPORT_DIRECTORY ExpDir;
				ReadProcessMemory(hProcess,ModInfo.modBaseAddr + pe.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,&ExpDir,sizeof(ExpDir),NULL);
				LONG pName		= (LONG)(ExpDir.AddressOfNames			+ ModInfo.modBaseAddr);
				LONG pOrd		= (LONG)(ExpDir.AddressOfNameOrdinals	+ ModInfo.modBaseAddr);
				LONG pAddress	= (LONG)(ExpDir.AddressOfFunctions		+ ModInfo.modBaseAddr);

				LONG aName[MAX_EXPORTS];
				WORD aOrd[MAX_EXPORTS];
				LONG aAddress[MAX_EXPORTS];
				ReadProcessMemory(hProcess,(LPCVOID)pName,	 aName,		ExpDir.NumberOfNames*sizeof(aName[0]),NULL);
				ReadProcessMemory(hProcess,(LPCVOID)pOrd,	 aOrd,		ExpDir.NumberOfNames*sizeof(aOrd[0]),NULL);
				ReadProcessMemory(hProcess,(LPCVOID)pAddress,aAddress,	ExpDir.NumberOfFunctions*sizeof(aAddress[0]),NULL);

				for (DWORD i=0; i < ExpDir.NumberOfNames; i++)
				{
					char TextBuffer[1024];
					
					ReadProcessMemory(hProcess,(LPCVOID)(aName[i]+ (LONG)ModInfo.modBaseAddr),TextBuffer,255,NULL);
					LONG Address = aAddress[aOrd[i]] + (LONG)ModInfo.modBaseAddr;

					Entry.Name.Format("%s.%s",ModInfo.szModule,TextBuffer);
					Entry.Address = Address;
					Exports.push_back(Entry);
				}
			}

			rm = Module32Next(ModuleList,&ModInfo);
		}
		CloseHandle(ModuleList);
	}
}

__int64 StrToNum(const TCHAR *udata, int udatalen, int base)
{
	long index;
	const TCHAR numdigits[] = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	long digitValue = 0;
	__int64 RetVal = 0;
	TCHAR digits[sizeof(numdigits)+1];
	TCHAR *dataVal;
	TCHAR data[512] ;
	//copy the data to our variable
	_tcscpy(data, udata);
	//convert it to upper case
	_tcsupr(data);
	ZeroMemory(digits, sizeof(digits));
	//copy the number of digits supported by base in digits
	_tcsncpy(digits, numdigits, base);

	//printf( "ptr %s %d\n", udata, udatalen );

	for ( index = 0; index < udatalen; index++ )
	{
		//is the number there
		dataVal = _tcschr( digits, data[index] );
		//

		if(dataVal != 0 )
		{
			//if it is subtract where to start point
			digitValue = long( dataVal - digits );
			//printf( "->%X", digitValue );
			//increment Retval with digitvalue
			RetVal = RetVal * base + digitValue;

			//printf( " ( %p )", RetVal );
		}
	}

	// printf( "retval %p\n", RetVal );

	//return the result
	return RetVal;
}

int SplitString(const CString& input, const CString& delimiter, CStringArray& results)
{
	int iPos = 0;
	int newPos = -1;
	int sizeS2 = delimiter.GetLength();
	int isize = input.GetLength();

	CArray<INT, int> positions;

	newPos = input.Find (delimiter, 0);

	if( newPos < 0 ) { return 0; }

	int numFound = 0;

	while( newPos > iPos )
	{
		numFound++;
		positions.Add(newPos);
		iPos = newPos;
		newPos = input.Find (delimiter, iPos+sizeS2+1);
	}

	for( int i=0; i <= positions.GetSize(); i++ )
	{
		CString s;
		if( i == 0 )
			s = input.Mid( i, positions[i] );
		else
		{
			int offset = positions[i-1] + sizeS2;
			if( offset < isize )
			{
				if( i == positions.GetSize() )
					s = input.Mid(offset);
				else if( i > 0 )
					s = input.Mid( positions[i-1] + sizeS2, 
					positions[i] - positions[i-1] - sizeS2 );
			}
		}
		if( s.GetLength() > 0 )
			results.Add(s);
	}
	return numFound;
}

DWORD_PTR ConvertStrToAddress(CString Address)
{
	CStringArray chuncks;

	if ( SplitString( Address, "+", chuncks ) == 0 )
		chuncks.Add( Address );

	DWORD_PTR Final = 0;

	for ( UINT i=0; i < ( UINT )chuncks.GetCount( ); i++)
	{
		CString a = chuncks[i];
		a.MakeLower();
		a.Trim();

		bool bPointer = false;
		bool bMod =  false;

		if (a[0] == '*')
		{
			bPointer = true;
			a = a.Mid(1);
		}
		if (a[0] == '&')
		{
			bMod = true;
			a = a.Mid(1);
		}

		DWORD_PTR curadd = 0;

		if (bMod)
		{
			for (UINT i=0; i < MemMapModule.size( ); i++ )
			{
				CString ModName = MemMapModule[i].Name;
				ModName.MakeLower();
				if (StrStr( ModName, a ) != NULL)
				{
					curadd = MemMapModule[i].Start;
					bMod = true;
					break;
				}
			}
		} else {

			curadd = (__int64)StrToNum( a.GetBuffer( ), a.GetLength( ), 16 );
			//printf( "Final [%p] %d\n", curadd, a.GetLength( ) );
		}

		Final += curadd;

		if ( bPointer )
		{
			//printf( "here2\n" );
			if ( !ReadProcessMemory( hProcess, ( PVOID )Final, &Final, sizeof( Final ), NULL ) )
			{
				printf( "[!] Failed to read memory (stdafx.cpp) GetLastError( ) = %p\n", GetLastError( ) );
			}
		}
	}


	return Final;
}
