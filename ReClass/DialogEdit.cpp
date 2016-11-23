// DialogEdit.cpp : implementation file
//
#include "stdafx.h"
#include "ReClass2016.h"
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
static SScintillaColors g_rgbSyntaxCpp[] =
{
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

CDialogEdit::CDialogEdit( CWnd* pParent /*=NULL*/ ) : CDialogEx( CDialogEdit::IDD, pParent )
{
	m_hwndEditor = NULL;
}

CDialogEdit::~CDialogEdit( )
{
}

LRESULT CDialogEdit::SendEditor( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	return ::SendMessage( m_hwndEditor, Msg, wParam, lParam );
}

void CDialogEdit::SetAStyle( int style, COLORREF fore, COLORREF back, int size, const TCHAR * face )
{
	SendEditor( SCI_STYLESETFORE, style, fore );
	//SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendEditor( SCI_STYLESETSIZE, style, size );
	if (face)
		SendEditor( SCI_STYLESETFONT, style, (LPARAM)face );
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
	if (::IsWindow( m_hwndEditor ))
		return;

	// Create editor window
	m_hwndEditor = CreateWindowEx( 0, _T( "Scintilla" ), _T( "" ), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
								   0, 0, 500, 400, GetSafeHwnd( ), NULL /*(HMENU)GuiID*/, AfxGetApp( )->m_hInstance, NULL );
	// Did we get the editor window
	if (!::IsWindow( m_hwndEditor ))
	{
		PrintOut( _T( "Unable to create editor window" ) );
		return;
	}

	// CPP lexer
	SendEditor( SCI_SETLEXER, SCLEX_CPP );

	// Set number of style bits to use
	SendEditor( SCI_SETSTYLEBITS, 5 );

	// Set tab width
	SendEditor( SCI_SETTABWIDTH, 4 );

	// Use CPP keywords
	SendEditor( SCI_SETKEYWORDS, 0, (LPARAM)g_cppKeyWords );

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle( STYLE_DEFAULT, black, white, 10, _T( "Courier New" ) );

	// Set caret foreground color
	//SendEditor(SCI_SETCARETFORE, RGB(255, 255, 255));

	// Set all styles
	SendEditor( SCI_STYLECLEARALL );

	// Set selection color
	SendEditor( SCI_SETSELBACK, TRUE, RGB( 240, 240, 240 ) );

	// Set syntax colors
	for (int i = 0; g_rgbSyntaxCpp[i].iItem != -1; i++)
	{
		SendEditor( SCI_STYLESETFORE, g_rgbSyntaxCpp[i].iItem, g_rgbSyntaxCpp[i].rgb );
	}

	SendEditor( SCI_SETHSCROLLBAR, false );
	//SendEditor(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS);
	SendEditor( SCI_SETMARGINWIDTHN, 0, 32 );
	SendEditor( SCI_SETMARGINWIDTHN, 1, 0 );
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
	int length = Text.GetLength( );
	char* TextBuffer = new char[length + 1];
	if (TextBuffer)
	{
		#ifdef UNICODE
		size_t converted = 0;
		wcstombs_s( &converted, TextBuffer, length + 1, Text.GetBuffer( ), length + 1 );
		#else
		strcpy_s( pText, length, Text.GetBuffer( ) );
		#endif

		SendEditor( SCI_SETTEXT, 0, (WPARAM)TextBuffer );
		ShowWindow( SW_NORMAL );
		SizeEditor( );
		SendEditor( SCI_SETSEL, 0, 0 );

		delete[] TextBuffer; // free memory
	}

	return FALSE;  // return TRUE unless you set the focus to a control
}


void CDialogEdit::OnCancel( )
{
	// Get text length
	DWORD uSize = (DWORD)SendEditor( SCI_GETLENGTH, 0, 0L );
	if (uSize > 0)
	{
		char* Buffer = new char[uSize + 1 + 8];
		if (Buffer)
		{
			SendEditor( SCI_GETTEXT, uSize + 1, (LPARAM)Buffer );
			Buffer[uSize] = '\0';
			Text = Buffer;
			delete[] Buffer;
		}
	}
	CDialogEx::OnCancel( );
}

void CDialogEdit::SizeEditor( )
{
	if (m_hwndEditor)
	{
		CWnd* pWnd = NULL;
		RECT rect = { 0 };

		GetClientRect( &rect );
		pWnd = CWnd::FromHandle( m_hwndEditor );
		if (pWnd)
		{
			pWnd->MoveWindow( &rect );
		}
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
		SendEditor( SCI_SETTEXT, 0, (WPARAM)"" );
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
							// NULL terminate
							Buffer[uSize] = '\0';
							// Set editor text
							SendEditor( SCI_SETTEXT, 0, (WPARAM)Buffer );
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
			DWORD dwSize = (DWORD)SendEditor( SCI_GETLENGTH, 0, 0L );
			if (dwSize > 0)
			{
				// Allocate memory
				char* Buffer = new char[dwSize + 1 + 8];
				if (Buffer)
				{
					// Get editor text
					SendEditor( SCI_GETTEXT, dwSize + 1, (LPARAM)Buffer );
					// NULL terminate (Probably not needed here)
					Buffer[dwSize] = '\0';
					// Write out the file data
					file.Write( Buffer, dwSize );
					// Close the file
					file.Close( );
				}
			}
		}
	}
}
