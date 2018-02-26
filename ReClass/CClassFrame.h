#pragma once

#include "CClassView.h"

class CClassFrame : public CMDIChildWndEx {
    DECLARE_DYNCREATE( CClassFrame )
public:
    CClassFrame( );

    // Attributes
public:

    // Operations
public:

    // Overrides
public:
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
    virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

    // Implementation
public:
    inline void SetClass( CNodeClass* pClass ) { m_pClassView->SetClass( pClass ); }
    inline CClassView* GetChildView( void ) { return m_pClassView; }

    // View for the client area of the frame.
    CClassView* m_pClassView;

    virtual ~CClassFrame( );

#ifdef _DEBUG
    virtual void AssertValid( ) const;
    virtual void Dump( CDumpContext& dc ) const;
#endif

    // Generated message map functions
protected:
    afx_msg void OnFileClose( );
    afx_msg void OnSetFocus( CWnd* pOldWnd );
    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    DECLARE_MESSAGE_MAP( )
};
