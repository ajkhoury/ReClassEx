#pragma once

#define MAX_NODES 32768

class myCEdit : public CEdit
{
public:
	HotSpot spot;
	int MinWidth;
	HBRUSH m_brBackGnd;

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	DECLARE_MESSAGE_MAP( )
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnChange( );
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};

class myCToolTip : public CEdit
{
public:
	HBRUSH m_brBackGnd;
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
};

// CChildView window
class CChildView : public CWnd
{
// Construction
public:
	CChildView();
	virtual ~CChildView();

	CNodeClass* m_pClass;
	CMemory Memory;

	std::vector<HotSpot> HotSpots;
	std::vector<HotSpot> Selected;
	HotSpot ExchangeTarget;

	//Controls
	myCEdit m_Edit;
	myCToolTip m_ToolTip;

	CScrollBar m_Scroll;

	bool isDeleting;

// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	UINT		FindNodeIndex(CNodeBase* pNode);
	CNodeBase*	FindNodeFromIndex(CNodeBase* currentlySelectedNode, UINT index);

	void		ReplaceNode(CNodeClass* pClass,UINT idx,CNodeBase* pNewNode);
	void		RemoveNodes(CNodeClass* pClass,UINT idx,DWORD Length);
	void		FillNodes(CNodeClass* pClass,UINT idx,DWORD Length);
	void		ResizeNode(CNodeClass* pClass,UINT idx,DWORD before,DWORD After);
	void		AddBytes(CNodeClass* pClass,DWORD Length);
	void		InsertBytes(CNodeClass* pClass,UINT idx,DWORD Length);

	void		ReplaceSelectedWithType(NodeType Type);

	void		StandardTypeUpdate(CCmdUI *pCmdUI);

	// Generated message map functions
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnAddAdd8();
	afx_msg void OnUpdateAddAdd8(CCmdUI *pCmdUI);

	afx_msg void OnAddAdd64();
	afx_msg void OnUpdateAddAdd64(CCmdUI *pCmdUI);

	afx_msg void OnAddAdd1024();
	afx_msg void OnUpdateAddAdd1024(CCmdUI *pCmdUI);

	afx_msg void OnInsertInsert4();
	afx_msg void OnUpdateInsertInsert4(CCmdUI *pCmdUI);

	afx_msg void OnInsertInsert8();
	afx_msg void OnUpdateInsertInsert8(CCmdUI *pCmdUI);

	afx_msg void OnInsertInsert64();
	afx_msg void OnUpdateInsertInsert64(CCmdUI *pCmdUI);

	afx_msg void OnInsertInsert1024();
	afx_msg void OnUpdateInsertInsert1024(CCmdUI *pCmdUI);

	afx_msg void OnInsertInsert2048();
	afx_msg void OnUpdateInsertInsert2048(CCmdUI *pCmdUI);

	afx_msg void OnModifyDelete();
	afx_msg void OnUpdateModifyDelete(CCmdUI *pCmdUI);

	afx_msg void OnModifyShow();
	afx_msg void OnUpdateModifyShow(CCmdUI *pCmdUI);

	afx_msg void OnModifyHide();
	afx_msg void OnUpdateModifyHide(CCmdUI *pCmdUI);

	afx_msg void OnTypeHex64();
	afx_msg void OnUpdateTypeHex64(CCmdUI *pCmdUI);

	afx_msg void OnTypeHex32();
	afx_msg void OnUpdateTypeHex32(CCmdUI *pCmdUI);

	afx_msg void OnTypeHex16();
	afx_msg void OnUpdateTypeHex16(CCmdUI *pCmdUI);

	afx_msg void OnTypeHex8();
	afx_msg void OnUpdateTypeHex8(CCmdUI *pCmdUI);

	afx_msg void OnTypeInt64();
	afx_msg void OnUpdateTypeInt64(CCmdUI *pCmdUI);

	afx_msg void OnTypeInt32();
	afx_msg void OnUpdateTypeInt32(CCmdUI *pCmdUI);

	afx_msg void OnTypeInt16();
	afx_msg void OnUpdateTypeInt16(CCmdUI *pCmdUI);

	afx_msg void OnTypeInt8();
	afx_msg void OnUpdateTypeInt8(CCmdUI *pCmdUI);

	afx_msg void OnTypeDword();
	afx_msg void OnUpdateTypeDword(CCmdUI *pCmdUI);

	afx_msg void OnTypeWord();
	afx_msg void OnUpdateTypeWord(CCmdUI *pCmdUI);

	afx_msg void OnTypeByte();
	afx_msg void OnUpdateTypeByte(CCmdUI *pCmdUI);

	afx_msg void OnTypeVec2();
	afx_msg void OnUpdateTypeVec2(CCmdUI *pCmdUI);

	afx_msg void OnTypeVec3();
	afx_msg void OnUpdateTypeVec3(CCmdUI *pCmdUI);

	afx_msg void OnTypeQuat();
	afx_msg void OnUpdateTypeQuat(CCmdUI *pCmdUI);

	afx_msg void OnTypeMatrix();
	afx_msg void OnUpdateTypeMatrix(CCmdUI *pCmdUI);

	afx_msg void OnTypeFloat();
	afx_msg void OnUpdateTypeFloat(CCmdUI *pCmdUI);

	afx_msg void OnTypeDouble();
	afx_msg void OnUpdateTypeDouble(CCmdUI *pCmdUI);

	afx_msg void OnTypeCustom();
	afx_msg void OnUpdateTypeCustom(CCmdUI *pCmdUI);

	afx_msg void OnTypeUnicode();
	afx_msg void OnUpdateTypeUnicode(CCmdUI *pCmdUI);

	afx_msg void OnTypeText();
	afx_msg void OnUpdateTypeText(CCmdUI *pCmdUI);

	afx_msg void OnTypePChar();
	afx_msg void OnUpdateTypePChar(CCmdUI *pCmdUI);

	afx_msg void OnTypeVtable();
	afx_msg void OnUpdateTypeVtable(CCmdUI *pCmdUI);

	afx_msg void OnTypeFunction();
	afx_msg void OnUpdateTypeFunction(CCmdUI *pCmdUI);

	afx_msg void OnTypePointer();
	afx_msg void OnUpdateTypePointer(CCmdUI *pCmdUI);

	afx_msg void OnTypeArray();
	afx_msg void OnUpdateTypeArray(CCmdUI *pCmdUI);

	afx_msg void OnTypeClass();
	afx_msg void OnUpdateTypeClass(CCmdUI *pCmdUI);

	afx_msg void OnButtonEditcode();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnMouseLeave();

	afx_msg void OnButtonZero();
	afx_msg void OnUpdateButtonZero(CCmdUI *pCmdUI);

	afx_msg void OnButtonOne();
	afx_msg void OnUpdateButtonOne(CCmdUI *pCmdUI);

	afx_msg void OnButtonRandom();
	afx_msg void OnUpdateButtonRandom(CCmdUI *pCmdUI);
};

