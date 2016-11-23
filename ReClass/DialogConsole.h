#pragma once

#include "afxwin.h"
#include "Resource.h"

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

	// Sends a message to the Scintilla editor
	LRESULT SendEditor( UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0 );
	// Sets a Scintilla style
	void SetStyle( int style, COLORREF fore, COLORREF back = RGB( 255, 255, 255 ), int size = -1, const TCHAR* face = 0 );

public:
	afx_msg BOOL Create( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	afx_msg BOOL OnInitDialog( );
	afx_msg void OnCancel( );
	afx_msg void OnSize( UINT nType, int cx, int cy );

	void PrintText( const TCHAR * message );

	BOOL IsInitialized( ) { return m_bInited; }

	CString m_strWindowTitle;
	BOOL m_bVisible;

private:
	void InitialiseEditor( );
	void SizeEditor( );

	HWND m_hwndEditWindow;
	CString m_strConsoleText;
	BOOL m_bInited;
};
