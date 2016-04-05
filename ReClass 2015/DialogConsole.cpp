// DialogConsole.cpp : implementation file
//

#include "stdafx.h"
#include "DialogConsole.h"
#include "ReClass2015.h"
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
static SScintillaColors g_rgbSyntaxConsole[] =
{
	{ SCE_C_COMMENT,		green },
	{ SCE_C_COMMENTLINE,	green },
	{ SCE_C_COMMENTDOC,		green },
	{ SCE_C_NUMBER,			darkblue },
	{ SCE_C_STRING,			orange },
	{ SCE_C_CHARACTER,		yellow },
	{ SCE_C_UUID,			cyan },
	{ SCE_C_OPERATOR,		lightred },
	{ SCE_C_PREPROCESSOR,	blue },
	{ SCE_C_WORD,			blue },
	{ -1,					0 }
};

// CDialogConsole dialog

IMPLEMENT_DYNAMIC(CDialogConsole, CDialogEx)

CDialogConsole::CDialogConsole(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONSOLE, pParent)
{
	m_hwndEditWindow = NULL;
	m_bInited = FALSE;
}

CDialogConsole::~CDialogConsole()
{
}

void CDialogConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogConsole, CDialogEx)
	ON_WM_SIZE()
	ON_MESSAGE(WM_PRINT_TEXT_MESSAGE, &CDialogConsole::PrintText)
END_MESSAGE_MAP()

BOOL CDialogConsole::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	return CDialogEx::Create(nIDTemplate, pParentWnd);
}

LRESULT CDialogConsole::PrintText(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL) return FALSE;

	// Make temporarily editable
	SendEditor(SCI_SETREADONLY, FALSE);

	LPTSTR pString = (LPTSTR)wParam;

	m_strConsoleText += _T("> ");
	m_strConsoleText += pString;
	m_strConsoleText += "\r\n";

	int length = m_strConsoleText.GetLength();
	char* pText = (char*)malloc(length + 1);

	#ifdef UNICODE
	size_t converted = 0;
	wcstombs_s(&converted, pText, length + 1, m_strConsoleText.GetBuffer(), length + 1);
	#else
	strcpy_s(pText, length + 1, m_strConsoleText.GetBuffer());
	#endif

	SendEditor(SCI_SETTEXT, 0, (LPARAM)pText);

	delete[] pText;

	// Make read only
	SendEditor(SCI_SETREADONLY, TRUE);

	return TRUE;
}

BOOL CDialogConsole::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CWnd* pWnd = GetDesktopWindow();
	CenterWindow(pWnd);

	SetWindowText(m_strWindowTitle);
	// Create the Scintilla editor	
	InitialiseEditor();
	
	SizeEditor();
	SendEditor(SCI_SETSEL, 0, 0);
	//delete[] pText; // free memory

	m_bInited = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogConsole::InitialiseEditor()
{
	// Punt if we already have a window
	if (::IsWindow(m_hwndEditWindow))
		return;

	// Create editor window
	m_hwndEditWindow = CreateWindowEx(0, _T("Scintilla"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
		0, 0, 700, 300, GetSafeHwnd(), NULL /*(HMENU)GuiID*/, AfxGetApp()->m_hInstance, NULL);

	// Did we get the editor window
	if (!::IsWindow(m_hwndEditWindow))
	{
		//PrintOut(_T("Unable to create editor window"));
		return;
	}

	// CPP lexer
	SendEditor(SCI_SETLEXER, SCLEX_CPP);

	// Set number of style bits to use
	SendEditor(SCI_SETSTYLEBITS, 5);

	// Set tab width
	SendEditor(SCI_SETTABWIDTH, 4);

	// Use CPP keywords
	SendEditor(SCI_SETKEYWORDS, 0, (LPARAM)g_ConsoleKeyWords);

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetStyle(STYLE_DEFAULT, black, white, 10, _T("Courier New"));

	// Set caret foreground color
	//SendEditor(SCI_SETCARETFORE, RGB(255, 255, 255));

	// Set all styles
	SendEditor(SCI_STYLECLEARALL);

	// Set selection color
	SendEditor(SCI_SETSELBACK, TRUE, RGB(240, 240, 240));

	// Set syntax colors
	for (long i = 0; g_rgbSyntaxConsole[i].iItem != -1; i++)
		SendEditor(SCI_STYLESETFORE, g_rgbSyntaxConsole[i].iItem, g_rgbSyntaxConsole[i].rgb);

	SendEditor(SCI_SETHSCROLLBAR, false);
	//SendEditor(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS);
	SendEditor(SCI_SETMARGINWIDTHN, 0, 32);
	SendEditor(SCI_SETMARGINWIDTHN, 1, 0);

	// Make read only
	SendEditor(SCI_SETREADONLY, TRUE);
}

void CDialogConsole::OnCancel()
{
	ShowWindow(SW_HIDE);
	CDialogEx::OnCancel();
}

void CDialogConsole::SizeEditor()
{
	if (m_hwndEditWindow)
	{
		RECT rect;
		GetClientRect(&rect);
		CWnd *pWnd = CWnd::FromHandle(m_hwndEditWindow);
		if (pWnd) pWnd->MoveWindow(&rect);
	}
}

void CDialogConsole::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	SizeEditor();
}


// CDialogConsole message handlers
