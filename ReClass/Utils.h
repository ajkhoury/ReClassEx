#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <afxstr.h> // For CString
#include <random>

#include "Native.h"
#include "Intrinsics.h"

namespace Utils
{

//
// Forward declarations
//
PVOID GetLocalProcAddressA( PVOID ModuleBase, PCHAR ProcName );
PVOID GetLocalProcAddressW( PVOID ModuleBase, LPCWSTR ProcName );
PVOID GetLocalModuleBaseA( LPCSTR ModuleName );
PVOID GetLocalModuleBaseW( LPCWSTR ModuleName );

#ifdef UNICODE
#define GetLocalModuleBase GetLocalModuleBaseW
#define GetLocalProcAddress GetLocalProcAddressW
#else
#define GetLocalModuleBase GetLocalModuleBaseA
#define GetLocalProcAddress GetLocalProcAddressA
#endif

bool SetPrivilege( HANDLE hToken, LPCTSTR lpPrivilege, BOOLEAN bEnablePrivilege );
bool SetDebugPrivilege( BOOLEAN bEnable );


#ifdef _DEBUG
//
//	Console API stuff
//
static FILE* ConsoleStream = 0;
static bool CreateDbgConsole( const TCHAR* lpConsoleTitle )
{
	if (!ConsoleStream)
	{
		if (!AllocConsole( ))
			return false;
		if (freopen_s( &ConsoleStream, "CONOUT$", "w", stdout ) != ERROR_SUCCESS)
			return false;
		SetConsoleTitle( lpConsoleTitle );
	}
	return true;
}
static void FreeDbgConsole( )
{
	if (ConsoleStream)
	{
		fclose( ConsoleStream );
		FreeConsole( );
		ConsoleStream = 0;
	}
}
static CString GetLastErrorString( )
{
	TCHAR buf[256];
	int size = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError( ), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), buf, 256, NULL );
	return CString( buf, size );
}
#endif

static size_t FindPattern( size_t startAddress, unsigned int searchSize, unsigned char pattern[], int patternSize )
{
    ULONG pos = 0;
    PUCHAR start = (PUCHAR)startAddress;
    PUCHAR end = start + searchSize - patternSize;
    PUCHAR ptr;

    for (ptr = start; ptr <= end; ptr++)
    {
        if (*ptr == pattern[pos] || pattern[pos] == 0xCC)
        {
            if (++pos == patternSize)
                return (ULONG_PTR)ptr - searchSize;
        }
        else
        {
            pos = 0;
        }
    }

    return 0;
}

template<int N> 
inline size_t FindPattern( size_t startAddress, unsigned int searchSize, unsigned char( &pattern )[N] )
{
	return FindPattern( startAddress, searchSize, pattern, N );
}

static CString GetVersionInfo( const TCHAR* versionKey )
{
    HRSRC hVersion;
    HGLOBAL hGlobal;
    LPVOID lpVersionInfo;

    struct LANGANDCODEPAGE {
        WORD wLanguage;
        WORD wCodePage;
    } *lpTranslate;
    UINT uiSize;
    
    BYTE* lpData;
    CString strQuery;

    CString strResult;

	hVersion = FindResource( NULL, MAKEINTRESOURCE( VS_VERSION_INFO ), VS_FILE_INFO );
	if (hVersion != NULL)
	{
		hGlobal = LoadResource( NULL, hVersion );
		if (hGlobal != NULL)
		{
            lpVersionInfo = LockResource( hGlobal );
			if (lpVersionInfo != NULL)
			{          
				if (VerQueryValue( lpVersionInfo, _T( "\\VarFileInfo\\Translation" ), (LPVOID*)&lpTranslate, &uiSize ) && uiSize > 0)
				{
					// Version information			
					strQuery.Format( _T( "\\StringFileInfo\\%04x%04x\\%s" ), lpTranslate->wLanguage, lpTranslate->wCodePage, versionKey );
					if (VerQueryValue( lpVersionInfo, strQuery.GetString( ), (LPVOID*)&lpData, &uiSize ) && uiSize > 0)
						strResult = (LPCTSTR)lpData;
				}
                ::UnlockResource( hGlobal );
			}
            ::FreeResource( hGlobal );
		}		
	}

	return strResult;
}

template<typename T>
static T* GenerateRandomString( T* RandomString, ULONG Length )
{
	std::random_device RandomDevice;
	std::mt19937_64 RNG( RandomDevice( ) );
	std::uniform_int_distribution<int> Distribution( '0', 'z' );

	ULONG count = 0;
	while (count < Length - 1)
	{
		T RandomChar = (T)Distribution( RNG );

		if (
			(RandomChar >= '0' && RandomChar <= '9') ||
			(RandomChar >= 'A' && RandomChar <= 'Z') ||
			(RandomChar >= 'a' && RandomChar <= 'z')
			)
		{
			RandomString[count] = RandomChar;
			count++;
		}
	}

	RandomString[Length - 1] = L'\0';

	return RandomString;
}

enum OSType {
	UnknownOS = 0,
	Win2000 = 0x4105,
	WinXP = 0x4106,
	WinVista = 0x4107,
	Windows7 = 0x4108,
	Windows8 = 0x4109,
	Windows10 = 0x4110,
	Windows = 0x4000,
};

enum PlatformType {
    UnknownPlatform = 0,
    ProcessPlatformX86 = 1,
    ProcessPlatformX64 = 2,
    ProcessPlatformNotSupported = -1
};

bool IsWindowsVersionOrLater( OSType target );
LONG GetProcessorArchitecture( );
OSType GetOperatingSystemType( );
int GetProcessPlatform( HANDLE hProcess );
HANDLE NtCreateThreadEx( HANDLE hProcess, LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD createFlags, DWORD* threadId );
HANDLE NtCreateThread( LPVOID lpRemoteThreadStart, LPVOID lpParam, DWORD createFlags, DWORD* threadId = NULL );

};