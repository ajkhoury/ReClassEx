#pragma once

#include "afxwin.h"
#include "Resource.h"
#include "CScintillaEdit.h"

// CDialogConsole dialog

class CDialogConsole : public CDialogEx
{
	DECLARE_DYNAMIC( CDialogConsole )

public:
	CDialogConsole( CString WindowTitle, CWnd* pParent = NULL );   // standard constructor
	virtual ~CDialogConsole( );

// Dialog Data
	enum { IDD = IDD_DIALOG_CONSOLE };

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

	DECLARE_MESSAGE_MAP( )


	// Sets a Scintilla style
	void SetStyle( int style, COLORREF fore, COLORREF back = RGB( 255, 255, 255 ), int size = -1, const char* face = 0 );

public:
	afx_msg BOOL Create( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	afx_msg BOOL OnInitDialog( );
	afx_msg void OnCancel( );
	afx_msg void OnSize( UINT nType, int cx, int cy );

	void PrintText( const TCHAR* message );

	BOOL IsInitialized( ) { return m_bInitialized; }

	CString m_strWindowTitle;
	BOOL m_bVisible;

private:
	void InitialiseEditor( );
	void SizeEditor( );

	BOOL m_bInitialized;
	//CString m_strConsoleText;

	HWND m_hEditWindow;
	CScintillaEdit m_Edit;
};
