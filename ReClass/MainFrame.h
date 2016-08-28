// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#define TIMER_MEMORYMAP_UPDATE 0xDEADF00D

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CMFCRibbonBar m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

public:
	//afx_msg void OnButtonShowclasses();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnButtonCBackground();
	afx_msg void OnButtonCSelect();
	afx_msg void OnButtonCHidden();
	afx_msg void OnButtonCOffset();
	afx_msg void OnButtonCAddress();
	afx_msg void OnButtonCHex();
	afx_msg void OnButtonCType();
	afx_msg void OnButtonCName();
	afx_msg void OnButtonCValue();
	afx_msg void OnButtonCIndex();
	afx_msg void OnButtonCComment();
	afx_msg void OnButtonCText();
	afx_msg void OnButtonCVtable();
	afx_msg void OnButtonCFunction();
	afx_msg void OnButtonCCustom();
	afx_msg void OnCheckCbAddress();
	afx_msg void OnUpdateCheckCbAddress(CCmdUI *pCmdUI);
	afx_msg void OnCheckCbOffset();
	afx_msg void OnUpdateCheckCbOffset(CCmdUI *pCmdUI);
	afx_msg void OnCheckCbText();
	afx_msg void OnUpdateCheckCbText(CCmdUI *pCmdUI);
	afx_msg void OnCheckCbRtti();
	afx_msg void OnUpdateCheckCbRtti(CCmdUI *pCmdUI);
	afx_msg void OnButtonTypedef();
	afx_msg void OnButtonSelectProcess();
	afx_msg void OnButtonEditClass();
	afx_msg void OnUpdateButtonEditClass(CCmdUI *pCmdUI);
	afx_msg void OnButtonDeleteClass();
	afx_msg void OnUpdateButtonDeleteClass(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCheckTopmost();
	afx_msg void OnUpdateCheckTopmost(CCmdUI *pCmdUI);
	afx_msg void OnCheckClassBrowser( );
	afx_msg void OnUpdateCheckClassBrowser(CCmdUI *pCmdUI);
	afx_msg void OnCheckPrivatePadding();
	afx_msg void OnUpdateCheckPrivatePadding(CCmdUI *pCmdUI);
	afx_msg void OnCheckClipboardCopy();
	afx_msg void OnUpdateCheckClipboardCopy(CCmdUI *pCmdUI);
	afx_msg void OnButtonLeft();
	afx_msg void OnButtonRight();
	afx_msg void OnCheckFloat();
	afx_msg void OnUpdateCheckFloat(CCmdUI *pCmdUI);
	afx_msg void OnCheckInteger();
	afx_msg void OnUpdateCheckInteger(CCmdUI *pCmdUI);
	afx_msg void OnCheckString();
	afx_msg void OnUpdateCheckString(CCmdUI *pCmdUI);
	afx_msg void OnCheckPointer();
	afx_msg void OnUpdateCheckPointer(CCmdUI *pCmdUI);
};


