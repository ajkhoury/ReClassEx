#pragma once

#include "afxwin.h"

// Scintilla Colors structure
typedef struct _SCINTILLA_COLORS {
    int iItem;
    COLORREF rgb;
} SCINTILLA_COLORS, *PSCINTILLA_COLORS;

class CScintillaEdit : public CWnd {
public:
    CScintillaEdit( );
    virtual ~CScintillaEdit( );

    BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

    // Lexer
    void SetLexer( int lexer );
    int GetLexer( void );
    void SetLexerLanguage( const char* language );
    void LoadLexerLibrary( const char* path );
    void Colorise( int start, int end );
    void SetProperty( const char* key, const char* value );
    int GetProperty( const char* key, char* value );
    int GetPropertyExpanded( const char* key, char* value );
    int GetPropertyInt( const char* key, int defaultValue );
    void SetKeywords( int keyWordSet, const char* keyWords );

    // Styles
    void SetStyleBits( int bits );
    int GetStyleBits( void );
    int GetStyleBitsNeeded( void );
    void ResetDefaultStyle( void );
    void SetAllStylesDefault( void );
    void SetFont( int style, const char* fontName );
    void SetFont( int style, CString fontName );
    void SetSize( int style, int sizePoints );
    void SetFontBold( int style, BOOLEAN bold );
    void SetFontItalic( int style, BOOLEAN italic );
    void SetFontUnderline( int style, BOOLEAN underline );
    void SetForeground( int style, COLORREF fore );
    void SetBackground( int style, COLORREF back );
    void SetEndOfLineFilled( int style, BOOLEAN eolFilled );
    void SetCharacterSet( int style, int characterSet );
    void SetCase( int style, int caseVisible );
    void SetVisible( int style, BOOLEAN visible );
    void SetHotspot( int style, BOOLEAN hotspot );
    void SetSelectionForeground( BOOLEAN useSetting, COLORREF fore );
    void SetSelectionBackground( BOOLEAN useSetting, COLORREF back );
    void SetSelectionAlpha( int alpha );
    int GetSelectionAlpha( void );
    void SetCaretForeground( COLORREF fore );
    COLORREF GetCaretForeground( void );
    void SetCaretLineVisible( BOOLEAN show );
    BOOLEAN IsCaretLineVisible( void );
    void SetCaretLineBackground( COLORREF back );
    COLORREF GetCaretLineBackground( void );
    void SetCaretLineBackgroundAlpha( int alpha );
    int GetCaretLineBackgroundAlpha( void );
    void SetCaretPeriod( int periodMilliseconds );
    int GetCaretPeriod( void );
    void SetCaretWidth( int pixelWidth );
    int GetCaretWidth( void );

    // Scrolling
    int GetFirstVisibleLine( void );
    void SetOffsetX( int xOffset );
    int GetOffsetX( void );
    void LineScroll( int columns, int lines );
    void ScrollCaret( void );
    void SetCaretPolicyX( int caretPolicy, int caretSlop );
    void SetCaretPolicyY( int caretPolicy, int caretSlop );
    void SetVisiblePolicy( int visiblePolicy, int visibleSlop );
    void SetHorizontalScrollVisible( BOOLEAN visible );
    BOOLEAN IsHorizontalScrollVisible( void );
    void SetVerticalScrollVisible( BOOLEAN visible );
    BOOLEAN IsVerticalScrollVisible( void );
    void SetScrollWidth( int pixelWidth );
    int GetScrollWidth( void );
    void SetScrollEndAtLastLine( BOOLEAN endAtLastLine );
    BOOLEAN DoesScrollEndAtLastLine( void );

    // Margins
    void SetMarginType( int margin, int marginType );
    int GetMarginType( int margin );
    void SetMarginWidth( int margin, int pixelWidth );
    int GetMarginWidth( int margin );
    void SetMarginMask( int margin, int mask );
    int GetMarginMask( int margin );
    void SetMarginSensitive( int margin, BOOLEAN mask );
    BOOLEAN GetMarginSensitive( int margin );
    void SetMarginLeft( int pixelWidth );
    int GetMarginLeft( void );
    void SetMarginRight( int pixelWidth );
    int GetMarginRight( void );
    void SetFoldMarginColor( BOOLEAN useSetting, COLORREF back );
    void SetFoldMarginHighlightColor( BOOLEAN useSetting, COLORREF fore );

    // Tabs and Indentation Guides
    void SetTabWidth( int tabWidth );
    int GetTabWidth( void );
    void SetIndent( int indentSize );
    int GetIndent( void );
    void SetTabIndents( BOOLEAN tabIndents );
    BOOLEAN IsTabIndents( void );
    void SetLineIndentation( int line, int indentation );
    int	GetLineIndentation( int line );
    int GetLineIndentPosition( int line );
    void SetIndentationGuides( int indentView );
    int GetIndentationGuides( void );
    void SetHighlightGuide( int column );
    int GetHighlightGuide( void );

    // Selection and information
    DWORD GetLength( void );
    DWORD GetTextLength( void );
    DWORD GetLineCount( void );
    DWORD LinesOnScreen( void );
    BOOLEAN GetModify( void );
    void SetSelection( int anchor, int caret );
    void GoToPosition( int caret );
    void GoToLine( int line );
    void SetCurrentPosition( int caret );
    int GetCurrentPosition( );
    void SetAnchor( int anchor );
    int GetAnchor( void );
    void SetSelectionStart( int anchor );
    void SetSelectionEnd( int caret );
    int GetSelectionStart( void );
    int GetSelectionEnd( void );
    void SelectAll( void );
    int LineFromPosition( int pos );
    int PositionFromLine( int line );
    int GetLineEndPosition( int line );
    DWORD LineLength( int line );
    CString GetSelectedText( void );
    CString GetCurrentLine( void );
    BOOLEAN SelectionIsRectangle( void );
    void SetSelectionMode( int selectionMode );
    int GetSelectionMode( void );
    int GetSelectedLineStartPosition( int line );
    int GetSelectedLineEndPosition( int line );
    int TextHeight( void );
    int PositionFromPoint( int x, int y );
    int PositionFromPointClose( int x, int y );
    int PointXFromPosition( int pos );
    int PointYFromPosition( int pos );

    // Text retrieval and modification
    CString GetText( void );
    void SetText( CString text );
    CString GetLine( int line );
    void ReplaceSelected( CString text );
    void SetReadOnly( BOOLEAN readOnly );
    BOOLEAN IsReadOnly( void );
    void AddText( CString text, ULONG length = 0 );
    void AppendText( CString text, ULONG length = 0 );
    void AppendText( const char* text, ULONG length = 0 );
    void InsertText( CString text, int pos );
    TCHAR CharAt( int pos );
    void Clear( void );

private:
    sptr_t SendEditor( UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0 );

    SciFnDirect m_pSciMsg;
    sptr_t m_pSciWndData;
};