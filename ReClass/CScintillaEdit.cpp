#include "stdafx.h"
#include "CScintillaEdit.h"

CScintillaEdit::CScintillaEdit( ) :
    m_pSciMsg( NULL ),
    m_pSciWndData( NULL )
{ 
}

BOOL CScintillaEdit::Create( DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID )
{
    CWnd* pWnd = this;
    BOOL ret = FALSE;

    ret = pWnd->Create( _T( "Scintilla" ), NULL, dwStyle, rect, pParentWnd, nID );
    if (ret == TRUE)
    {
        m_pSciMsg = (SciFnDirect)::SendMessage( m_hWnd, SCI_GETDIRECTFUNCTION, 0, 0 );
        m_pSciWndData = (sptr_t)::SendMessage( m_hWnd, SCI_GETDIRECTPOINTER, 0, 0 );
    }

    return ret;
}

CScintillaEdit::~CScintillaEdit( )
{
    DestroyWindow( );
}

sptr_t CScintillaEdit::SendEditor( UINT Msg, WPARAM wParam, LPARAM lParam )
{
    return ::SendMessage( m_hWnd, Msg, wParam, lParam );
    //return m_pSciMsg( m_pSciWndData, Msg, wParam, lParam );
}

//
// Lexer
//

void CScintillaEdit::SetLexer( int lexer )
{
    SendEditor( SCI_SETLEXER, lexer );
}

int CScintillaEdit::GetLexer( void )
{
    return (int)SendEditor( SCI_GETLEXER );
}

void CScintillaEdit::SetLexerLanguage( const char* language )
{
    SendEditor( SCI_SETLEXERLANGUAGE, (WPARAM)0, (LPARAM)language );
}

void CScintillaEdit::LoadLexerLibrary( const char* path )
{
    SendEditor( SCI_LOADLEXERLIBRARY, (WPARAM)0, (LPARAM)path );
}

void CScintillaEdit::Colorise( int start, int end )
{
    SendEditor( SCI_COLOURISE, (WPARAM)start, (LPARAM)end );
}

void CScintillaEdit::SetProperty( const char* key, const char* value )
{
    SendEditor( SCI_SETPROPERTY, (WPARAM)key, (LPARAM)value );
}

int CScintillaEdit::GetProperty( const char* key, char* value )
{
    return (int)SendEditor( SCI_GETPROPERTY, (WPARAM)key, (LPARAM)value );
}

int CScintillaEdit::GetPropertyExpanded( const char* key, char* value )
{
    return (int)SendEditor( SCI_GETPROPERTYEXPANDED, (WPARAM)key, (LPARAM)value );
}

int CScintillaEdit::GetPropertyInt( const char* key, int defaultValue )
{
    return (int)SendEditor( SCI_GETPROPERTYINT, (WPARAM)key, (LPARAM)defaultValue );
}

void CScintillaEdit::SetKeywords( int keyWordSet, const char* keyWords )
{
    SendEditor( SCI_SETKEYWORDS, (WPARAM)keyWordSet, (LPARAM)keyWords );
}

//
// Styles
//

void CScintillaEdit::SetStyleBits( int bits )
{
    SendEditor( SCI_SETSTYLEBITS, (WPARAM)bits );
}

int CScintillaEdit::GetStyleBits( void )
{
    return (int)SendEditor( SCI_GETSTYLEBITS );
}

int CScintillaEdit::GetStyleBitsNeeded( void )
{
    return (int)SendEditor( SCI_GETSTYLEBITSNEEDED );
}

void CScintillaEdit::ResetDefaultStyle( void )
{
    SendEditor( SCI_STYLERESETDEFAULT );
}

void CScintillaEdit::SetAllStylesDefault( void )
{
    SendEditor( SCI_STYLECLEARALL );
}

void CScintillaEdit::SetFont( int style, const char* fontName )
{
    SendEditor( SCI_STYLESETFONT, (WPARAM)style, (LPARAM)fontName );
}

