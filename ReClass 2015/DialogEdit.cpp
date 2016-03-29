// DialogEdit.cpp : implementation file
//
#include "stdafx.h"
#include "ReClass2015.h"
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



/// Scintilla Colors structure
struct SScintillaColors
{	
	int			iItem;
	COLORREF	rgb;
};

// A few basic colors
const COLORREF black = RGB( 0, 0, 0 );
const COLORREF white = RGB( 255, 255, 255 );
const COLORREF green = RGB( 0, 200, 0 );
const COLORREF red = RGB( 255, 0, 0 );
const COLORREF blue = RGB( 0, 0, 255 );
const COLORREF darkblue = RGB( 0, 0, 100 );
const COLORREF yellow = RGB( 255, 255, 0 );
const COLORREF magenta = RGB( 255, 0, 255 );
const COLORREF cyan = RGB( 0, 255, 255 );
const COLORREF purple = RGB( 128, 0, 255 );

/// Default color scheme
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

IMPLEMENT_DYNAMIC(CDialogEdit, CDialogEx)

CDialogEdit::CDialogEdit(CWnd* pParent /*=NULL*/) : CDialogEx(CDialogEdit::IDD, pParent)
{
	m_hwndEditor = NULL;
}

CDialogEdit::~CDialogEdit()
{
}

void CDialogEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogEdit, CDialogEx)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_EDITOROPEN, &CDialogEdit::OnFileEditoropen)
	ON_COMMAND(ID_FILE_EDITORSAVEAS, &CDialogEdit::OnFileEditorsaveas)
END_MESSAGE_MAP()

BOOL CDialogEdit::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}

void CDialogEdit::InitialiseEditor() 
{
	// Punt if we already have a window
	if (::IsWindow(m_hwndEditor))
		return;

	// Create editor window
	m_hwndEditor = CreateWindowEx(0, _T("Scintilla"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
		0, 0, 500, 400, GetSafeHwnd(), NULL /*(HMENU)GuiID*/, AfxGetApp()->m_hInstance, NULL);	

	// Did we get the editor window
	if (!::IsWindow(m_hwndEditor))
	{	
		_tprintf(_T("Unable to create editor window\n"));
		return;
	}

	// CPP lexer
	SendEditor(SCI_SETLEXER, SCLEX_CPP);

	// Set number of style bits to use
	SendEditor(SCI_SETSTYLEBITS, 5);

	// Set tab width
	SendEditor(SCI_SETTABWIDTH, 4);

	// Use CPP keywords
	SendEditor(SCI_SETKEYWORDS, 0, (LPARAM)g_cppKeyWords);

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle(STYLE_DEFAULT, black, white, 10, _T("Courier New"));

	// Set caret foreground color
	//SendEditor(SCI_SETCARETFORE, RGB(255, 255, 255));

	// Set all styles
	SendEditor(SCI_STYLECLEARALL);

	// Set selection color
	SendEditor(SCI_SETSELBACK, TRUE, RGB(240, 240, 240));

	// Set syntax colors
	for ( long i = 0; g_rgbSyntaxCpp[i].iItem != -1; i++ )
	{
		SendEditor(SCI_STYLESETFORE, g_rgbSyntaxCpp[i].iItem, g_rgbSyntaxCpp[i].rgb);
	}

	SendEditor(SCI_SETHSCROLLBAR, false);
	//SendEditor(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS);
	SendEditor(SCI_SETMARGINWIDTHN, 0, 32);
	SendEditor(SCI_SETMARGINWIDTHN, 1, 0);
}

BOOL CDialogEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CWnd* pWnd = GetDesktopWindow();
	CenterWindow(pWnd);

	SetWindowText(Title);
	// Create the Scintilla editor	
	InitialiseEditor();
	int length = Text.GetLength();
	char* pText = (char*)malloc(length + 1);

#ifdef UNICODE
	size_t converted = 0;
	wcstombs_s(&converted, pText, length + 1, Text.GetBuffer(), length + 1);
#else
	strcpy_s(pText, length, Text.GetBuffer());
#endif

	SendEditor(SCI_SETTEXT, 0, (WPARAM)pText);
	ShowWindow(SW_NORMAL);
	SizeEditor();
	SendEditor(SCI_SETSEL, 0, 0);

	delete[] pText; // free memory

	return FALSE;  // return TRUE unless you set the focus to a control
}


