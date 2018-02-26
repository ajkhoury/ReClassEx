// DialogConsole.cpp : implementation file
//

#include "stdafx.h"
#include "DialogConsole.h"
#include "ReClassEx.h"
#include "afxdialogex.h"

// Console keywords
static const char g_ConsoleKeyWords[] =

// Standard
"asm auto bool break case catch char class const "
"const_cast continue default delete do double "
"dynamic_cast else enum explicit extern false finally "
"float for friend goto if inline int long mutable "
"namespace new operator private protected public "
"register reinterpret_cast register return short signed "
"sizeof static static_cast struct switch template "
"this throw true try typedef typeid typename "
"union unsigned using virtual void volatile "
"wchar_t while "

//Specific
"__int32 __int16 __int8 DWORD WORD BYTE D3DXVECTOR2 D3DXVECTOR3 D3DXQUATERNION D3DXMATRIX "

// Extended
"__asm __asume __based __box __cdecl __declspec "
"__delegate delegate depreciated dllexport dllimport "
"event __event __except __fastcall __finally __forceinline "
"__int8 __int16 __int32 __int64 __int128 __interface "
"interface __leave naked noinline __noop noreturn "
"nothrow novtable nullptr safecast __stdcall "
"__try __except __finally __unaligned uuid __uuidof "
"__virtual_inheritance";

// Default color scheme
static SCINTILLA_COLORS g_rgbSyntaxConsole[] = {
    { SCE_C_COMMENT,		RGB_GREEN },
    { SCE_C_COMMENTLINE,	RGB_GREEN },
    { SCE_C_COMMENTDOC,		RGB_DARKGREEN },
    { SCE_C_NUMBER,			RGB_DARKYELLOW },
    { SCE_C_STRING,			RGB_ORANGE },
    { SCE_C_CHARACTER,		RGB_DARKBLUE },
    { SCE_C_UUID,			RGB_CYAN },
    { SCE_C_OPERATOR,		RGB_RED },
    { SCE_C_PREPROCESSOR,	RGB_PURPLE },
    { SCE_C_WORD,			RGB_BLUE },
    { SCE_C_WORD2,			RGB_CYAN },
    { -1,					0 }
};

// CDialogConsole dialog
IMPLEMENT_DYNAMIC( CDialogConsole, CDialogEx )

CDialogConsole::CDialogConsole( CString WindowTitle, CWnd* pParent /*=NULL*/ ) : 
    CDialogEx( IDD_DIALOG_CONSOLE, pParent ), 
    m_strWindowTitle( WindowTitle ),
    m_hEditWindow( NULL ),
    m_bInitialized( FALSE )
{
}

CDialogConsole::~CDialogConsole( )
{
    EndDialog( 0 );
}

void CDialogConsole::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
}

void CDialogConsole::SetStyle( int style, COLORREF fore, COLORREF back, int size, const char* face )
{
    m_Edit.SetForeground( style, fore );
    m_Edit.SetBackground( style, back );

    if (size >= 1)
        m_Edit.SetSize( style, size );

    if (face)
        m_Edit.SetFont( style, face );
}

BEGIN_MESSAGE_MAP( CDialogConsole, CDialogEx )
    ON_WM_SIZE( )
END_MESSAGE_MAP( )

BOOL CDialogConsole::Create( UINT nIDTemplate, CWnd* pParentWnd )
{
    return CDialogEx::Create( nIDTemplate, pParentWnd );
}

void CDialogConsole::PrintText( const TCHAR* message )
{
    CString MessageText;

    // Make temporarily editable
    m_Edit.SetReadOnly( FALSE );

    MessageText += _T( "\r\n> " );
    MessageText += message;

    // Append the text
    m_Edit.AppendText( MessageText );

    // Send cursor to end of document
    m_Edit.GoToLine( m_Edit.GetLineCount( ) - 1 );

    // Make read only
    m_Edit.SetReadOnly( TRUE );
}

BOOL CDialogConsole::OnInitDialog( )
{
    CDialogEx::OnInitDialog( );

    CenterWindow( GetDesktopWindow( ) );
    SetWindowText( m_strWindowTitle );

    // Create the Scintilla editor	
    InitialiseEditor( );

    // Size the editor appropriately
    SizeEditor( );

    // Make temporarily editable
    m_Edit.SetReadOnly( FALSE );
    // Print text
    m_Edit.SetText( _T( "> [~] Console Initialized" ) );
    // Make read only
    m_Edit.SetReadOnly( TRUE );

    // Set the caret at the top
    m_Edit.GoToLine( 0 );

    m_bInitialized = TRUE;

    return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogConsole::InitialiseEditor( )
{
    // Punt if we already have a window
    if (::IsWindow( m_Edit.GetSafeHwnd( ) ))
        return;

    // Create editor window
    m_Edit.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, CRect( 0, 0, 700, 300 ), this, NULL );

    // Did we get the editor window
    if (!::IsWindow( m_Edit.GetSafeHwnd( ) ))
        return;

    m_hEditWindow = m_Edit.GetSafeHwnd( );

    // CPP lexer
    m_Edit.SetLexer( SCLEX_CPP );

    // Set number of style bits to use
    m_Edit.SetStyleBits( 5 );

    // Set tab width
    m_Edit.SetTabWidth( 4 );

    // Use console keywords
    m_Edit.SetKeywords( 0, g_ConsoleKeyWords );

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    SetStyle( STYLE_DEFAULT, RGB_BLACK, RGB_WHITE, 10, "Segoe UI" );

    // Set caret foreground color
    //SendEditor(SCI_SETCARETFORE, RGB(255, 255, 255));

    // Set all styles
    m_Edit.SetAllStylesDefault( );

    // Set selection color
    m_Edit.SetSelectionBackground( TRUE, RGB( 240, 240, 240 ) );

    // Set syntax colors
    for (int i = 0; g_rgbSyntaxConsole[i].iItem != -1; i++)
        m_Edit.SetForeground( g_rgbSyntaxConsole[i].iItem, g_rgbSyntaxConsole[i].rgb );

    m_Edit.SetHorizontalScrollVisible( TRUE );
    //SendEditor(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS);
    m_Edit.SetMarginWidth( 0, 32 );
    m_Edit.SetMarginWidth( 1, 0 );

    // Make read only
    m_Edit.SetReadOnly( TRUE );
}

void CDialogConsole::OnCancel( )
{
    ShowWindow( SW_HIDE );
    CDialogEx::OnCancel( );
}

void CDialogConsole::SizeEditor( )
{
    if (m_hEditWindow)
    {
        RECT rect;
        GetClientRect( &rect );
        m_Edit.MoveWindow( &rect );
    }
}

void CDialogConsole::OnSize( UINT nType, int cx, int cy )
{
    CDialogEx::OnSize( nType, cx, cy );
    SizeEditor( );
}

// CDialogConsole message handlers