void CScintillaEdit::SetFont( int style, CString fontName )
{
    #ifdef UNICODE
    SendEditor( SCI_STYLESETFONT, (WPARAM)style, (LPARAM)CW2A(fontName).m_psz );
    #else
    SendEditor( SCI_STYLESETFONT, (WPARAM)style, (LPARAM)fontName.GetBuffer( ) );
    #endif
}

void CScintillaEdit::SetSize( int style, int sizePoints )
{
    SendEditor( SCI_STYLESETSIZE, (WPARAM)style, (LPARAM)sizePoints );
}

void CScintillaEdit::SetFontBold( int style, BOOLEAN bold )
{
    SendEditor( SCI_STYLESETBOLD, (WPARAM)style, (LPARAM)bold );
}

void CScintillaEdit::SetFontItalic( int style, BOOLEAN italic )
{
    SendEditor( SCI_STYLESETITALIC, (WPARAM)style, (LPARAM)italic );
}

void CScintillaEdit::SetFontUnderline( int style, BOOLEAN underline )
{
    SendEditor( SCI_STYLESETUNDERLINE, (WPARAM)style, (LPARAM)underline );
}

void CScintillaEdit::SetForeground( int style, COLORREF fore )
{
    SendEditor( SCI_STYLESETFORE, (WPARAM)style, (LPARAM)fore );
}

void CScintillaEdit::SetBackground( int style, COLORREF back )
{
    SendEditor( SCI_STYLESETBACK, (WPARAM)style, (LPARAM)back );
}

void CScintillaEdit::SetEndOfLineFilled( int style, BOOLEAN eolFilled )
{
    SendEditor( SCI_STYLESETEOLFILLED, (WPARAM)style, (LPARAM)eolFilled );
}

void CScintillaEdit::SetCharacterSet( int style, int characterSet )
{
    SendEditor( SCI_STYLESETCHARACTERSET, (WPARAM)style, (LPARAM)characterSet );
}

void CScintillaEdit::SetCase( int style, int caseVisible )
{
    SendEditor( SCI_STYLESETCASE, (WPARAM)style, (LPARAM)caseVisible );
}

void CScintillaEdit::SetVisible( int style, BOOLEAN visible )
{
    SendEditor( SCI_STYLESETVISIBLE, (WPARAM)style, (LPARAM)visible );
}

void CScintillaEdit::SetHotspot( int style, BOOLEAN hotspot )
{
    SendEditor( SCI_STYLESETHOTSPOT, (WPARAM)style, (LPARAM)hotspot );
}

void CScintillaEdit::SetSelectionForeground( BOOLEAN useSetting, COLORREF fore )
{
    SendEditor( SCI_SETSELFORE, (WPARAM)useSetting, (LPARAM)fore );
}

void CScintillaEdit::SetSelectionBackground( BOOLEAN useSetting, COLORREF back )
{
    SendEditor( SCI_SETSELBACK, (WPARAM)useSetting, (LPARAM)back );
}

void CScintillaEdit::SetSelectionAlpha( int alpha )
{
    SendEditor( SCI_SETSELALPHA, (WPARAM)alpha );
}

int CScintillaEdit::GetSelectionAlpha( void )
{
    return (int)SendEditor( SCI_GETSELALPHA );
}

void CScintillaEdit::SetCaretForeground( COLORREF fore )
{
    SendEditor( SCI_SETCARETFORE, (WPARAM)fore );
}

COLORREF CScintillaEdit::GetCaretForeground( void )
{
    return (COLORREF)SendEditor( SCI_GETCARETFORE );
}

void CScintillaEdit::SetCaretLineVisible( BOOLEAN show )
{
    SendEditor( SCI_SETCARETLINEVISIBLE, (WPARAM)show );
}

BOOLEAN CScintillaEdit::IsCaretLineVisible( void )
{
    return (BOOLEAN)SendEditor( SCI_GETCARETLINEVISIBLE );
}

void CScintillaEdit::SetCaretLineBackground( COLORREF back )
{
    SendEditor( SCI_SETCARETLINEBACK, (WPARAM)back );
}