void CDialogEdit::OnCancel()
{
	// Get text length
	UINT uSize = (UINT)SendEditor(SCI_GETLENGTH, 0, 0L);
	if (uSize)
	{
		char *pBuf = new char[uSize + 1 + 8];
		if (pBuf)
		{
			SendEditor(SCI_GETTEXT, uSize + 1, (LPARAM)pBuf);
			pBuf[uSize] = '\0';
			Text = pBuf;
			delete pBuf;
		}
	}
	CDialogEx::OnCancel();
}

void CDialogEdit::SizeEditor()
{
	if (m_hwndEditor)
	{
		RECT rect;
		GetClientRect( &rect );
		CWnd *pWnd = CWnd::FromHandle( m_hwndEditor );
		if ( pWnd ) pWnd->MoveWindow( &rect );
	}
}

void CDialogEdit::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	SizeEditor();
}


void CDialogEdit::OnFileEditoropen()
{
	TCHAR fname[MAX_PATH + 1024] = _T("");

	// Open file structure
	OPENFILENAME ofn;
	ZeroMemory((LPVOID)&ofn, sizeof(OPENFILENAME));
	// Fill in open file structure
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.lpstrFilter	= _T("All Files (*.*)\x0*.*\x0\x0");
	ofn.lpstrTitle	= _T("Open Source File");
	ofn.Flags		= OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrFile	= fname;
	ofn.nMaxFile	= sizeof(fname);
	ofn.lpstrDefExt	= _T("");
	ofn.hwndOwner	= GetSafeHwnd();

	// Get user file
	if (!::GetOpenFileName(&ofn))
		return;

	// Clear editor text
	SendEditor(SCI_SETTEXT, 0, (WPARAM)"");

	// Ensure it exists
	if (0xFFFFFFFF == ::GetFileAttributes(fname))
	{
		ShowError(_T("File does not exist"));
		return;
	}

	try
	{
		// Open the file
		CFile f;
		if (!f.Open(fname, CFile::modeRead | CFile::shareDenyNone))
		{
			ShowError(_T("Error opening file"));
			return;
		}

		// Ensure length
		UINT uSize = (UINT)(f.GetLength() & 0xFFFFFFFF);
		if (!uSize) 
		{
			ShowError(_T("File of zero length"));
			return;
		}

		// Allocate memory
		unsigned char* pBuf = new unsigned char[uSize + 1];
		if (!pBuf) 
		{
			ShowError(_T("Memory allocation error"));
			return;
		}

		// Read the data
		if (f.Read((void*)pBuf, (UINT)uSize))
		{	
			// NULL terminate
			pBuf[uSize] = '\0';
			// Set editor text
			SendEditor(SCI_SETTEXT, 0, (WPARAM)pBuf);
		}

		// Close the file
		f.Close();

		// Release memory
		delete[] pBuf;
	}
	catch(...) 
	{	
		ShowError(_T("Assertion while reading file"));
	}
}


void CDialogEdit::OnFileEditorsaveas()
{
	TCHAR fname[MAX_PATH + 1024] = _T("");

	// Open file structure
	OPENFILENAME ofn;
	ZeroMemory((LPVOID)&ofn, sizeof(OPENFILENAME));

	// Fill in open file structure
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.lpstrFilter	= _T("All Files (*.*)\x0*.*\x0\x0");
	ofn.lpstrTitle	= _T("Open Source File");
	ofn.Flags		= OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrFile	= fname;
	ofn.nMaxFile	= sizeof( fname );
	ofn.lpstrDefExt	= _T("");
	ofn.hwndOwner	= GetSafeHwnd();

	// Get user file
	if (!::GetSaveFileName(&ofn))
		return;

	try
	{
		// Open the file
		CFile f;
		if (!f.Open(fname, CFile::modeCreate | CFile::shareExclusive | CFile::modeWrite ))
		{
			ShowError(_T("Error creating file"));
			return;
		}

		// Get text length
		UINT uSize = (UINT)SendEditor(SCI_GETLENGTH, 0, 0L);
		if (!uSize) 
			return;

		// Allocate memory
		char *pBuf = new char[uSize + 1 + 8];
		if (!pBuf) 
		{	
			ShowError(_T("Memory allocation error"));
			return;
		}

		// Get editor text
		SendEditor(SCI_GETTEXT, uSize + 1, (LPARAM)pBuf);

		// NULL terminate ( I know this isn't needed here )
		pBuf[uSize] = 0;

		// Write out the file data
		f.Write(pBuf, uSize);

		// Close the file
		f.Close();
	}
	catch( ... ) 
	{	
		ShowError(_T("Assertion while reading file"));
	}
}

void CDialogEdit::ShowError(LPCTSTR pError)
{
	MessageBox(pError, _T("Error"), MB_OK | MB_ICONEXCLAMATION);
}
