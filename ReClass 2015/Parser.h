#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <regex>
#include <iterator>

#define MAX_CURSOR 9999

int GetNextWord( CString code, unsigned int pointer, CString &out )
{
	// skip 1 unless we're just starting out
	if ( pointer != 0 )
		pointer++;

	// Find first instance of '\n' ';' ' ' or '\t'
	int brk = code.Find( ' ', pointer );

	if ( brk == -1 ) brk = code.Find( ';',  pointer );
	if ( brk == -1 ) brk = code.Find( '\n', pointer );
	if ( brk == -1 ) brk = code.Find( '\t', pointer );
	if ( brk == -1 ) 
	{
		brk = code.Find( '{',  pointer );
		printf( "<{>" );
	};
	if ( brk == -1 ) brk = code.Find( '}',  pointer );

	// Can't find anything

	if ( brk == -1 )
		return MAX_CURSOR;

	out = code.Mid( pointer, brk - pointer );
	out.Replace( "\n", "" );
	out.Replace( "\r", "" );

	printf( "%s\n", out.GetBuffer( ) );

	return brk;
}

CString RemoveComments( CString code, bool multiline )
{
	// the shittiest solution to single line regex match
	if ( multiline )
		code.Replace( "\r\n", "R3$86" );

	// Convert from my CString to std::string
	CT2CA pszBuffer( code );
	std::string str( pszBuffer );

	// unescaped:        '//.*?$|/ \*.*? \*/| \'(?:\\.|[^\\\ '])*\ '| "(?:\\.|[^\\ "])*"'
	std::tr1::regex rx1( "//.*?$|/\\*.*?\\*/|\\'(?:\\.|[^\\\\'])*\\'|\"(?:\\.|[^\\\"])*" );
	
	//std::tr1::regex rx1( "//.*?$|/\\*.*?\\*/|\\'(?:\\.|[^\\\\'])*\\'|\"(?:\\\\.|[^\\\\\"])*" );

	str = std::regex_replace( str, rx1, std::string( "" ) );

	// Convert back to CString, might break if NULL characters
	CString code_parsed( str.c_str( ) );

	// put back the new line characters
	if ( multiline )
		code_parsed.Replace( "R3$86", "\r\n" );

	return code_parsed;
}

void Parse( CString code )
{
	// Step 1. Remove Tabs
	code.Replace( "\t", " " ); code.Replace( "   ", " " ); code.Replace( "  ", " " );
	
	// Step 2. Remove Comments
	//code = RemoveComments( code, true ); // filter regural comments
	code = RemoveComments( code, false );  // filter multiline comment blocks

	printf( "%s\n", code.GetBuffer( ) );

	/*int cursor = 0;
	while ( cursor < 300 )
	{
		CString out;

		// skip the space or character you are on
		cursor = GetNextWord( code, cursor, out );

		if ( cursor == MAX_CURSOR )
		{
			printf( "\n Error Occured\n" );
			break;
		}

		//printf( "[%s]", out.GetBuffer( ) );
	}*/

};

#endif


// http://stackoverflow.com/questions/241327/python-snippet-to-remove-c-and-c-comments
// '//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',

/*
class ProjectileEntityData
{
public:
    char pad_0x0000[96];
    __int64 m_Name; //0x0060 char* Gameplay/Weapons/Common/Bullets/9x19mm_Pistol 
    char pad_0x0068[40];
    float m_InitialSpeed; //0x0090 
    float m_TimeToLive; //0x0094 
    __int32 m_MaxCount; //0x0098 
    float m_InitMeshHideTime; //0x009C 
    float m_VisualConvergeDistance; //0x00A0 
    float m_VisualConvergenceDelay; //0x00A4 
    float m_VisualConvergenceDuration; //0x00A8 
    float m_ProxyVisualConvergenceDelay; //0x00AC 
    float m_ProxyVisualConvergenceDuration; //0x00B0 
    char pad_0x00B4[20];
    __int64 m_AmmunitionType; //0x00C8 char* 9x19mm
    char pad_0x00D0[8];
    BYTE m_DetonateOnTimeout; //0x00D8 
    BYTE m_ServerProjectileDisabled; //0x00D9 
    char pad_0x00DA[6];
};//Size=0x00E0
*/