COLORREF CScintillaEdit::GetCaretLineBackground( void )
{
    return (COLORREF)SendEditor( SCI_GETCARETLINEBACK );
}

void CScintillaEdit::SetCaretLineBackgroundAlpha( int alpha )
{
    SendEditor( SCI_SETCARETLINEBACKALPHA, (WPARAM)alpha );
}

int CScintillaEdit::GetCaretLineBackgroundAlpha( void )
{
    return (int)SendEditor( SCI_GETCARETLINEBACKALPHA );
}

void CScintillaEdit::SetCaretPeriod( int periodMilliseconds )
{
    SendEditor( SCI_SETCARETPERIOD, (WPARAM)periodMilliseconds );
}

int CScintillaEdit::GetCaretPeriod( void )
{
    return (int)SendEditor( SCI_GETCARETPERIOD );
}

void CScintillaEdit::SetCaretWidth( int pixelWidth )
{
    SendEditor( SCI_SETCARETWIDTH, (WPARAM)pixelWidth );
}

int CScintillaEdit::GetCaretWidth( void )
{
    return (int)SendEditor( SCI_GETCARETWIDTH );
}

int CScintillaEdit::GetFirstVisibleLine( void )
{
    return (int)SendEditor( SCI_GETFIRSTVISIBLELINE );
}

void CScintillaEdit::SetOffsetX( int xOffset )
{
    SendEditor( SCI_SETXOFFSET, (WPARAM)xOffset );
}

int CScintillaEdit::GetOffsetX( void )
{
    return (int)SendEditor( SCI_GETXOFFSET );
}

void CScintillaEdit::LineScroll( int columns, int lines )
{
    SendEditor( SCI_LINESCROLL, (WPARAM)columns, (LPARAM)lines );
}

void CScintillaEdit::ScrollCaret( void )
{
    SendEditor( SCI_SCROLLCARET );
}

void CScintillaEdit::SetCaretPolicyX( int caretPolicy, int caretSlop )
{
    SendEditor( SCI_SETXCARETPOLICY, (WPARAM)caretPolicy, (LPARAM)caretSlop );
}

void CScintillaEdit::SetCaretPolicyY( int caretPolicy, int caretSlop )
{
    SendEditor( SCI_SETYCARETPOLICY, (WPARAM)caretPolicy, (LPARAM)caretSlop );
}

void CScintillaEdit::SetVisiblePolicy( int visiblePolicy, int visibleSlop )
{
    SendEditor( SCI_SETVISIBLEPOLICY, (WPARAM)visiblePolicy, (LPARAM)visibleSlop );
}

void CScintillaEdit::SetHorizontalScrollVisible( BOOLEAN visible )
{
    SendEditor( SCI_SETHSCROLLBAR, (WPARAM)visible );
}

BOOLEAN CScintillaEdit::IsHorizontalScrollVisible( void )
{
    return (BOOLEAN)SendEditor( SCI_GETHSCROLLBAR );
}

void CScintillaEdit::SetVerticalScrollVisible( BOOLEAN visible )
{
    SendEditor( SCI_SETVSCROLLBAR, (WPARAM)visible );
}

BOOLEAN CScintillaEdit::IsVerticalScrollVisible( void )
{
    return (BOOLEAN)SendEditor( SCI_GETVSCROLLBAR );
}

void CScintillaEdit::SetScrollWidth( int pixelWidth )
{
    SendEditor( SCI_SETSCROLLWIDTH, (WPARAM)pixelWidth );
}

int CScintillaEdit::GetScrollWidth( void )
{
    return (int)SendEditor( SCI_GETSCROLLWIDTH );
}

void CScintillaEdit::SetScrollEndAtLastLine( BOOLEAN endAtLastLine )
{
    SendEditor( SCI_SETENDATLASTLINE, (WPARAM)endAtLastLine );
}

BOOLEAN CScintillaEdit::DoesScrollEndAtLastLine( void )
{
    return (BOOLEAN)SendEditor( SCI_GETENDATLASTLINE );
}

//
// Margins
//

