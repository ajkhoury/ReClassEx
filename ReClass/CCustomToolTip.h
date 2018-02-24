#pragma once

#include <afxwin.h>

class CCustomToolTip : public CEdit {
public:
    virtual BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

    DECLARE_MESSAGE_MAP( )
    afx_msg HBRUSH CtlColor( CDC* pDC, UINT nCtlColor );

public:
    HBRUSH m_brBackGnd;
};