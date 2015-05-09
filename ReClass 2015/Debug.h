#pragma warning(disable : 4996)

#ifndef DEBUG_H
#define DEBUG_H

#include <fcntl.h>
#include <stdio.h>
#include <io.h>
#include <time.h>
#include <stdlib.h>
#include <string>

#define VALID( x ) ( x != NULL && HIWORD( x ) )

void sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock(); 
	while (goal > clock());
}

static void CreateConsole( )
{
	int hConHandle = 0;   
	HANDLE lStdHandle = 0;  
	FILE *fp = 0;
	AllocConsole( );
	lStdHandle = GetStdHandle( STD_OUTPUT_HANDLE );
	hConHandle = _open_osfhandle( PtrToUlong( lStdHandle ), _O_TEXT );
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
}

bool DataCompare( BYTE* pData, BYTE* bMask, char * szMask )
{
	for( ; *szMask; ++szMask, ++pData, ++bMask )
		if( *szMask == 'x' && *pData != *bMask )
			return FALSE;

	return ( *szMask == NULL );
}

PCHAR GetFilePath( PCHAR append )
{
	const int MAXPATH = 256;
	static char szFile[MAXPATH] = { };
	//static const char* const szFile[_MAX_PATH];

	GetModuleFileName( GetModuleHandle( NULL ), szFile, MAXPATH ); 
	for( int i = 0; i < ( int )strlen( szFile ); i++ )
	{
		if( szFile[ strlen( szFile ) - i ] == '\\' )
		{
			szFile[ ( strlen( szFile ) -  i ) + 1 ] = '\0';
			strcat_s( szFile, append );
			return szFile;
		}
	}

	return NULL;
}

DWORD WINAPI lpThreadWait( LPVOID lpParam )
{
	while( ( GetAsyncKeyState( VK_SPACE ) & 1 ) == 0 )
	{
		Sleep( 100 );
	}

	return 0;
}

void Pause( )
{
	printf( "\n\nPress the space bar to continue...\n" );

	WaitForSingleObject( CreateThread( 0, 0, lpThreadWait, 0, 0, 0 ), INFINITE );
}

std::wstring widestring( const std::string &text )
{
	std::wstring result;
	result.resize( text.length() );
	mbstowcs( &result[0], &text[0], text.length() );
	return result;
}

std::string narrowstring( const std::wstring &text )
{
	std::string result;
	result.resize( text.length( ) );
	wcstombs( &result[0], &text[0], text.length( ) );
	return result;
}

bool is_number( const std::string& s )
{
	std::string::const_iterator it = s.begin();
	while ( it != s.end() && isdigit( *it ) ) ++it;
	return !s.empty() && it == s.end();
}

template <class T>
void EndianSwap( T* pObj )
{
	unsigned char* pMem = reinterpret_cast<unsigned char*>( pObj );
	std::reverse( pMem, pMem + sizeof( T ) );
}


#endif