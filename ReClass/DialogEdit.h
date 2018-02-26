#pragma once

#include "afxwin.h"
#include "CScintillaEdit.h"

class CDialogEdit : public CDialogEx {
    DECLARE_DYNAMIC( CDialogEdit )
public:
    CDialogEdit( CWnd* pParent = NULL );
    virtual ~CDialogEdit( );

    CString Title;
    CString Text;

    // Sets a Scintilla style
    void SetStyle( int style, COLORREF fore, COLORREF back = RGB( 255, 255, 255 ), int size = -1, const char* face = 0 );

    void InitializeEditor( );
    void SizeEditor( );

    enum { IDD = IDD_DIALOG_EDIT };

protected:
    HWND m_hEditWindow;
    CScintillaEdit m_Edit;

protected:
    virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

public:
    virtual BOOL Create( LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL );
    virtual BOOL OnInitDialog( );
    virtual void OnCancel( );

    DECLARE_MESSAGE_MAP( )
    afx_msg void OnSize( UINT nType, int cx, int cy );
    afx_msg void OnFileEditoropen( );
    afx_msg void OnFileEditorsaveas( );
};
