#pragma once

#include <afxwin.h>

class CProgressBar : public CProgressCtrl
{
public:
    CProgressBar( );
    CProgressBar( LPCTSTR strMessage, int nSize = 100, int MaxValue = 100,
        BOOL bSmooth = FALSE, int nPane = 0, CStatusBar* pBar = NULL );
    ~CProgressBar( );
    BOOL Create( LPCTSTR strMessage, int nSize = 100, int MaxValue = 100,
        BOOL bSmooth = FALSE, int nPane = 0 );

    DECLARE_DYNCREATE( CProgressBar )

    // operations
public:
    BOOL SetRange( int nLower, int nUpper, int nStep = 1 );
    BOOL SetText( LPCTSTR strMessage );
    BOOL SetSize( int nSize );
    COLORREF SetBarColour( COLORREF clrBar );
    COLORREF SetBkColour( COLORREF clrBk );
    int  SetPos( int nPos );
    int  OffsetPos( int nPos );
    int  SetStep( int nStep );
    int  StepIt( );
    void Clear( );

    // Generated message map functions
protected:
    afx_msg BOOL OnEraseBkgnd( CDC* pDC );

    DECLARE_MESSAGE_MAP( )

    // implementation
protected:
    CStatusBar *GetStatusBar( );
    BOOL Resize( );

    int		m_nSize;		// Percentage size of control
    int		m_nPane;		// ID of status bar pane progress bar is to appear in
    CString	m_strMessage;	// Message to display to left of control
    CString m_strPrevText;  // Previous text in status bar
    CRect	m_Rect;			// Dimensions of the whole thing
    CStatusBar* m_pStatusBar; // Set in the ctor to explicitly state which status bar to use.
};