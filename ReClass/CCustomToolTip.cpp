#include "stdafx.h"

#include "CCustomToolTip.h"

BEGIN_MESSAGE_MAP( CCustomToolTip, CEdit )
    ON_WM_CTLCOLOR_REFLECT( )
END_MESSAGE_MAP( )

BOOL CCustomToolTip::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
    m_brBackGnd = CreateSolidBrush( 0xFFFFFF );
    return CEdit::Create( dwStyle, rect, pParentWnd, nID );
}

HBRUSH CCustomToolTip::CtlColor( CDC* pDC, UINT nCtlColor )
{
    pDC->SetBkColor( 0xFFFFFF );
    return m_brBackGnd;
}