void CScintillaEdit::SetMarginType( int margin, int marginType )
{
    SendEditor( SCI_SETMARGINTYPEN, (WPARAM)margin, (LPARAM)marginType );
}

int CScintillaEdit::GetMarginType( int margin )
{
    return (int)SendEditor( SCI_GETMARGINTYPEN, (WPARAM)margin );
}

void CScintillaEdit::SetMarginWidth( int margin, int pixelWidth )
{
    SendEditor( SCI_SETMARGINWIDTHN, (WPARAM)margin, (LPARAM)pixelWidth );
}

int CScintillaEdit::GetMarginWidth( int margin )
{
    return (int)SendEditor( SCI_GETMARGINWIDTHN, (WPARAM)margin );
}

void CScintillaEdit::SetMarginMask( int margin, int mask )
{
    SendEditor( SCI_SETMARGINMASKN, (WPARAM)margin, (LPARAM)mask );
}

int CScintillaEdit::GetMarginMask( int margin )
{
    return (int)SendEditor( SCI_GETMARGINMASKN, (WPARAM)margin );
}

void CScintillaEdit::SetMarginSensitive( int margin, BOOLEAN sensitive )
{
    SendEditor( SCI_SETMARGINSENSITIVEN, (WPARAM)margin, (LPARAM)sensitive );
}

BOOLEAN CScintillaEdit::GetMarginSensitive( int margin )
{
    return (BOOLEAN)SendEditor( SCI_GETMARGINSENSITIVEN, (WPARAM)margin );
}

void CScintillaEdit::SetMarginLeft( int pixelWidth )
{
    SendEditor( SCI_SETMARGINLEFT, (WPARAM)0, (LPARAM)pixelWidth );
}

int CScintillaEdit::GetMarginLeft( void )
{
    return (int)SendEditor( SCI_GETMARGINLEFT );
}

void CScintillaEdit::SetMarginRight( int pixelWidth )
{
    SendEditor( SCI_SETMARGINRIGHT, (WPARAM)0, (LPARAM)pixelWidth );
}

int CScintillaEdit::GetMarginRight( void )
{
    return (int)SendEditor( SCI_GETMARGINRIGHT );
}

void CScintillaEdit::SetFoldMarginColor( BOOLEAN useSetting, COLORREF back )
{
    SendEditor( SCI_SETFOLDMARGINCOLOUR, (WPARAM)useSetting, (LPARAM)back );
}

void CScintillaEdit::SetFoldMarginHighlightColor( BOOLEAN useSetting, COLORREF fore )
{
    SendEditor( SCI_SETFOLDMARGINHICOLOUR, (WPARAM)useSetting, (LPARAM)fore );
}

//
// Tabs and Indentation Guides 
//

void CScintillaEdit::SetTabWidth( int tabWidth )
{
    SendEditor( SCI_SETTABWIDTH, (WPARAM)tabWidth );
}

int CScintillaEdit::GetTabWidth( void )
{
    return (int)SendEditor( SCI_GETTABWIDTH );
}

void CScintillaEdit::SetIndent( int indentSize )
{
    SendEditor( SCI_SETINDENT, (WPARAM)indentSize );
}

int CScintillaEdit::GetIndent( void )
{
    return (int)SendEditor( SCI_GETINDENT );
}

void CScintillaEdit::SetTabIndents( BOOLEAN tabIndents )
{
    SendEditor( SCI_SETTABINDENTS, (WPARAM)tabIndents );
}

BOOLEAN CScintillaEdit::IsTabIndents( void )
{
    return (SendEditor( SCI_GETTABINDENTS ) != FALSE) ? TRUE : FALSE;
}

void CScintillaEdit::SetLineIndentation( int line, int indentation )
{
    SendEditor( SCI_SETLINEINDENTATION, (WPARAM)line, (LPARAM)indentation );
}

int CScintillaEdit::GetLineIndentation( int line )
{
    return (int)SendEditor( SCI_GETLINEINDENTATION, (WPARAM)line );
}

