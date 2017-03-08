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
	m_brBackGnd = CreateSolidBrush( g_crSelect );
	dwStyle = dwStyle | ES_AUTOHSCROLL; // ICY
	return CEdit::Create( dwStyle, rect, pParentWnd, nID );
}

HBRUSH CCustomEdit::CtlColor( CDC* pDC, UINT nCtlColor )
{
	pDC->SetBkColor( g_crSelect );
	return m_brBackGnd;
}

void CCustomEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (nChar == VK_RETURN)
	{
		ShowWindow( SW_HIDE );
		GetWindowText( spot.Text );

		CClassView* pChild = (CClassView*)GetParent( );
		CNodeBase* c = (CNodeBase*)spot.object;

		DWORD before = c->GetMemorySize( );
		c->Update( spot );
		DWORD after = c->GetMemorySize( );

		pChild->ResizeNode( (CNodeClass*)c->GetParent( ), pChild->FindNodeIndex( c ), before, after );
		pChild->Invalidate( );
	}
	CEdit::OnChar( nChar, nRepCnt, nFlags );
}

void CCustomEdit::OnEnChange( )
{
	CString text;
	GetWindowText( text );
	int  w = (text.GetLength( ) + 1) * g_FontWidth; // + 6;
	if (w > MinWidth)
		SetWindowPos( NULL, 0, 0, w, g_FontHeight, SWP_NOMOVE );
}
