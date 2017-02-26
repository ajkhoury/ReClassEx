#pragma once

#define MAX_NODES 32768

#include "CMemory.h"

#include "CCustomEdit.h"
#include "CCustomToolTip.h"

// CChildView window
class CChildView : public CWnd
{
	// Construction
public:
	CChildView( );
	virtual ~CChildView( );

	// Overrides
protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	// Implementation
public:
	UINT		FindNodeIndex( CNodeBase* pNode );
	CNodeBase*	FindNodeFromIndex( CNodeBase* currentlySelectedNode, UINT index );

	void		ReplaceNode( CNodeClass* pClass, UINT idx, CNodeBase* pNewNode );
	void		RemoveNodes( CNodeClass* pClass, UINT idx, DWORD Length );
	void		FillNodes( CNodeClass* pClass, UINT idx, DWORD Length );
	void		ResizeNode( CNodeClass* pClass, UINT idx, DWORD before, DWORD after );
	void		AddBytes( CNodeClass* pClass, DWORD Length );
	void		InsertBytes( CNodeClass* pClass, UINT idx, DWORD Length );

	void		ReplaceSelectedWithType( NodeType Type );
	void		ClearSelection( );

	void		StandardTypeUpdate( CCmdUI *pCmdUI );

	// Generated message map functions
public:
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP( )

	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnPaint( );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnMouseHover( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnTimer( UINT_PTR nIDEvent );

	afx_msg void OnAddAdd4( );
	afx_msg void OnUpdateAddAdd4( CCmdUI *pCmdUI );

	afx_msg void OnAddAdd8( );
	afx_msg void OnUpdateAddAdd8( CCmdUI *pCmdUI );

	afx_msg void OnAddAdd64( );
	afx_msg void OnUpdateAddAdd64( CCmdUI *pCmdUI );

	afx_msg void OnAddAdd1024( );
	afx_msg void OnUpdateAddAdd1024( CCmdUI *pCmdUI );

	afx_msg void OnAddAdd2048( );
	afx_msg void OnUpdateAddAdd2048( CCmdUI *pCmdUI );

	afx_msg void OnInsertInsert4( );
	afx_msg void OnUpdateInsertInsert4( CCmdUI *pCmdUI );

	afx_msg void OnInsertInsert8( );
	afx_msg void OnUpdateInsertInsert8( CCmdUI *pCmdUI );

	afx_msg void OnInsertInsert64( );
	afx_msg void OnUpdateInsertInsert64( CCmdUI *pCmdUI );

	afx_msg void OnInsertInsert1024( );
	afx_msg void OnUpdateInsertInsert1024( CCmdUI *pCmdUI );

	afx_msg void OnInsertInsert2048( );
	afx_msg void OnUpdateInsertInsert2048( CCmdUI *pCmdUI );

	afx_msg void OnModifyDelete( );
	afx_msg void OnUpdateModifyDelete( CCmdUI *pCmdUI );

	afx_msg void OnModifyShow( );
	afx_msg void OnUpdateModifyShow( CCmdUI *pCmdUI );

	afx_msg void OnModifyHide( );
	afx_msg void OnUpdateModifyHide( CCmdUI *pCmdUI );

	afx_msg void OnTypeHex64( );
	afx_msg void OnUpdateTypeHex64( CCmdUI *pCmdUI );

	afx_msg void OnTypeHex32( );
	afx_msg void OnUpdateTypeHex32( CCmdUI *pCmdUI );

	afx_msg void OnTypeHex16( );
	afx_msg void OnUpdateTypeHex16( CCmdUI *pCmdUI );

	afx_msg void OnTypeHex8( );
	afx_msg void OnUpdateTypeHex8( CCmdUI *pCmdUI );

	afx_msg void OnTypeBits( );
	afx_msg void OnUpdateTypeBits( CCmdUI *pCmdUI );

	afx_msg void OnTypeInt64( );
	afx_msg void OnUpdateTypeInt64( CCmdUI *pCmdUI );

	afx_msg void OnTypeInt32( );
	afx_msg void OnUpdateTypeInt32( CCmdUI *pCmdUI );

	afx_msg void OnTypeInt16( );
	afx_msg void OnUpdateTypeInt16( CCmdUI *pCmdUI );

	afx_msg void OnTypeInt8( );
	afx_msg void OnUpdateTypeInt8( CCmdUI *pCmdUI );

	afx_msg void OnTypeQword( );
	afx_msg void OnUpdateTypeQword( CCmdUI* pCmdUI );

	afx_msg void OnTypeDword( );
	afx_msg void OnUpdateTypeDword( CCmdUI *pCmdUI );

	afx_msg void OnTypeWord( );
	afx_msg void OnUpdateTypeWord( CCmdUI *pCmdUI );

	afx_msg void OnTypeByte( );
	afx_msg void OnUpdateTypeByte( CCmdUI *pCmdUI );

	afx_msg void OnTypeVec2( );
	afx_msg void OnUpdateTypeVec2( CCmdUI *pCmdUI );

	afx_msg void OnTypeVec3( );
	afx_msg void OnUpdateTypeVec3( CCmdUI *pCmdUI );

	afx_msg void OnTypeQuat( );
	afx_msg void OnUpdateTypeQuat( CCmdUI *pCmdUI );

	afx_msg void OnTypeMatrix( );
	afx_msg void OnUpdateTypeMatrix( CCmdUI *pCmdUI );

	afx_msg void OnTypeFloat( );
	afx_msg void OnUpdateTypeFloat( CCmdUI *pCmdUI );

	afx_msg void OnTypeDouble( );
	afx_msg void OnUpdateTypeDouble( CCmdUI *pCmdUI );

	afx_msg void OnTypeCustom( );
	afx_msg void OnUpdateTypeCustom( CCmdUI *pCmdUI );

	afx_msg void OnTypeUnicode( );
	afx_msg void OnUpdateTypeUnicode( CCmdUI *pCmdUI );

	afx_msg void OnTypeText( );
	afx_msg void OnUpdateTypeText( CCmdUI *pCmdUI );

	afx_msg void OnTypePChar( );
	afx_msg void OnUpdateTypePChar( CCmdUI *pCmdUI );

	afx_msg void OnTypePWChar( );
	afx_msg void OnUpdateTypePWChar( CCmdUI *pCmdUI );

	afx_msg void OnTypeVtable( );
	afx_msg void OnUpdateTypeVtable( CCmdUI *pCmdUI );

	afx_msg void OnTypeFunction( );
	afx_msg void OnUpdateTypeFunction( CCmdUI *pCmdUI );

	afx_msg void OnTypeFunctionPtr( );
	afx_msg void OnUpdateTypeFunctionPtr( CCmdUI *pCmdUI );

	afx_msg void OnTypePointer( );
	afx_msg void OnUpdateTypePointer( CCmdUI *pCmdUI );

	afx_msg void OnTypeArray( );
	afx_msg void OnUpdateTypeArray( CCmdUI *pCmdUI );

	afx_msg void OnTypeClass( );
	afx_msg void OnUpdateTypeClass( CCmdUI *pCmdUI );

	afx_msg void OnButtonEditcode( );
	afx_msg void OnEditCopy( );
	afx_msg void OnEditPaste( );
	afx_msg void OnMouseLeave( );

	afx_msg void OnButtonZero( );
	afx_msg void OnUpdateButtonZero( CCmdUI *pCmdUI );

	afx_msg void OnButtonOne( );
	afx_msg void OnUpdateButtonOne( CCmdUI *pCmdUI );

	afx_msg void OnButtonRandom( );
	afx_msg void OnUpdateButtonRandom( CCmdUI *pCmdUI );

	afx_msg void OnButtonSwap( );
	afx_msg void OnUpdateButtonSwap( CCmdUI *pCmdUI );

public:
	VOID SetClass( CNodeClass* pClass ) { m_pClass = pClass; };

private:
	CNodeClass* m_pClass;

	BOOLEAN m_bTracking;
	CPoint m_HoverPoint;

public:
	CMemory m_Memory;

	std::vector<HotSpot> HotSpots;
	std::vector<HotSpot> Selected;
	HotSpot ExchangeTarget;

	// Controls
	CCustomEdit m_Edit;
	CCustomToolTip m_ToolTip;

	CScrollBar m_Scroll;
	CScrollBar m_HScroll;

	bool isDeleting;
};