int CScintillaEdit::GetLineIndentPosition( int line )
{
    return (int)SendEditor( SCI_GETLINEINDENTPOSITION, (WPARAM)line );
}

void CScintillaEdit::SetIndentationGuides( int indentView )
{
    SendEditor( SCI_SETINDENTATIONGUIDES, (WPARAM)indentView );
}

int CScintillaEdit::GetIndentationGuides( void )
{
    return (int)SendEditor( SCI_GETINDENTATIONGUIDES );
}

void CScintillaEdit::SetHighlightGuide( int column )
{
    SendEditor( SCI_SETHIGHLIGHTGUIDE, (WPARAM)column );
}

int CScintillaEdit::GetHighlightGuide( void )
{
    return (int)SendEditor( SCI_GETHIGHLIGHTGUIDE );
}

DWORD CScintillaEdit::GetLength( void )
{
    return (DWORD)SendEditor( SCI_GETLENGTH );
}

DWORD CScintillaEdit::GetTextLength( void )
{
    return (DWORD)SendEditor( SCI_GETTEXTLENGTH );
}

DWORD CScintillaEdit::GetLineCount( void )
{
    return (DWORD)SendEditor( SCI_GETLINECOUNT );
}

DWORD CScintillaEdit::LinesOnScreen( void )
{
    return (DWORD)SendEditor( SCI_LINESONSCREEN );
}

BOOLEAN CScintillaEdit::GetModify( void )
{
    return (BOOLEAN)SendEditor( SCI_GETMODIFY );
}

void CScintillaEdit::SetSelection( int anchor, int caret )
{
    SendEditor( SCI_SETSEL, (WPARAM)anchor, (LPARAM)caret );
}

void CScintillaEdit::GoToPosition( int caret )
{
    SendEditor( SCI_GOTOPOS, (WPARAM)caret );
}

void CScintillaEdit::GoToLine( int line )
{
    SendEditor( SCI_GOTOLINE, (WPARAM)line );
}

void CScintillaEdit::SetCurrentPosition( int caret )
{
    SendEditor( SCI_SETCURRENTPOS, (WPARAM)caret );
}

int CScintillaEdit::GetCurrentPosition( void )
{
    return (int)SendEditor( SCI_GETCURRENTPOS );
}

void CScintillaEdit::SetAnchor( int anchor )
{
    SendEditor( SCI_SETANCHOR, (WPARAM)anchor );
}

int CScintillaEdit::GetAnchor( void )
{
    return (int)SendEditor( SCI_GETANCHOR );
}

void CScintillaEdit::SetSelectionStart( int anchor )
{
    SendEditor( SCI_SETSELECTIONSTART, (WPARAM)anchor );
}

void CScintillaEdit::SetSelectionEnd( int caret )
{
    SendEditor( SCI_SETSELECTIONEND, (WPARAM)caret );
}

int CScintillaEdit::GetSelectionStart( void )
{
    return (int)SendEditor( SCI_GETSELECTIONSTART );
}

int CScintillaEdit::GetSelectionEnd( void )
{
    return (int)SendEditor( SCI_GETSELECTIONEND );
}

void CScintillaEdit::SelectAll( void )
{
    SendEditor( SCI_SELECTALL );
}

int CScintillaEdit::LineFromPosition( int pos )
{
    return (int)SendEditor( SCI_LINEFROMPOSITION, (WPARAM)pos );
}

int CScintillaEdit::PositionFromLine( int line )
{
    return (int)SendEditor( SCI_POSITIONFROMLINE, (WPARAM)line );
}

int CScintillaEdit::GetLineEndPosition( int line )
{
    return (int)SendEditor( SCI_GETLINEENDPOSITION, (WPARAM)line );
}

DWORD CScintillaEdit::LineLength( int line )
{
    return (DWORD)SendEditor( SCI_LINELENGTH, (WPARAM)line );
}

