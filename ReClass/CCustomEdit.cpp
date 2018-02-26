#include "stdafx.h"

#include "CCustomEdit.h"
#include "CClassFrame.h"

BEGIN_MESSAGE_MAP( CCustomEdit, CEdit )
    ON_WM_CHAR( )
    ON_CONTROL_REFLECT( EN_CHANGE, &CCustomEdit::OnEnChange )
    ON_WM_CTLCOLOR_REFLECT( )
END_MESSAGE_MAP( )

BOOL CCustomEdit::Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
    m_brBackGnd = CreateSolidBrush( g_clrSelect );
    dwStyle = dwStyle | ES_AUTOHSCROLL; // ICY
    return CEdit::Create( dwStyle, rect, pParentWnd, nID );
}

HBRUSH CCustomEdit::CtlColor( CDC* pDC, UINT nCtlColor )
{
    pDC->SetBkColor( g_clrSelect );
    return m_brBackGnd;
}

void CCustomEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
    CClassView* ClassView;
    CNodeBase* HotspotNode;
    UINT HotspotNodeIndex;
    DWORD Before, After;

    if (nChar == VK_RETURN)
    {
        ShowWindow( SW_HIDE );
        GetWindowText( m_Hotspot.Text );

        ClassView = static_cast<CClassView*>(GetParent( ));
        HotspotNode = m_Hotspot.Object;

        Before = HotspotNode->GetMemorySize( );
        HotspotNode->Update( &m_Hotspot );
        After = HotspotNode->GetMemorySize( );

        HotspotNodeIndex = ClassView->FindNodeIndex( HotspotNode );

        ClassView->ResizeNode( static_cast<CNodeClass*>(HotspotNode->GetParent( )), HotspotNodeIndex, Before, After );
        ClassView->Invalidate( );
    }

    CEdit::OnChar( nChar, nRepCnt, nFlags );
}

void CCustomEdit::OnEnChange( )
{
    CString TextString;
    int StringWidth;

    GetWindowText( TextString );

    StringWidth = (TextString.GetLength( ) + 1) * g_FontWidth; // + 6;
    if (StringWidth > m_MinWidth)
        SetWindowPos( NULL, 0, 0, StringWidth, g_FontHeight, SWP_NOMOVE );
}
