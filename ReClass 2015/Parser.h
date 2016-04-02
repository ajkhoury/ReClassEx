#ifndef PARSER_H
#define PARSER_H
//
//#include <iostream>
//#include <string>
//#include <regex>
//#include <iterator>
//
//#define MAX_CURSOR 9999
//
//int GetNextWord( CString code, unsigned int pointer, CString &out )
//{
//	// skip 1 unless we're just starting out
//	if ( pointer != 0 )
//		pointer++;
//
//	// Find first instance of '\n' ';' ' ' or '\t'
//	int brk = code.Find(_T(' '), pointer );
//
//	if ( brk == -1 ) 
//		brk = code.Find(_T(';'),  pointer );
//	if ( brk == -1 ) 
//		brk = code.Find(_T('\n'), pointer );
//	if ( brk == -1 ) 
//		brk = code.Find(_T('\t'), pointer );
//	if ( brk == -1 ) 
//	{
//		brk = code.Find(_T('{'),  pointer );
//		PrintOut(_T("<{>"));
//	};
//	if ( brk == -1 ) brk = code.Find(_T('}'),  pointer );
//
//	// Can't find anything
//
//	if ( brk == -1 )
//		return MAX_CURSOR;
//
//	out = code.Mid( pointer, brk - pointer );
//	out.Replace(_T("\n"), _T(""));
//	out.Replace(_T("\r"), _T(""));
//
//	PrintOut(_T("%s\n"), out.GetBuffer( ) );
//
//	return brk;
//}
//
//CString RemoveComments( CString code, bool multiline )
//{
//	// the shittiest solution to single line regex match
//	if ( multiline )
//		code.Replace(_T("\r\n"), _T("R3$86"));
//
//	// Convert from my CString to std::string
//	CT2CA pszBuffer( code );
//	std::string str( pszBuffer );
//
//	// unescaped:        '//.*?$|/ \*.*? \*/| \'(?:\\.|[^\\\ '])*\ '| "(?:\\.|[^\\ "])*"'
//	std::tr1::regex rx1("//.*?$|/\\*.*?\\*/|\\'(?:\\.|[^\\\\'])*\\'|\"(?:\\.|[^\\\"])*" );
//	
//	//std::tr1::regex rx1( "//.*?$|/\\*.*?\\*/|\\'(?:\\.|[^\\\\'])*\\'|\"(?:\\\\.|[^\\\\\"])*" );
//
//	str = std::regex_replace( str, rx1, std::string( "" ) );
//
//	// Convert back to CString, might break if NULL characters
//	CString code_parsed( str.c_str( ) );
//
//	// put back the new line characters
//	if ( multiline )
//		code_parsed.Replace("R3$86", "\r\n" );
//
//	return code_parsed;
//}
//
//void Parse( CString code )
//{
//	// Step 1. Remove Tabs
//	code.Replace( "\t", " " ); code.Replace( "   ", " " ); code.Replace( "  ", " " );
//	
//	// Step 2. Remove Comments
//	//code = RemoveComments( code, true ); // filter regural comments
//	code = RemoveComments( code, false );  // filter multiline comment blocks
//
//	printf( "%s\n", code.GetBuffer( ) );
//
//	/*int cursor = 0;
//	while ( cursor < 300 )
//	{
//		CString out;
//
//		// skip the space or character you are on
//		cursor = GetNextWord( code, cursor, out );
//
//		if ( cursor == MAX_CURSOR )
//		{
//			printf( "\n Error Occured\n" );
//			break;
//		}
//
//		//printf( "[%s]", out.GetBuffer( ) );
//	}*/
//
//};
//
#endif


// http://stackoverflow.com/questions/241327/python-snippet-to-remove-c-and-c-comments
// '//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