CString CScintillaEdit::GetSelectedText( void )
{
    CString OutString;
    CHAR* Buffer = NULL;
    DWORD BufferSize = (DWORD)SendEditor( SCI_GETSELTEXT, (WPARAM)0, (LPARAM)NULL );
    if (BufferSize > 0)
    {
        Buffer = new CHAR[BufferSize + 1];
        SendEditor( SCI_GETSELTEXT, (WPARAM)0, (LPARAM)Buffer );
        #ifdef UNICODE
        OutString.SetString( CA2W( Buffer ), BufferSize );
        #else
        OutString.SetString( Buffer, BufferSize );
        #endif
        delete[] Buffer;
    }
    return OutString;
}

CString CScintillaEdit::GetCurrentLine( void )
{
    CString OutString;
    CHAR* Buffer = NULL;
    DWORD BufferSize = (DWORD)SendEditor( SCI_GETCURLINE, (WPARAM)0, (LPARAM)NULL );
    if (BufferSize > 0)
    {
        Buffer = new CHAR[BufferSize + 1];
        SendEditor( SCI_GETCURLINE, (WPARAM)BufferSize, (LPARAM)Buffer );
        #ifdef UNICODE
        OutString.SetString( CA2W( Buffer ), BufferSize );
        #else
        OutString.SetString( Buffer, BufferSize );
        #endif
        delete[] Buffer;
    }
    return OutString;
}

BOOLEAN CScintillaEdit::SelectionIsRectangle( void )
{
    return (SendEditor( SCI_SELECTIONISRECTANGLE ) != FALSE);
}

void CScintillaEdit::SetSelectionMode( int selectionMode )
{
    SendEditor( SCI_SETSELECTIONMODE, (WPARAM)selectionMode );
}

int CScintillaEdit::GetSelectionMode( void )
{
    return (int)SendEditor( SCI_GETSELECTIONMODE );
}

int CScintillaEdit::GetSelectedLineStartPosition( int line )
{
    return (int)SendEditor( SCI_GETLINESELSTARTPOSITION, (WPARAM)line );
}

int CScintillaEdit::GetSelectedLineEndPosition( int line )
{
    return (int)SendEditor( SCI_GETLINESELENDPOSITION, (WPARAM)line );
}

int CScintillaEdit::TextHeight( void )
{
    return (int)SendEditor( SCI_TEXTHEIGHT );
}

int CScintillaEdit::PositionFromPoint( int x, int y )
{
    return (int)SendEditor( SCI_POSITIONFROMPOINT, (WPARAM)x, (LPARAM)y );
}

int CScintillaEdit::PositionFromPointClose( int x, int y )
{
    return (int)SendEditor( SCI_POSITIONFROMPOINTCLOSE, (WPARAM)x, (LPARAM)y );
}

int CScintillaEdit::PointXFromPosition( int pos )
{
    return (int)SendEditor( SCI_POINTXFROMPOSITION, (WPARAM)0, (LPARAM)pos );
}

int CScintillaEdit::PointYFromPosition( int pos )
{
    return (int)SendEditor( SCI_POINTYFROMPOSITION, (WPARAM)0, (LPARAM)pos );
}

CString CScintillaEdit::GetText( void )
{
    CString OutString;
    CHAR* Buffer = NULL;
    DWORD BufferSize = (DWORD)GetLength( ) + 1;
    if (BufferSize > 0)
    {
        Buffer = new CHAR[BufferSize];
        SendEditor( SCI_GETTEXT, (WPARAM)BufferSize, (LPARAM)Buffer );
        #ifdef UNICODE
        OutString.SetString( CA2W( Buffer ), BufferSize );
        #else
        OutString.SetString( Buffer, BufferSize );
        #endif
        delete[] Buffer;
    }
    return OutString;
}

void CScintillaEdit::SetText( CString text )
{
    #ifdef UNICODE
    SendEditor( SCI_SETTEXT, (WPARAM)0, (LPARAM)CW2A( text.GetBuffer( ) ).m_psz );
    #else
    SendEditor( SCI_SETTEXT, (WPARAM)0, (LPARAM)text.GetBuffer( ) );
    #endif
}

