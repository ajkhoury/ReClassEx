// DialogEdit.cpp : implementation file
//
#include "stdafx.h"
#include "ReClassEx.h"
#include "DialogEdit.h"
#include "afxdialogex.h"



// C++ keywords
static const char g_cppKeyWords[] =
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

    // Specific
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
static SCINTILLA_COLORS s_rgbSyntaxCpp[] = {
    { SCE_C_COMMENT,		green },
    { SCE_C_COMMENTLINE,	green },
    { SCE_C_COMMENTDOC,		green },
    { SCE_C_NUMBER,			darkblue },
    { SCE_C_STRING,			red },
    { SCE_C_CHARACTER,		yellow },
    { SCE_C_UUID,			cyan },
    { SCE_C_OPERATOR,		red },
    { SCE_C_PREPROCESSOR,	blue },
    { SCE_C_WORD,			blue },
    { -1,					0 }
};

IMPLEMENT_DYNAMIC( CDialogEdit, CDialogEx )

CDialogEdit::CDialogEdit( CWnd* pParent /*=NULL*/ ) : 
    CDialogEx( CDialogEdit::IDD, pParent ),
    m_hEditWindow( NULL )
{
}

CDialogEdit::~CDialogEdit( )
{
}

void CDialogEdit::SetStyle( int style, COLORREF fore, COLORREF back, int size, const char* face )
{
    m_Edit.SetForeground( style, fore );
    m_Edit.SetBackground( style, back );
    if (size >= 1)
        m_Edit.SetSize( style, size );
    if (face)
        m_Edit.SetFont( style, face );
}

void CDialogEdit::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CDialogEdit, CDialogEx )
    ON_WM_SIZE( )
    ON_COMMAND( ID_FILE_EDITOROPEN, &CDialogEdit::OnFileEditoropen )
    ON_COMMAND( ID_FILE_EDITORSAVEAS, &CDialogEdit::OnFileEditorsaveas )
END_MESSAGE_MAP( )

BOOL CDialogEdit::Create( LPCTSTR lpszTemplateName, CWnd* pParentWnd )
{
    return CDialogEx::Create( lpszTemplateName, pParentWnd );
}

void CDialogEdit::InitialiseEditor( )
{
    // Punt if we already have a window
    if (::IsWindow( m_Edit.GetSafeHwnd( ) ))
        return;

    // Create editor window
    m_Edit.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, CRect( 0, 0, 500, 400 ), this, NULL );

    // Did we get the editor window
    if (!::IsWindow( m_Edit.GetSafeHwnd( ) ))
    {
        PrintOut( _T( "Unable to create editor window" ) );
        return;
    }

    m_hEditWindow = m_Edit.GetSafeHwnd( );

    // CPP lexer
    m_Edit.SetLexer( SCLEX_CPP );

    // Set number of style bits to use
    m_Edit.SetStyleBits( 5 );

    // Set tab width
    m_Edit.SetTabWidth( 4 );

    // Use CPP keywords
    m_Edit.SetKeywords( 0, g_cppKeyWords );

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    SetStyle( STYLE_DEFAULT, black, white, 10, "Courier New" );

    // Set caret foreground color
    //SendEditor(SCI_SETCARETFORE, RGB(255, 255, 255));

    // Set all styles
    m_Edit.SetAllStylesDefault( );

    // Set selection color
    m_Edit.SetSelectionBackground( TRUE, RGB( 240, 240, 240 ) );

    // Set syntax colors
    for (int i = 0; s_rgbSyntaxCpp[i].iItem != -1; i++)
    {
        m_Edit.SetForeground( s_rgbSyntaxCpp[i].iItem, s_rgbSyntaxCpp[i].rgb );
    }

    m_Edit.SetHorizontalScrollVisible( FALSE );
    //SendEditor(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS);
    m_Edit.SetMarginWidth( 0, 32 );
    m_Edit.SetMarginWidth( 1, 0 );


}

