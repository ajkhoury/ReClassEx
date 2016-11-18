#pragma once

#include <afxwin.h>
#include "HotSpot.h"

class CCustomEdit : public CEdit
{
public:
	virtual BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

	DECLARE_MESSAGE_MAP( )

	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnEnChange( );
	afx_msg HBRUSH CtlColor( CDC* /*pDC*/, UINT /*nCtlColor*/ );

public:
	HotSpot spot;
	int MinWidth;
	HBRUSH m_brBackGnd;
};