CString CScintillaEdit::GetLine( int line )
{
    CString OutString;
    CHAR* Buffer = NULL;
    DWORD BufferSize = (DWORD)LineLength( line ) + 1;
    if (BufferSize > 0)
    {
        Buffer = new CHAR[BufferSize];
        BufferSize = (DWORD)SendEditor( SCI_GETLINE, (WPARAM)line, (LPARAM)Buffer );
        Buffer[BufferSize] = '\0';
        #ifdef UNICODE
        OutString.SetString( CA2W( Buffer ), BufferSize );
        #else
        OutString.SetString( Buffer, BufferSize );
        #endif
        delete[] Buffer;
    }
    return OutString;
}

void CScintillaEdit::ReplaceSelected( CString text )
{
    #ifdef UNICODE
    SendEditor( SCI_REPLACESEL, (WPARAM)0, (LPARAM)CW2A( text.GetBuffer( ) ).m_psz );
    #else
    SendEditor( SCI_REPLACESEL, (WPARAM)0, (LPARAM)text.GetBuffer( ) );
    #endif
}

void CScintillaEdit::SetReadOnly( BOOLEAN readOnly )
{
    SendEditor( SCI_SETREADONLY, (WPARAM)readOnly );
}

BOOLEAN CScintillaEdit::IsReadOnly( void )
{
    return (BOOLEAN)SendEditor( SCI_GETREADONLY );
}

void CScintillaEdit::AddText( CString text, ULONG length )
{
    if (length)
    {
        #ifdef UNICODE
        SendEditor( SCI_ADDTEXT, (WPARAM)length, (LPARAM)CW2A( text.GetBuffer( ) ).m_psz );
        #else
        SendEditor( SCI_ADDTEXT, (WPARAM)length, (LPARAM)text.GetBuffer( ) );
        #endif
    }
    else
    {
        #ifdef UNICODE
        SendEditor( SCI_ADDTEXT, (WPARAM)text.GetLength( ), (LPARAM)CW2A( text.GetBuffer( ) ).m_psz );
        #else
        SendEditor( SCI_ADDTEXT, (WPARAM)text.GetLength( ), (LPARAM)text.GetBuffer( ) );
        #endif
    }
}

void CScintillaEdit::AppendText( CString text, ULONG length )
{
    if (length)
    {
        #ifdef UNICODE
        SendEditor( SCI_APPENDTEXT, (WPARAM)length, (LPARAM)CW2A( text.GetBuffer( ) ).m_psz );
        #else
        SendEditor( SCI_APPENDTEXT, (WPARAM)length, (LPARAM)text.GetBuffer( ) );
        #endif
    }
    else
    {
        #ifdef UNICODE
        SendEditor( SCI_APPENDTEXT, (WPARAM)text.GetLength( ), (LPARAM)CW2A( text.GetBuffer( ) ).m_psz );
        #else
        SendEditor( SCI_APPENDTEXT, (WPARAM)text.GetLength( ), (LPARAM)text.GetBuffer( ) );
        #endif
    }
}

void CScintillaEdit::AppendText( const char* text, ULONG length )
{
    if (length)
    {
        SendEditor( SCI_APPENDTEXT, (WPARAM)length, (LPARAM)text );
    }
    else
    {
        ULONG textLength = (ULONG)strnlen_s( text, 1024 );
        SendEditor( SCI_APPENDTEXT, (WPARAM)textLength, (LPARAM)text );
    }
}

void CScintillaEdit::InsertText( CString text, int pos )
{
    #ifdef UNICODE
    SendEditor( SCI_INSERTTEXT, (WPARAM)pos, (LPARAM)CW2A( text.GetBuffer( ) ).m_psz );
    #else
    SendEditor( SCI_INSERTTEXT, (WPARAM)pos, (LPARAM)text.GetBuffer( ) );
    #endif
}

TCHAR CScintillaEdit::CharAt( int pos )
{
    return (TCHAR)SendEditor( SCI_GETCHARAT, (WPARAM)pos );
}

void CScintillaEdit::Clear( void )
{
    SendEditor( SCI_CLEARALL );
}