BOOL CDialogEdit::OnInitDialog( )
{
    CDialogEx::OnInitDialog( );

    ShowWindow( SW_HIDE );

    CWnd* pWnd = GetDesktopWindow( );
    CenterWindow( pWnd );

    SetWindowText( Title );
    // Create the Scintilla editor	
    InitialiseEditor( );
    m_Edit.SetText( Text );

    ShowWindow( SW_NORMAL );
    SizeEditor( );

    m_Edit.SetSelection( 0, 0 );

    return FALSE;  // return TRUE unless you set the focus to a control
}


void CDialogEdit::OnCancel( )
{
    // Get text
    Text = m_Edit.GetText( );
    CDialogEx::OnCancel( );
}

void CDialogEdit::SizeEditor( )
{
    if (m_hEditWindow != NULL)
    {
        CWnd* pWnd = NULL;
        RECT rect;
        GetClientRect( &rect );
        m_Edit.MoveWindow( &rect );
    }
}

void CDialogEdit::OnSize( UINT nType, int cx, int cy )
{
    CDialogEx::OnSize( nType, cx, cy );
    SizeEditor( );
}

void CDialogEdit::OnFileEditoropen( )
{
    TCHAR szFileName[MAX_PATH + 1024] = _T( "" );

    // Open file structure
    OPENFILENAME ofn;
    ZeroMemory( (LPVOID)&ofn, sizeof( OPENFILENAME ) );
    // Fill in open file structure
    ofn.lStructSize = sizeof( OPENFILENAME );
    ofn.lpstrFilter = _T( "All Files (*.*)\x0*.*\x0\x0" );
    ofn.lpstrTitle = _T( "Open Source File" );
    ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof( szFileName );
    ofn.lpstrDefExt = _T( "" );
    ofn.hwndOwner = GetSafeHwnd( );

    // Get user file
    if (::GetOpenFileName( &ofn ))
    {
        // Clear editor text
        m_Edit.Clear( );

        // Ensure it exists
        if (::GetFileAttributes( szFileName ) == 0xFFFFFFFF)
        {
            // Open the file
            CFile file;
            if (file.Open( szFileName, CFile::modeRead | CFile::shareDenyNone ))
            {
                // Ensure length
                DWORD uSize = (DWORD)(file.GetLength( ) & 0xFFFFFFFF);
                if (uSize > 0)
                {
                    // Allocate memory
                    char* Buffer = new char[uSize + 1];
                    if (Buffer)
                    {
                        // Read the data
                        if (file.Read( (void*)Buffer, (UINT)uSize ))
                        {
                            // null terminate (probably not needed here)
                            Buffer[uSize] = '\0';
                            // Set editor text
                            m_Edit.SetText( Buffer );
                        }
                        // Close the file
                        file.Close( );
                        // Release memory
                        delete[] Buffer;
                    }
                }
            }
        }
    }
}


void CDialogEdit::OnFileEditorsaveas( )
{
    TCHAR szFileName[MAX_PATH] = _T( "" );

    // Open file structure
    OPENFILENAME ofn;
    ZeroMemory( (LPVOID)&ofn, sizeof( OPENFILENAME ) );
    // Fill in open file structure
    ofn.lStructSize = sizeof( OPENFILENAME );
    ofn.lpstrFilter = _T( "All Files (*.*)\x0*.*\x0\x0" );
    ofn.lpstrTitle = _T( "Open Source File" );
    ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof( szFileName );
    ofn.lpstrDefExt = _T( "" );
    ofn.hwndOwner = GetSafeHwnd( );

    // Get user file
    if (::GetSaveFileName( &ofn ))
    {
        // Open the file
        CFile file;
        if (file.Open( szFileName, CFile::modeCreate | CFile::shareExclusive | CFile::modeWrite ))
        {
            // Get text length
            DWORD dwSize = m_Edit.GetLength( );
            if (dwSize > 0)
            {
                // Get editor text
                CString Text = m_Edit.GetText( );
                // Write out the file data
                file.Write( (const VOID*)Text.GetBuffer( ), dwSize );
                // Close the file
                file.Close( );	
            }
        }
    }
}
